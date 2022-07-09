#include <LibGUI/Application.h>
#include <LibGUI/Window.h>
#include <LibGUI/Widget.h>
#include <LibGUI/Icon.h>
#include <LibGUI/Toolbar.h>
#include <LibGUI/TreeView.h>
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
    auto my_rooms_icon = GUI::Icon(MUST(Gfx::Bitmap::try_load_from_file("/res/icons/16x16/plus.png"))); // TODO: Make better icon
    auto& toolbar = WIDGET(widget, GUI::Toolbar, "toolbar");
    auto& tree = WIDGET(widget, GUI::TreeView, "tree");

    // TODO: Make better icon
    auto logout_action = GUI::Action::create(
        "&Log Out",
        {},
        MUST(Gfx::Bitmap::try_load_from_file("/res/icons/16x16/network-disconnected.png")),
        [&](auto&) {
            matrix.logout();
        }
    );

    toolbar.add_action(*logout_action);

    m_tree_model = GUI::TreeViewModel::create();

    auto my_rooms = m_tree_model->add_node("My Rooms", my_rooms_icon);

    tree.set_model(m_tree_model);

    set_title("Vector IM");
    set_icon(window_icon_bitmap);
    resize(250, 500);

    sync();
}

void HubWindow::sync() {
    auto& matrix = Matrix::the();

    matrix.sync(); // TODO: Handle errors
}
