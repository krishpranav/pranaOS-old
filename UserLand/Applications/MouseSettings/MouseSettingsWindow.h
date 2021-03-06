#pragma once
// includes
#include <LibGUI/Button.h>
#include <LibGUI/Slider.h>
#include <LibGUI/SpinBox.h>
#include <LibGUI/Window.h>

class MouseSettingsWindow final : public GUI::Window {
    C_OBJECT(MouseSettingsWindow)
public:
    virtual ~MouseSettingsWindow() override;

private:
    MouseSettingsWindow();

    void update_window_server();
    void reset_default_values();

    RefPtr<GUI::HorizontalSlider> m_speed_slider;
    RefPtr<GUI::Label> m_speed_label;
    RefPtr<GUI::SpinBox> m_scroll_length_spinbox;
    RefPtr<GUI::HorizontalSlider> m_double_click_speed_slider;
    RefPtr<GUI::Label> m_double_click_speed_label;
    RefPtr<GUI::Button> m_ok_button;
    RefPtr<GUI::Button> m_apply_button;
    RefPtr<GUI::Button> m_reset_button;
};
