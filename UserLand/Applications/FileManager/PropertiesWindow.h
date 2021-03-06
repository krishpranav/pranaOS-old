#pragma once

#include <LibCore/File.h>
#include <LibGUI/Button.h>
#include <LibGUI/Dialog.h>
#include <LibGUI/FileSystemModel.h>
#include <LibGUI/ImageWidget.h>
#include <LibGUI/Label.h>
#include <LibGUI/TextBox.h>

class PropertiesWindow final : public GUI::Window {
    C_OBJECT(PropertiesWindow);

public:
    virtual ~PropertiesWindow() override;

private:
    PropertiesWindow(const String& path, bool disable_rename, Window* parent = nullptr);

    struct PropertyValuePair {
        String property;
        String value;
        Optional<URL> link = {};
    };

    struct PermissionMasks {
        mode_t read;
        mode_t write;
        mode_t execute;
    };

    static const String get_description(const mode_t mode)
    {
        if (S_ISREG(mode))
            return "File";
        if (S_ISDIR(mode))
            return "Directory";
        if (S_ISLNK(mode))
            return "Symbolic link";
        if (S_ISCHR(mode))
            return "Character device";
        if (S_ISBLK(mode))
            return "Block device";
        if (S_ISFIFO(mode))
            return "FIFO (named pipe)";
        if (S_ISSOCK(mode))
            return "Socket";
        if (mode & S_IXUSR)
            return "Executable";

        return "Unknown";
    }

    GUI::Button& make_button(String, GUI::Widget& parent);
    void make_property_value_pairs(const Vector<PropertyValuePair>& pairs, GUI::Widget& parent);
    void make_permission_checkboxes(GUI::Widget& parent, PermissionMasks, String label_string, mode_t mode);
    void permission_changed(mode_t mask, bool set);
    bool apply_changes();
    void update();
    String make_full_path(const String& name);

    RefPtr<GUI::Button> m_apply_button;
    RefPtr<GUI::TextBox> m_name_box;
    RefPtr<GUI::ImageWidget> m_icon;
    String m_name;
    String m_parent_path;
    String m_path;
    mode_t m_mode { 0 };
    mode_t m_old_mode { 0 };
    bool m_permissions_dirty { false };
    bool m_name_dirty { false };
};
