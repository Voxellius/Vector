#include <LibGUI/Application.h>
#include <LibGUI/Window.h>
#include <LibGUI/ImageWidget.h>
#include <LibGUI/Button.h>
#include <LibGUI/MessageBox.h>

#include "LoginDialog.h"

// TODO: Get `serenity_main` working instead of classic UNIX `main`

int main(int argc, char* argv[]) {
    auto app = MUST(GUI::Application::try_create(argc, argv));
    auto window = MUST(LoginDialog::try_create());

    window->show();

    return app->exec();
}
