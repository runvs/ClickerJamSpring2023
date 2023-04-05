
#include "purchased_object.hpp"
#include <bank_interface.hpp>
#include <drawable_helpers.hpp>
#include <game_properties.hpp>
#include <random/random.hpp>

PurchasedObject::PurchasedObject(BankInterface& bank, PurchaseInfo const& info, int numberOfObjects)
    : m_bank { bank }
    , m_info { info }
    , m_numberOfObjects { numberOfObjects }
{
    m_timers.resize(m_numberOfObjects);
}

void PurchasedObject::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();
    m_animation->loadFromJson(m_info.animationFile, textureManager());
    m_animation->play(m_info.animationNamePurchased);

    m_baseOffset = jt::Vector2f { -8, 22.0f + 54.0f * m_info.index };

    m_text = jt::dh::createText(renderTarget(), "", 16);
    m_text->setTextAlign(jt::Text::TextAlign::RIGHT);
    m_text->setPosition(m_baseOffset + jt::Vector2f { GP::GetScreenSize().x / 3.0f + 4.0f, 48.0f });
}

void PurchasedObject::doUpdate(float const elapsed)
{
    m_animation->update(elapsed);
    for (auto& t : m_timers) {
        t += elapsed;
        if (t >= m_info.timerMax) {
            t -= m_info.timerMax;
            m_bank.receiveMoney(m_info.income);
        }
    }
    m_text->update(elapsed);
}

void PurchasedObject::doDraw() const
{
    for (auto i = 0; i != m_numberOfObjects; ++i) {
        int x = i % m_info.objectsPerLine;
        int y = i / m_info.objectsPerLine;
        if (y >= GP::PurchasedMaxNumberOfLines()) {
            break;
        }
        float xOffset = 0.0f;
        if (y % 2 == 1) {
            xOffset = 5.0f;
        }
        m_animation->setPosition(
            m_baseOffset + jt::Vector2f { xOffset, 0.0f } + jt::Vector2f { x * 10.0f, y * 8.0f });
        m_animation->update(0.0f);
        m_animation->draw(renderTarget());
    }
    m_text->draw(renderTarget());
}

void PurchasedObject::buyOne()
{
    m_numberOfObjects++;
    m_timers.push_back(jt::Random::getFloat(0, m_info.timerMax));
    auto const maxObjects = GP::PurchasedMaxNumberOfLines() * GP::PurchasedNumberOfMinersPerLine();
    if (m_numberOfObjects > maxObjects) {
        m_text->setText("+" + std::to_string(m_numberOfObjects - maxObjects));
    }
}

api::API PurchasedObject::getInputPerMinute() const
{
    return m_info.income * api::from_uint64(m_numberOfObjects) * api::from_uint64(1000u)
        / api::from_uint64(static_cast<std::uint64_t>(m_info.timerMax) * 1000);
}
int PurchasedObject::getNumberOfBoughtObjects() const { return m_numberOfObjects; }

void PurchasedObject::clean()
{
    m_numberOfObjects = 0;
    m_timers.clear();
}
