#pragma once

#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>
#include <memory>

class SoundManager
{
public:
    static SoundManager& getInstance()
    {
        static SoundManager instance;
        return instance;
    }

    void initialize();
    void cleanup();

    bool loadSound(const std::string& name, const std::string& filename);
    bool loadMusic(const std::string& name, const std::string& filename);

    void playSound(const std::string& name, float volume = 100.0f, float pitch = 1.0f);
    void stopSound(const std::string& name);

    void playMusic(const std::string& name, float volume = 100.0f);
    void pauseMusic();
    void resumeMusic();
    void stopMusic();
    void setMusicVolume(float volume);
    void setMusicLoop(bool loop);
    bool isMusicPlaying() const;

    void removeSound(const std::string& name);
    void removeMusic(const std::string& name);

    bool hasSound(const std::string& name) const;
    bool hasMusic(const std::string& name) const;

    void loadAllGameSounds();

private:
    SoundManager();
    ~SoundManager();

    std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> m_soundBuffers;
    std::unordered_map<std::string, std::unique_ptr<sf::Sound>> m_sounds;
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> m_music;

    std::string m_currentMusic;
    bool m_initialized;
};