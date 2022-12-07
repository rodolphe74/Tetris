#pragma once

#include <SFML/Audio.hpp>
#include <vector>

// static std::vector<sf::Int16> m_ivecsamples;

// custom audio stream that plays a loaded buffer
class AudioStream : public sf::SoundStream
{
  private:
    // number of samples to stream every time the function is called;
    // in a more robust implementation, it should be a fixed
    // amount of time rather than an arbitrary number of samples
    const int SAMPLES_TO_STREAM = 2048;
    
    static std::vector<sf::Int16> m_ivecsamples;
    virtual bool onGetData(Chunk& data);
    virtual void onSeek(sf::Time timeOffset);

  public:
    bool isPlaying = true;
    std::size_t m_currentSample = 0;
    float m_fpeakAmp = 0.0f;
    void load(sf::SoundBuffer& buffer);
    void computePeakAmp(int samplesCount);
    void stop();
};