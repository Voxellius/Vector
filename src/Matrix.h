#pragma once

#include <AK/String.h>
#include <AK/HashTable.h>
#include <AK/URL.h>
#include <LibCore/MemoryStream.h>
#include <LibHTTP/HttpRequest.h>
#include <LibHTTP/HttpsJob.h>

typedef HashMap<String, String, CaseInsensitiveStringTraits> Headers;

class Request : public RefCounted<Request>, public Weakable<Request> {
    public:
        static ErrorOr<NonnullRefPtr<Request>> create(AK::URL url, HTTP::HttpRequest::Method method = HTTP::HttpRequest::Method::GET, ByteBuffer body = AK::String("").to_byte_buffer());

        Function<void()> on_response;

        void start();

    protected:
        bool m_completed;
        Optional<u32> m_response_code;
        Headers m_response_headers;
        ByteBuffer m_response_buffer;

    private:
        Request(AK::URL, HTTP::HttpRequest::Method method, ByteBuffer body);

        RefPtr<HTTP::HttpsJob> m_job;
        ByteBuffer m_stream_backing_buffer;
        OwnPtr<Core::Stream::MemoryStream> m_output_stream;
        OwnPtr<Core::Stream::BufferedSocketBase> m_socket;
};

class Matrix {
    AK_MAKE_NONCOPYABLE(Matrix);
    AK_MAKE_NONMOVABLE(Matrix);

    public:
        ~Matrix() = default;

        static Matrix& the();

        void attempt_login(String homeserver, String username, String password);

        Function<void()> on_login_success;

    private:
        Matrix() {}

        static String construct_login_json(String username, String password);

        RefPtr<Request> m_login_request;
};
