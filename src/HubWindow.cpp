#include <LibGUI/Application.h>
#include <LibGUI/Window.h>
#include <LibGUI/Widget.h>
#include <LibGUI/Toolbar.h>
#include <LibGfx/Bitmap.h>

#include "HubWindow.h"
#include "../build/HubWindowGML.h"
#include "common.h"
#include "Matrix.h"

HubWindow::HubWindow() {
    auto& matrix = Matrix::the();

    auto widget = MUST(try_set_main_widget<GUI::Widget>());

    widget->load_from_gml(hub_window_gml);

    auto window_icon_bitmap = MUST(Gfx::Bitmap::try_load_from_file("/usr/local/share/vector/icon_s.png"));
    auto& toolbar = WIDGET(widget, GUI::Toolbar, "toolbar");

    // TODO: Make better icon
    auto logout_action = GUI::Action::create(
        "&Log Out",
        {},
        Gfx::Bitmap::try_load_from_file("/res/icons/16x16/network-disconnected.png").release_value(),
        [&](auto&) {
            matrix.logout();
        }
    );

    toolbar.add_action(*logout_action);

    set_title("Vector IM");
    set_icon(window_icon_bitmap);
    resize(250, 500);
}
