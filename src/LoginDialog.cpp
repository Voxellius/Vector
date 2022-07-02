#include <LibGUI/Application.h>
#include <LibGUI/Window.h>
#include <LibGUI/Widget.h>
#include <LibGUI/ImageWidget.h>

#include "LoginDialog.h"
#include "../LoginDialogGML.h"

LoginDialog::LoginDialog() {
    auto widget = MUST(try_set_main_widget<GUI::Widget>());

    widget->load_from_gml(login_dialog_gml);

    auto& icon = *widget->find_descendant_of_type_named<GUI::ImageWidget>("icon");

    icon.load_from_file("/res/icons/32x32/key.png");

    set_title("Log into Vector");
    set_resizable(false);
    resize(450, 180);
}
