#include <LibGUI/Application.h>
#include <LibGUI/Window.h>
#include <LibGUI/Widget.h>
#include <LibGUI/MessageBox.h>
#include <LibGUI/ImageWidget.h>
#include <LibGUI/Button.h>
#include <LibGUI/TextBox.h>
#include <LibGfx/Bitmap.h>

#include "LoginDialog.h"
#include "../build/LoginDialogGML.h"
#include "common.h"
#include "Matrix.h"

LoginDialog::LoginDialog() {
    auto widget = MUST(try_set_main_widget<GUI::Widget>());

    widget->load_from_gml(login_dialog_gml);

    auto& icon = WIDGET(widget, GUI::ImageWidget, "icon");
    auto& login_button = WIDGET(widget, GUI::Button, "login_button");
    auto& cancel_button = WIDGET(widget, GUI::Button, "cancel_button");
    auto& password_input = WIDGET(main_widget(), GUI::TextBox, "password_input");
    auto window_icon_bitmap = MUST(Gfx::Bitmap::try_load_from_file("/usr/local/share/vector/icon_s.png"));

    icon.load_from_file("/res/icons/32x32/key.png");

    login_button.on_click = [&](auto) {
        attempt_login();
    };

    password_input.on_return_pressed = [&]() {
        attempt_login();
    };

    cancel_button.on_click = [&](auto) {
        close();
    };

    set_title("Log into Vector");
    set_icon(window_icon_bitmap);
    set_resizable(false);
    resize(450, 180);
}

void LoginDialog::set_interface_locking(bool locked) {
    WIDGET(main_widget(), GUI::TextBox, "homeserver_input").set_enabled(!locked);
    WIDGET(main_widget(), GUI::TextBox, "username_input").set_enabled(!locked);
    WIDGET(main_widget(), GUI::TextBox, "password_input").set_enabled(!locked);
    WIDGET(main_widget(), GUI::Button, "login_button").set_enabled(!locked);
    WIDGET(main_widget(), GUI::Button, "cancel_button").set_enabled(!locked);
}

void LoginDialog::attempt_login() {
    auto& matrix = Matrix::the();

    auto& homeserver_input = WIDGET(main_widget(), GUI::TextBox, "homeserver_input");
    auto& username_input = WIDGET(main_widget(), GUI::TextBox, "username_input");
    auto& password_input = WIDGET(main_widget(), GUI::TextBox, "password_input");

    if (homeserver_input.text() == "" || username_input.text() == "" || password_input.text() == "") {
        GUI::MessageBox::show(
            this,
            "Please enter all of your account's details",
            "Could not log in",
            GUI::MessageBox::Type::Error,
            GUI::MessageBox::InputType::OK
        );

        return;
    }

    matrix.on_login_success = [&]() {
        CALLBACK(on_login_success)();

        close();
    };

    matrix.on_login_failure = [&](String error_message) {
        set_interface_locking(false);

        GUI::MessageBox::show(
            this,
            error_message,
            "Could not log in",
            GUI::MessageBox::Type::Error,
            GUI::MessageBox::InputType::OK
        );
    };

    set_interface_locking(true);

    matrix.attempt_login(
        homeserver_input.text(),
        username_input.text(),
        password_input.text()
    );
}
