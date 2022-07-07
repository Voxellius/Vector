#include <LibGUI/Application.h>
#include <LibGUI/Window.h>
#include <LibGUI/ImageWidget.h>
#include <LibGUI/Button.h>
#include <LibGUI/MessageBox.h>

#include "Matrix.h"
#include "LoginDialog.h"
#include "HubWindow.h"

// TODO: Get `serenity_main` working instead of classic UNIX `main`

int main(int argc, char* argv[]) {
    auto app = MUST(GUI::Application::try_create(argc, argv));
    auto& matrix = Matrix::the();

    auto login_dialog = MUST(LoginDialog::try_create());
    auto hub_window = MUST(HubWindow::try_create());

    login_dialog->on_login_success = [&]() {
        hub_window->show();
    };

    if (matrix.is_logged_in()) {
        hub_window->show();
    } else {
        login_dialog->show();
    }

    return app->exec();
}
