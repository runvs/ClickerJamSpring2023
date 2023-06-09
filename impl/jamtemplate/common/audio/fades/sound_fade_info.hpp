#ifndef MYAWESOMEGAME_SOUND_FADE_INFO_HPP
#define MYAWESOMEGAME_SOUND_FADE_INFO_HPP

#include <audio/sound/sound_interface.hpp>
#include <memory>

namespace jt {
class SoundFadeManager;

class SoundFadeInfo {

public:
    SoundFadeInfo(std::weak_ptr<jt::SoundInterface> const& sound, float duration, float startVolume,
        float endVolume);

    void update(float elapsed);

    bool hasValidSound() const;
    bool isAlive() const;

private:
    std::weak_ptr<jt::SoundInterface> m_sound;
    float m_startVolume { 0.0f };
    float m_endVolume { 0.0f };
    float m_duration { 0.0f };
    float m_age { 0.0f };
};
} // namespace jt

#endif // MYAWESOMEGAME_SOUND_FADE_INFO_HPP
