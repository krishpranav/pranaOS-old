#pragma once

// includes
#include <AK/Vector.h>
#include <LibGUI/Model.h>

class CharacterMapFileListModel final : public GUI::Model {
public:
    static NonnullRefPtr<CharacterMapFileListModel> create(Vector<String>& file_names)
    {
        return adopt(*new CharacterMapFileListModel(file_names));
    }

    virtual ~CharacterMapFileListModel() override { }

    virtual int row_count(const GUI::ModelIndex&) const override
    {
        return m_file_names.size();
    }

    virtual int column_count(const GUI::ModelIndex&) const override
    {
        return 1;
    }

    virtual GUI::Variant data(const GUI::ModelIndex& index, GUI::ModelRole role) const override
    {
        VERIFY(index.is_valid());
        VERIFY(index.column() == 0);

        if (role == GUI::ModelRole::Display)
            return m_file_names.at(index.row());

        return {};
    }

    virtual void update() override
    {
        did_update();
    }

private:
    explicit CharacterMapFileListModel(Vector<String>& file_names)
        : m_file_names(file_names)
    {
    }

    Vector<String>& m_file_names;
};
