@GUI::Widget {
    fill_with_background_color: true

    layout: @GUI::HorizontalBoxLayout {
        margins: [10]
        spacing: 10
    }

    @GUI::Widget {
        fixed_width: 32
        layout: @GUI::VerticalBoxLayout {}

        @GUI::ImageWidget {
            text: "hi"
            name: "icon"
        }

        @GUI::Widget {}
    }

    @GUI::Widget {
        layout: @GUI::VerticalBoxLayout {}

        @GUI::Label {
            text: "Please log in with your Matrix account details to access Vector."
            text_alignment: "TopLeft"
        }

        @GUI::Widget {}

        @GUI::Widget {
            fixed_height: 24
            layout: @GUI::HorizontalBoxLayout {}

            @GUI::Label {
                text: "Homeserver:"
                text_alignment: "CenterLeft"
                preferred_width: 80
            }

            @GUI::TextBox {
                name: "homeserver_input"
                text: "matrix.org"
            }
        }

        @GUI::Widget {
            fixed_height: 24
            layout: @GUI::HorizontalBoxLayout {}

            @GUI::Label {
                text: "Username:"
                text_alignment: "CenterLeft"
                preferred_width: 80
            }

            @GUI::TextBox {
                name: "username_input"
            }
        }

        @GUI::Widget {
            fixed_height: 24
            layout: @GUI::HorizontalBoxLayout {}

            @GUI::Label {
                text: "Password:"
                text_alignment: "CenterLeft"
                preferred_width: 80
            }

            @GUI::PasswordBox {
                name: "password_input"
            }
        }

        @GUI::Widget {}

        @GUI::Widget {
            fixed_height: 24
            layout: @GUI::HorizontalBoxLayout {}

            @GUI::Widget {}

            @GUI::Button {
                name: "login_button"
                text: "Log In"
                fixed_width: 75
            }

            @GUI::Button {
                name: "cancel_button"
                text: "Cancel"
                fixed_width: 75
            }
        }
    }
}
