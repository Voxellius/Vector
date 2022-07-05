#pragma once

#include <AK/Function.h>
#include <LibGUI/Widget.h>

class LoginDialog final : public GUI::Window {
    C_OBJECT(LoginDialog)

    public:
        virtual ~LoginDialog() override = default;

        Function<void()> on_login_success;

        void attempt_login();

    private:
        LoginDialog();
};
