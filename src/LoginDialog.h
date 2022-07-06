#pragma once

#include <AK/Function.h>
#include <LibGUI/Widget.h>

class LoginDialog final : public GUI::Window {
    C_OBJECT(LoginDialog)

    public:
        virtual ~LoginDialog() override = default;

        void attempt_login();

        Function<void()> on_login_success;

    private:
        LoginDialog();

        void set_interface_locking(bool locked);
};
