#include <LibGUI/Application.h>
#include <LibGUI/Window.h>
#include <LibGUI/Widget.h>
#include <LibGfx/Bitmap.h>

#include "HubWindow.h"
#include "../build/HubWindowGML.h"

HubWindow::HubWindow() {
    auto widget = MUST(try_set_main_widget<GUI::Widget>());

    widget->load_from_gml(hub_window_gml);

    auto window_icon_bitmap = MUST(Gfx::Bitmap::try_load_from_file("/usr/local/share/vector/icon_s.png"));

    set_title("Vector IM");
    set_icon(window_icon_bitmap);
    resize(250, 500);
}
