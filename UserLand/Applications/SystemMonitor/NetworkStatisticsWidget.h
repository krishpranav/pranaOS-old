#pragma once

// includes
#include <LibCore/Timer.h>
#include <LibGUI/LazyWidget.h>

class NetworkStatisticsWidget final : public GUI::LazyWidget {
    C_OBJECT(NetworkStatisticsWidget)
public:
    virtual ~NetworkStatisticsWidget() override;

private:
    NetworkStatisticsWidget();
    void update_models();

    RefPtr<GUI::TableView> m_adapter_table_view;
    RefPtr<GUI::TableView> m_socket_table_view;
    RefPtr<GUI::JsonArrayModel> m_adapter_model;
    RefPtr<GUI::JsonArrayModel> m_socket_model;
    RefPtr<Core::Timer> m_update_timer;
};
