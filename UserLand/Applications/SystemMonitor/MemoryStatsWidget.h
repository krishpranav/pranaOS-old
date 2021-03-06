#pragma once

// includes
#include <LibGUI/Widget.h>

class GraphWidget;

class MemoryStatsWidget final : public GUI::Widget {
    C_OBJECT(MemoryStatsWidget)
public:
    static MemoryStatsWidget* the();

    virtual ~MemoryStatsWidget() override;

    void refresh();

private:
    MemoryStatsWidget(GraphWidget& graph);

    GraphWidget& m_graph;
    RefPtr<GUI::Label> m_user_physical_pages_label;
    RefPtr<GUI::Label> m_user_physical_pages_committed_label;
    RefPtr<GUI::Label> m_supervisor_physical_pages_label;
    RefPtr<GUI::Label> m_kmalloc_space_label;
    RefPtr<GUI::Label> m_kmalloc_count_label;
    RefPtr<GUI::Label> m_kfree_count_label;
    RefPtr<GUI::Label> m_kmalloc_difference_label;
};
