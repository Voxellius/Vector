@GUI::Widget {
    fill_with_background_color: true

    layout: @GUI::VerticalBoxLayout {
        spacing: 2
    }

    @GUI::ToolbarContainer {
        @GUI::Toolbar {
            name: "main_toolbar"
        }
    }

    @GUI::TreeView {
        name: "rooms_tree"
    }
}
