#include <AK/String.h>
#include <AK/URL.h>
#include <AK/JsonObject.h>
#include <AK/JsonValue.h>
#include <AK/Weakable.h>
#include <LibCore/Stream.h>
#include <LibCore/MemoryStream.h>
#include <LibConfig/Client.h>
#include <LibHTTP/HttpRequest.h>
#include <LibHTTP/HttpsJob.h>

#include "Matrix.h"
#include "common.h"

Request::Request(AK::URL url, HTTP::HttpRequest::Method method, ByteBuffer body) {
    HTTP::HttpRequest request;

    request.set_url(url);
    request.set_method(method);
    request.set_body(MUST(ByteBuffer::copy(body)));

    m_stream_backing_buffer = MUST(ByteBuffer::create_uninitialized(1 * MiB));
    m_output_stream = Core::Stream::MemoryStream::construct(m_stream_backing_buffer.bytes()).release_value();

    m_job = HTTP::HttpsJob::construct(move(request), *m_output_stream);
    m_completed = false;

    m_job->on_headers_received = [weak_this = make_weak_ptr()](auto& response_headers, auto response_code) mutable {
        if (auto strong_this = weak_this.strong_ref()) {
            strong_this->m_response_code = response_code;

            for (auto& header : response_headers) {
                strong_this->m_response_headers.set(header.key, header.value);
            }
        }
    };

    m_job->on_finish = [weak_this = make_weak_ptr()](bool success) mutable {
        if (auto strong_this = weak_this.strong_ref()) {
            ReadonlyBytes response_bytes {
                strong_this->m_output_stream->bytes().data(),
                strong_this->m_output_stream->offset()
            };

            if (!success) {
                dbgln("Unsuccessful request");

                CALLBACK(strong_this->on_failure)();

                return;
            }

            strong_this->m_completed;
            strong_this->m_response_buffer = ByteBuffer::copy(response_bytes).release_value();

            CALLBACK(strong_this->on_response)();
        }
    };

    auto underlying_socket_result = TLS::TLSv12::connect(url.host(), url.port().value_or(443));

    if (m_socket_connection_failed = underlying_socket_result.is_error()) {
        return;
    }

    auto underlying_socket = underlying_socket_result.release_value();

    MUST(underlying_socket->set_blocking(false));

    m_socket = MUST(Core::Stream::BufferedSocket<TLS::TLSv12>::create(move(underlying_socket)));
}

ErrorOr<NonnullRefPtr<Request>> Request::create(AK::URL url, HTTP::HttpRequest::Method method, ByteBuffer body) {
    return adopt_ref(*new Request(url, method, body));
}

void Request::start() {
    if (m_socket_connection_failed) {
        CALLBACK(on_failure)();

        return;
    }

    m_job->start(*m_socket);
}

Matrix::Matrix() {
    // TODO: Try and get config to work without `ConfigServer`
    m_access_token = Config::read_string("Vector", "Account", "AccessToken", "");

    m_is_logged_in = m_access_token != "";
}

Matrix& Matrix::the() {
    static Matrix matrix;

    return matrix;
}

Error Matrix::get_error_from_code(String error_code) {
    dbgln("Error {} occurred", error_code);

    if (error_code == "M_FORBIDDEN") {
        return Error::from_string_literal("The username or password used for authentication is incorrect");
    }

    if (error_code == "M_USER_DEACTIVATED") {
        return Error::from_string_literal("The account was deactivated by the homeserver");
    }

    if (error_code == "M_INVALID_PARAM") {
        return Error::from_string_literal("Vector made an invalid request (Vector may need updating)");
    }

    return Error::from_string_literal("An unknown error occurred");
}

String Matrix::construct_login_json(String username, String password) {
    JsonObject object;

    object.set("type", "m.login.password");
    object.set("user", JsonValue(username));
    object.set("password", JsonValue(password));

    return object.to_string();
}

ErrorOr<void> Matrix::consume_login_json(String login_json) {
    auto result = JsonValue::from_string(login_json);

    if (result.is_error()) {
        return Error::from_string_literal("Could not parse JSON response");
    }

    JsonObject object = result.release_value().as_object();

    if (object.has("errcode")) {
        return get_error_from_code(object.get("errcode").as_string());
    }

    m_is_logged_in = true;
    m_access_token = object.get("access_token").as_string();

    Config::write_string("Vector", "Account", "AccessToken", m_access_token);

    return {};
}

void Matrix::attempt_login(String homeserver, String username, String password) {
    auto login_json = Matrix::construct_login_json(username, password);
    auto url = AK::URL(String::formatted("https://{}/_matrix/client/r0/login", homeserver));

    m_homeserver = homeserver;
    m_login_request = MUST(Request::create(url, HTTP::HttpRequest::Method::POST, login_json.to_byte_buffer()));

    m_login_request->on_failure = [&]() {
        CALLBACK(on_login_failure)("Could not connect to the homeserver, check the internet connection and try again");
    };

    m_login_request->on_response = [&]() {
        auto result = consume_login_json(AK::String::copy(m_login_request->get_response_buffer().bytes()));

        if (result.is_error()) {
            CALLBACK(on_login_failure)(result.release_error().string_literal());

            return;
        }

        CALLBACK(on_login_success)();
    };

    m_login_request->start();
}

void Matrix::logout() {
    Config::write_string("Vector", "Account", "AccessToken", "");

    CALLBACK(on_logout)();
}
