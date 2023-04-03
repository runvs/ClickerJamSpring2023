
#include "purchased_object.hpp"
#include "bank_interface.hpp"
#include "game_properties.hpp"
PurchasedObject::PurchasedObject(BankInterface& bank, PurchaseInfo const& info)
    : m_bank { bank }
    , m_info { info }
{
    m_numberOfObjects = 0;
}
void PurchasedObject::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();
    m_animation->loadFromJson(m_info.animationFile, textureManager());
    m_animation->play(m_info.animationNamePurchased);
}
void PurchasedObject::doUpdate(float const elapsed)
{
    m_animation->update(elapsed);
    for (auto& t : m_timers) {
        t += elapsed;
        if (t >= m_info.timerMax) {
            t -= m_info.timerMax;
            m_bank.receiveMoney(api::from_uint64(1u));
        }
    }
}

void PurchasedObject::doDraw() const
{

    for (auto i = 0; i != m_numberOfObjects; ++i) {
        int x = i % m_info.objectsPerLine;
        int y = i / m_info.objectsPerLine;
        float xOffset = 0.0f;
        if (y % 2 == 1) {
            xOffset = 5.0f;
        }
        m_animation->setPosition(jt::Vector2f { xOffset - 5, 40 + 60.0f * m_info.index }
            + jt::Vector2f { x * 10.0f, y * 8.0f });
        m_animation->update(0.0f);
        m_animation->draw(renderTarget());
    }
}

void PurchasedObject::buyOne()
{
    m_numberOfObjects++;
    m_timers.push_back(0.0f);
}

api::API PurchasedObject::getInputPerMinute() const
{
    return m_info.income * api::from_uint64(m_numberOfObjects) * api::from_uint64(1000u)
        / api::from_uint64(static_cast<std::uint64_t>(m_info.timerMax) * 1000);
}
