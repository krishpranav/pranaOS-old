#pragma once

// includes

#include <LibGUI/Calendar.h>
#include <LibGUI/Dialog.h>
#include <LibGUI/Model.h>
#include <LibGUI/Window.h>

class AddEventDialog final : public GUI::Dialog {
    C_OBJECT(AddEventDialog)
public:
    virtual ~AddEventDialog() override;

    static void show(Core::DateTime date_time, Window* parent_window = nullptr)
    {
        auto dialog = AddEventDialog::construct(date_time, parent_window);
        dialog->exec();
    }

private:
    AddEventDialog(Core::DateTime date_time, Window* parent_window = nullptr);

    class MonthListModel final : public GUI::Model {
    public:
        enum Column {
            Month,
            __Count,
        };

        static NonnullRefPtr<MonthListModel> create() { return adopt(*new MonthListModel); }
        virtual ~MonthListModel() override;

        virtual int row_count(const GUI::ModelIndex& = GUI::ModelIndex()) const override;
        virtual int column_count(const GUI::ModelIndex& = GUI::ModelIndex()) const override { return Column::__Count; }
        virtual String column_name(int) const override;
        virtual GUI::Variant data(const GUI::ModelIndex&, GUI::ModelRole) const override;
        virtual void update() override;

    private:
        MonthListModel();
    };

    Core::DateTime m_date_time;
};