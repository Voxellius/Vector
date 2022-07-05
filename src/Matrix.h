#pragma once

#include <AK/String.h>
#include <LibHTTP/HttpsJob.h>

class Matrix {
    public:
        Matrix();
        ~Matrix() = default;

        static Matrix* the();

        static String construct_login_json(String username, String password);
        void attempt_login(String homeserver, String username, String password);

    protected:
        HTTP::HttpsJob* job;
};
