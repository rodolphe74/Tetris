#include "audioStream.h"

// initialize static member buffer;
std::vector<sf::Int16> AudioStream::m_ivecsamples;

bool
AudioStream::onGetData(Chunk& data)
{
    // set the pointer to the next audio samples to be played
    data.samples = &m_ivecsamples[m_currentSample];

    // have we reached the end of the sound?
    if (m_currentSample + SAMPLES_TO_STREAM <= m_ivecsamples.size()) {
        // end not reached: stream the samples and continue
        data.sampleCount = SAMPLES_TO_STREAM;
        m_currentSample += SAMPLES_TO_STREAM;

        if (isPlaying)
            computePeakAmp((int)data.sampleCount);

        return true;
    } else {
        // end of stream reached: stream the remaining samples and stop
        // playback
        data.sampleCount = m_ivecsamples.size() - m_currentSample;
        m_currentSample = m_ivecsamples.size();

        if (isPlaying)
            computePeakAmp((int)data.sampleCount);

        return false;
    }
}

void
AudioStream::onSeek(sf::Time timeOffset)
{
    // compute the corresponding sample index according to the sample rate
    // and channel count
    m_currentSample = static_cast<std::size_t>(
      timeOffset.asSeconds() * getSampleRate() * getChannelCount());
}

void
AudioStream::load(sf::SoundBuffer& buffer)
{
    // extract the audio samples from the sound buffer to our own container
    m_ivecsamples.assign(buffer.getSamples(),
                     buffer.getSamples() + buffer.getSampleCount());

    // reset the current playing position
    m_currentSample = 0;

    // initialize the base class
    initialize(buffer.getChannelCount(), buffer.getSampleRate());
}

void
AudioStream::computePeakAmp(int sampleCounts)
{
    float meanAmpTotal = 0.f;
    for (int i = 0; i < sampleCounts; i++) {
        if (i < m_ivecsamples.size() && m_currentSample < m_ivecsamples.size() &&
            abs(m_ivecsamples[m_currentSample] > meanAmpTotal)) {
            m_fpeakAmp = m_ivecsamples[m_currentSample];
        }
    }
}

void
AudioStream::stop()
{
    isPlaying = false;
}
