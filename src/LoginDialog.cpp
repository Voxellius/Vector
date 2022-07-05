#include <LibGUI/Application.h>
#include <LibGUI/Window.h>
#include <LibGUI/Widget.h>
#include <LibGUI/ImageWidget.h>
#include <LibGUI/Button.h>
#include <LibGUI/TextBox.h>
#include <LibGfx/Bitmap.h>

#include "LoginDialog.h"
#include "../build/LoginDialogGML.h"
#include "Matrix.h"

LoginDialog::LoginDialog() {
    auto widget = MUST(try_set_main_widget<GUI::Widget>());

    widget->load_from_gml(login_dialog_gml);

    auto& icon = *widget->find_descendant_of_type_named<GUI::ImageWidget>("icon");
    auto& login_button = *widget->find_descendant_of_type_named<GUI::Button>("login_button");
    auto& cancel_button = *widget->find_descendant_of_type_named<GUI::Button>("cancel_button");
    auto window_icon_bitmap = MUST(Gfx::Bitmap::try_load_from_file("/usr/local/share/vector/icon_s.png"));

    icon.load_from_file("/res/icons/32x32/key.png");

    login_button.on_click = [&](auto) {
        // TODO: Add auth checks before reporting success

        attempt_login();

        if (on_login_success) {
            on_login_success();
        }

        close();
    };

    cancel_button.on_click = [&](auto) {
        close();
    };

    set_title("Log into Vector");
    set_icon(window_icon_bitmap);
    set_resizable(false);
    resize(450, 180);
}

void LoginDialog::attempt_login() {
    auto* matrix = Matrix::the();

    auto& homeserver_input = *main_widget()->find_descendant_of_type_named<GUI::TextBox>("homeserver_input");
    auto& username_input = *main_widget()->find_descendant_of_type_named<GUI::TextBox>("username_input");
    auto& password_input = *main_widget()->find_descendant_of_type_named<GUI::TextBox>("password_input");

    matrix->attempt_login(
        homeserver_input.text(),
        username_input.text(),
        password_input.text()
    );
}
