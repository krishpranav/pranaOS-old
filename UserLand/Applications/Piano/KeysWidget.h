#pragma once

#include "Music.h"
#include <LibGUI/Frame.h>

class TrackManager;

class KeysWidget final : public GUI::Frame {
    C_OBJECT(KeysWidget)
public:
    virtual ~KeysWidget() override;

    int key_code_to_key(int key_code) const;
    int mouse_note() const;

    void set_key(int key, Switch);
    bool note_is_set(int note) const;

private:
    explicit KeysWidget(TrackManager&);

    virtual void paint_event(GUI::PaintEvent&) override;
    virtual void mousedown_event(GUI::MouseEvent&) override;
    virtual void mouseup_event(GUI::MouseEvent&) override;
    virtual void mousemove_event(GUI::MouseEvent&) override;

    int note_for_event_position(const Gfx::IntPoint&) const;

    TrackManager& m_track_manager;

    u8 m_key_on[note_count] { 0 };

    bool m_mouse_down { false };
    int m_mouse_note { -1 };
};