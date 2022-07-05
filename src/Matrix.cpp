#include <AK/String.h>
#include <AK/URL.h>
#include <AK/JsonObject.h>
#include <AK/JsonValue.h>
#include <LibCore/Stream.h>
#include <LibCore/MemoryStream.h>
#include <LibHTTP/HttpRequest.h>
#include <LibHTTP/HttpsJob.h>

#include "Matrix.h"

Matrix matrix;

Matrix::Matrix() {}

Matrix* Matrix::the() {
    return &matrix;
}

String Matrix::construct_login_json(String username, String password) {
    JsonObject object;

    object.set("type", "m.login.password");
    object.set("user", JsonValue(username));
    object.set("password", JsonValue(password));

    return object.to_string();
}

void read() {
    dbgln("Reading!");
}

void Matrix::attempt_login(String homeserver, String username, String password) {
    auto login_json = Matrix::construct_login_json(username, password);
    auto url_str = String::formatted("https://{}/_matrix/client/r0/login", homeserver);
    auto url = AK::URL(url_str);

    HTTP::HttpRequest request;

    request.set_url(url);
    request.set_method(HTTP::HttpRequest::Method::POST);
    request.set_body(login_json.to_byte_buffer());

    auto backing_buffer = MUST(ByteBuffer::create_uninitialized(1 * MiB));
    auto streamConstruction = Core::Stream::MemoryStream::construct(backing_buffer.bytes());
    auto stream = *streamConstruction.value().ptr();
    auto underlying_socket = MUST(Core::Stream::TCPSocket::connect(url.host(), url.port().value_or(443)));
    auto socket = MUST(Core::Stream::BufferedSocket<Core::Stream::TCPSocket>::create(move(underlying_socket)));

    job = HTTP::HttpsJob::construct(move(request), stream);

    job->on_headers_received = [&](auto& response_headers, auto response_code) mutable {};

    job->on_finish = [&](bool success) mutable {
        if (success) {
            dbgln("Success!");
        } else {
            dbgln("Not a success!");
        }
    };

    dbgln("Fine");

    job->start(*socket.ptr());

    dbgln("Okay");
}
