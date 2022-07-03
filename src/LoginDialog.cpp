#include <LibGUI/Application.h>
#include <LibGUI/Window.h>
#include <LibGUI/Widget.h>
#include <LibGUI/ImageWidget.h>
#include <LibGfx/Bitmap.h>

#include "LoginDialog.h"
#include "../build/LoginDialogGML.h"

LoginDialog::LoginDialog() {
    auto widget = MUST(try_set_main_widget<GUI::Widget>());

    widget->load_from_gml(login_dialog_gml);

    auto& icon = *widget->find_descendant_of_type_named<GUI::ImageWidget>("icon");
    auto window_icon_bitmap = MUST(Gfx::Bitmap::try_load_from_file("/usr/local/share/vector/icon_s.png"));

    icon.load_from_file("/res/icons/32x32/key.png");

    set_title("Log into Vector");
    set_icon(window_icon_bitmap);
    set_resizable(false);
    resize(450, 180);
}
