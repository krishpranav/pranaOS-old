#include "TrackManager.h"

TrackManager::TrackManager()
{
    add_track();
}

TrackManager::~TrackManager()
{
}

void TrackManager::fill_buffer(Span<Sample> buffer)
{
    memset(buffer.data(), 0, buffer_size);

    for (size_t i = 0; i < buffer.size(); ++i) {
        for (auto& track : m_tracks)
            track->fill_sample(buffer[i]);

        if (++m_time >= roll_length) {
            m_time = 0;
            if (!m_should_loop)
                break;
        }
    }

    memcpy(m_current_back_buffer.data(), buffer.data(), buffer_size);
    swap(m_current_front_buffer, m_current_back_buffer);
}

void TrackManager::reset()
{
    memset(m_front_buffer.data(), 0, buffer_size);
    memset(m_back_buffer.data(), 0, buffer_size);

    m_current_front_buffer = m_front_buffer.span();
    m_current_back_buffer = m_back_buffer.span();

    m_time = 0;

    for (auto& track : m_tracks)
        track->reset();
}

void TrackManager::set_note_current_octave(int note, Switch switch_note)
{
    current_track().set_note(note + octave_base(), switch_note);
}

void TrackManager::set_octave(Direction direction)
{
    if (direction == Up) {
        if (m_octave < octave_max)
            ++m_octave;
    } else {
        if (m_octave > octave_min)
            --m_octave;
    }
}

void TrackManager::set_octave(int octave)
{
    if (octave <= octave_max && octave >= octave_min) {
        m_octave = octave;
    }
}

void TrackManager::add_track()
{
    m_tracks.append(make<Track>(m_time));
}

void TrackManager::next_track()
{
    if (++m_current_track >= m_tracks.size())
        m_current_track = 0;
}
