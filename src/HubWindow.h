#pragma once

#include <LibGUI/Widget.h>
#include <LibGUI/TreeViewModel.h>

class HubWindow final : public GUI::Window {
    C_OBJECT(HubWindow)

    public:
        virtual ~HubWindow() override = default;

        void sync();

    private:
        HubWindow();

        RefPtr<GUI::TreeViewModel> m_tree_model;
};
