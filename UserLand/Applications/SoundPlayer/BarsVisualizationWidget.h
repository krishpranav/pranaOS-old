#pragma once

// includes
#include "VisualizationBase.h"
#include <AK/Complex.h>
#include <LibAudio/Buffer.h>
#include <LibGUI/Frame.h>

class BarsVisualizationWidget final : public GUI::Frame
    , public Visualization {
    C_OBJECT(BarsVisualizationWidget)

public:
    ~BarsVisualizationWidget() override;
    void set_buffer(RefPtr<Audio::Buffer> buffer) override;
    void set_samplerate(int samplerate) override;

private:
    BarsVisualizationWidget();
    void set_buffer(RefPtr<Audio::Buffer> buffer, int samples_to_use);

    void paint_event(GUI::PaintEvent&) override;
    void mousedown_event(GUI::MouseEvent& event) override;

    Vector<Complex<double>> m_sample_buffer;
    Vector<int> m_gfx_falling_bars;
    int m_last_id;
    int m_sample_count;
    int m_samplerate;
    bool m_is_using_last;
    bool m_adjust_frequencies;
    RefPtr<GUI::Menu> m_context_menu;
};