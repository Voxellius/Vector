#pragma once

#include <LibGUI/Widget.h>

class LoginDialog final : public GUI::Window {
    C_OBJECT(LoginDialog)

    public:
        virtual ~LoginDialog() override = default;

    private:
        LoginDialog();
};
