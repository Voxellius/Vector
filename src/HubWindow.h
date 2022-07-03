#pragma once

#include <LibGUI/Widget.h>

class HubWindow final : public GUI::Window {
    C_OBJECT(HubWindow)

    public:
        virtual ~HubWindow() override = default;

    private:
        HubWindow();
};
