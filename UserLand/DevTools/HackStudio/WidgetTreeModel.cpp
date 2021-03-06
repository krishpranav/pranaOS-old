// includes
#include "WidgetTreeModel.h"
#include <AK/StringBuilder.h>
#include <LibGUI/Widget.h>

namespace HackStudio {

WidgetTreeModel::WidgetTreeModel(GUI::Widget& root)
    : m_root(root)
{
    m_widget_icon.set_bitmap_for_size(16, Gfx::Bitmap::load_from_file("/res/icons/16x16/inspector-object.png"));
}

WidgetTreeModel::~WidgetTreeModel()
{
}

GUI::ModelIndex WidgetTreeModel::index(int row, int column, const GUI::ModelIndex& parent) const
{
    if (!parent.is_valid()) {
        return create_index(row, column, m_root.ptr());
    }
    auto& parent_node = *static_cast<GUI::Widget*>(parent.internal_data());
    return create_index(row, column, parent_node.child_widgets().at(row));
}

GUI::ModelIndex WidgetTreeModel::parent_index(const GUI::ModelIndex& index) const
{
    if (!index.is_valid())
        return {};
    auto& widget = *static_cast<GUI::Widget*>(index.internal_data());
    if (&widget == m_root.ptr())
        return {};

    if (widget.parent_widget() == m_root.ptr())
        return create_index(0, 0, m_root.ptr());

    // Walk the grandparent's children to find the index of widget's parent in its parent.
    // (This is needed to produce the row number of the GUI::ModelIndex corresponding to widget's parent.)
    int grandparent_child_index = 0;
    for (auto& grandparent_child : widget.parent_widget()->parent_widget()->child_widgets()) {
        if (grandparent_child == widget.parent_widget())
            return create_index(grandparent_child_index, 0, widget.parent_widget());
        ++grandparent_child_index;
    }

    VERIFY_NOT_REACHED();
    return {};
}

int WidgetTreeModel::row_count(const GUI::ModelIndex& index) const
{
    if (!index.is_valid())
        return 1;
    auto& widget = *static_cast<GUI::Widget*>(index.internal_data());
    return widget.child_widgets().size();
}

int WidgetTreeModel::column_count(const GUI::ModelIndex&) const
{
    return 1;
}

GUI::Variant WidgetTreeModel::data(const GUI::ModelIndex& index, GUI::ModelRole role) const
{
    auto* widget = static_cast<GUI::Widget*>(index.internal_data());
    if (role == GUI::ModelRole::Icon) {
        return m_widget_icon;
    }
    if (role == GUI::ModelRole::Display) {
        return String::formatted("{} ({})", widget->class_name(), widget->relative_rect());
    }
    return {};
}

void WidgetTreeModel::update()
{
    did_update();
}

GUI::ModelIndex WidgetTreeModel::index_for_widget(GUI::Widget& widget) const
{
    int parent_child_index = 0;
    for (auto& parent_child : widget.parent_widget()->child_widgets()) {
        if (parent_child == &widget)
            return create_index(parent_child_index, 0, &widget);
        ++parent_child_index;
    }
    return {};
}

}
