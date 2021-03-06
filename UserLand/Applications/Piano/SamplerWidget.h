#pragma once

#include <LibGUI/Frame.h>

class TrackManager;

class WaveEditor final : public GUI::Frame {
    C_OBJECT(WaveEditor)
public:
    virtual ~WaveEditor() override;

private:
    explicit WaveEditor(TrackManager&);

    virtual void paint_event(GUI::PaintEvent&) override;

    int sample_to_y(double percentage) const;

    TrackManager& m_track_manager;
};

class SamplerWidget final : public GUI::Frame {
    C_OBJECT(SamplerWidget)
public:
    virtual ~SamplerWidget() override;

private:
    explicit SamplerWidget(TrackManager&);

    TrackManager& m_track_manager;

    RefPtr<GUI::Widget> m_open_button_and_recorded_sample_name_container;
    RefPtr<GUI::Button> m_open_button;
    RefPtr<GUI::Label> m_recorded_sample_name;
    RefPtr<WaveEditor> m_wave_editor;
};
