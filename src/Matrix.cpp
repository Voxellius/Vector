#include <AK/String.h>
#include <AK/URL.h>
#include <AK/JsonObject.h>
#include <AK/JsonValue.h>
#include <AK/Weakable.h>
#include <LibCore/Stream.h>
#include <LibCore/MemoryStream.h>
#include <LibHTTP/HttpRequest.h>
#include <LibHTTP/HttpsJob.h>

#include "Matrix.h"

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
        if (!success) {
            dbgln("Unsuccessful request");

            return;
        }

        if (auto strong_this = weak_this.strong_ref()) {
            ReadonlyBytes response_bytes {
                strong_this->m_output_stream->bytes().data(),
                strong_this->m_output_stream->offset()
            };

            strong_this->m_completed;
            strong_this->m_response_buffer = ByteBuffer::copy(response_bytes).release_value();

            if (strong_this->on_response) {
                strong_this->on_response();
            }
        }
    };

    auto underlying_socket = MUST(TLS::TLSv12::connect(url.host(), url.port().value_or(443)));

    MUST(underlying_socket->set_blocking(false));

    m_socket = MUST(Core::Stream::BufferedSocket<TLS::TLSv12>::create(move(underlying_socket)));
}

ErrorOr<NonnullRefPtr<Request>> Request::create(AK::URL url, HTTP::HttpRequest::Method method, ByteBuffer body) {
    return adopt_ref(*new Request(url, method, body));
}

void Request::start() {
    m_job->start(*m_socket);
}

Matrix& Matrix::the() {
    static Matrix matrix;

    return matrix;
}

String Matrix::construct_login_json(String username, String password) {
    JsonObject object;

    object.set("type", "m.login.password");
    object.set("user", JsonValue(username));
    object.set("password", JsonValue(password));

    return object.to_string();
}

void Matrix::attempt_login(String homeserver, String username, String password) {
    auto login_json = Matrix::construct_login_json(username, password);
    auto url = AK::URL(String::formatted("https://{}/_matrix/client/r0/login", homeserver));

    m_login_request = MUST(Request::create(url, HTTP::HttpRequest::Method::POST, login_json.to_byte_buffer()));

    m_login_request->on_response = [&]() {
        // TODO: Add auth checks before indicating success

        dbgln("{}", AK::String::copy(m_login_request->get_response_buffer().bytes()));

        if (on_login_success) {
            on_login_success();
        }
    };

    m_login_request->start();
}
