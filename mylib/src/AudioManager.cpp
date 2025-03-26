#include "AudioManager.h"
#include "PathManager.h"
#include <iostream>

SoundManager::SoundManager()
    : m_initialized(false), m_currentMusic("")
{
}

SoundManager::~SoundManager()
{
    cleanup();
}

void SoundManager::initialize()
{
    if (!m_initialized)
    {
        m_initialized = true;
        std::cout << "SoundManager initialized" << std::endl;
    }
}

void SoundManager::cleanup()
{
    stopMusic();

    for (auto& sound : m_sounds)
    {
        sound.second->stop();
    }

    m_sounds.clear();
    m_soundBuffers.clear();
    m_music.clear();

    m_initialized = false;
    std::cout << "SoundManager cleaned up" << std::endl;
}

bool SoundManager::loadSound(const std::string& name, const std::string& filename)
{
    if (hasSound(name))
        return true;

    auto soundBuffer = std::make_unique<sf::SoundBuffer>();
    std::string fullPath = PathManager::getResourcePath(filename);

    if (!soundBuffer->loadFromFile(fullPath))
    {
        std::cerr << "Failed to load sound: " << fullPath << std::endl;
        return false;
    }

    m_soundBuffers[name] = std::move(soundBuffer);

    auto sound = std::make_unique<sf::Sound>();
    sound->setBuffer(*m_soundBuffers[name]);
    m_sounds[name] = std::move(sound);

    std::cout << "Loaded sound: " << name << " from " << fullPath << std::endl;
    return true;
}

bool SoundManager::loadMusic(const std::string& name, const std::string& filename)
{
    if (hasMusic(name))
        return true;

    auto music = std::make_unique<sf::Music>();
    std::string fullPath = PathManager::getResourcePath(filename);

    if (!music->openFromFile(fullPath))
    {
        std::cerr << "Failed to load music: " << fullPath << std::endl;
        return false;
    }

    m_music[name] = std::move(music);
    std::cout << "Loaded music: " << name << " from " << fullPath << std::endl;
    return true;
}

void SoundManager::playSound(const std::string& name, float volume, float pitch)
{
    if (!hasSound(name))
    {
        std::cerr << "Sound not found: " << name << std::endl;
        return;
    }

    if (m_sounds[name]->getStatus() == sf::Sound::Playing)
    {
        auto newSound = std::make_unique<sf::Sound>();
        newSound->setBuffer(*m_soundBuffers[name]);
        newSound->setVolume(volume);
        newSound->setPitch(pitch);
        newSound->play();

        std::string tempName = name + "_temp_" + std::to_string(rand());
        m_sounds[tempName] = std::move(newSound);

        for (auto iterator = m_sounds.begin(); iterator != m_sounds.end();)
        {
            if (iterator->first.find("_temp_") != std::string::npos &&
                iterator->second->getStatus() == sf::Sound::Stopped)
                iterator = m_sounds.erase(iterator);
            else
                ++iterator;
        }
    }
    else
    {
        m_sounds[name]->setVolume(volume);
        m_sounds[name]->setPitch(pitch);
        m_sounds[name]->play();
    }
}

void SoundManager::stopSound(const std::string& name)
{
    if (!hasSound(name))
    {
        std::cerr << "Sound not found: " << name << std::endl;
        return;
    }

    m_sounds[name]->stop();
}

void SoundManager::playMusic(const std::string& name, float volume)
{
    if (!hasMusic(name))
    {
        std::cerr << "Music not found: " << name << std::endl;
        return;
    }

    if (m_currentMusic != name && !m_currentMusic.empty())
        stopMusic();

    m_music[name]->setVolume(volume);
    m_music[name]->play();
    m_currentMusic = name;
}

void SoundManager::pauseMusic()
{
    if (!m_currentMusic.empty() && hasMusic(m_currentMusic))
        m_music[m_currentMusic]->pause();
}

void SoundManager::resumeMusic()
{
    if (!m_currentMusic.empty() && hasMusic(m_currentMusic))
        m_music[m_currentMusic]->play();
}

void SoundManager::stopMusic()
{
    if (!m_currentMusic.empty() && hasMusic(m_currentMusic))
        m_music[m_currentMusic]->stop();
}

void SoundManager::setMusicVolume(float volume)
{
    if (!m_currentMusic.empty() && hasMusic(m_currentMusic))
        m_music[m_currentMusic]->setVolume(volume);
}

void SoundManager::setMusicLoop(bool loop)
{
    if (!m_currentMusic.empty() && hasMusic(m_currentMusic))
        m_music[m_currentMusic]->setLoop(loop);
}

bool SoundManager::isMusicPlaying() const
{
    if (!m_currentMusic.empty() && hasMusic(m_currentMusic))
        return m_music.at(m_currentMusic)->getStatus() == sf::Music::Playing;

    return false;
}

void SoundManager::removeSound(const std::string& name)
{
    if (hasSound(name))
    {
        m_sounds[name]->stop();
        m_sounds.erase(name);
        m_soundBuffers.erase(name);
    }
}

void SoundManager::removeMusic(const std::string& name)
{
    if (hasMusic(name))
    {
        if (m_currentMusic == name)
        {
            m_music[name]->stop();
            m_currentMusic = "";
        }
        m_music.erase(name);
    }
}

bool SoundManager::hasSound(const std::string& name) const
{
    return m_soundBuffers.find(name) != m_soundBuffers.end();
}

bool SoundManager::hasMusic(const std::string& name) const
{
    return m_music.find(name) != m_music.end();
}

void SoundManager::loadAllGameSounds()
{
    if (!m_initialized)
        initialize();

    // Load sound effects
    loadSound("hero_attack", "sounds\\hero_attack.wav");
    loadSound("arrow_shoot", "sounds\\arrow_shoot.wav");
    loadSound("hit", "sounds\\blood_sound.wav");
    loadSound("death", "sounds\\npc_death.wav");

    // Load music
    loadMusic("game_music", "music\\game_music.wav");
    loadMusic("game_on", "music\\game_on.wav");

    std::cout << "All game sounds loaded successfully\n";
}