@GUI::Widget {
    fill_with_background_color: true

    layout: @GUI::VerticalBoxLayout {
        margins: [8]
    }

    @GUI::ImageWidget {
        name: "icon"
    }

    @GUI::Label {
        text: "Hello, world!"
        font_weight: "Bold"
    }

    @GUI::Label {
        text: "Welcome to my first SerenityOS application!"
    }

    @GUI::Label {
        text: "By @jthecoder"
    }

    @GUI::VerticalSplitter {}

    @GUI::Widget {
        preferred_height: "fit"

        layout: @GUI::HorizontalBoxLayout {
            spacing: 8
        }

        @GUI::Button {
            text: "Demo"
            name: "demo_button"
        }

        @GUI::Button {
            text: "Close"
            name: "close_button"
        }
    }
}
