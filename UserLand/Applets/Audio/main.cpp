//includes
#include <LibAudio/ClientConnection.h>
#include <LibGUI/Application.h>
#include <LibGUI/BoxLayout.h>
#include <LibGUI/CheckBox.h>
#include <LibGUI/Label.h>
#include <LibGUI/Painter.h>
#include <LibGUI/Slider.h>
#include <LibGUI/Widget.h>
#include <LibGUI/Window.h>
#include <LibGfx/Bitmap.h>
#include <LibGfx/Font.h>
#include <LibGfx/FontDatabase.h>
#include <LibGfx/Palette.h>

class AudioWidget final : public GUI::Widget{
    C_OBJECT(AudioWidget)
public:
    AudioWidget()
        : m_audio_client(Audio::ClientConnection::construct())
    {
        m_audio_client->on_muted_state_change = [this](bool muted){
            if (m_audio_muted == muted)
                return;
            m_mute_box->set_checked(!m_audio_muted)
            m_slider->set_enbled(!muted);
            m_audio_muted = muted;
            update();
        };

        m_audio_client->on_main_mix_volume_change = [this](int volume){
            m_audio_volume = volume;
            if (!m_audio_muted)
                update();
        };

        
    }

}