#pragma once

// includes
#include "Music.h"
#include <LibGUI/Widget.h>

// classes
class TrackManager;
class WaveWidget;
class RollWidget;
class SamplerWidget;
class KeysWidget;
class KnobsWidget;

class MainWidget final : public GUI::Widget {
    C_OBJECT(MainWidget)
public:
    virtual ~MainWidget() override;

    void add_actions(GUI::Menu&);

    void set_octave_and_ensure_note_change(Direction);
    void set_octave_and_ensure_note_change(int);

private:
    explicit MainWidget(TrackManager&);

    virtual void keydown_event(GUI::KeyEvent&) override;
    virtual void keyup_event(GUI::KeyEvent&) override;
    virtual void custom_event(Core::CustomEvent&) override;

    void note_key_action(int key_code, Switch);
    void special_key_action(int key_code);

    void turn_off_pressed_keys();
    void turn_on_pressed_keys();

    TrackManager& m_track_manager;

    RefPtr<WaveWidget> m_wave_widget;
    RefPtr<RollWidget> m_roll_widget;
    RefPtr<SamplerWidget> m_sampler_widget;
    RefPtr<GUI::TabWidget> m_tab_widget;
    RefPtr<GUI::Widget> m_keys_and_knobs_container;
    RefPtr<KeysWidget> m_keys_widget;
    RefPtr<KnobsWidget> m_knobs_widget;

    bool m_keys_pressed[key_code_count] { false };
};
