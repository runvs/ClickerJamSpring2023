#include "purchased_objects.hpp"
#include <game_properties.hpp>

PurchasedObjects::PurchasedObjects(BankInterface& bank)
    : m_bank { bank }
{
}

void PurchasedObjects::doCreate()
{
    m_miner = std::make_shared<jt::Animation>();
    m_miner->loadFromJson("assets/human/MiniArcherMan.json", textureManager());
    m_miner->play("idle");
}

void PurchasedObjects::doUpdate(float const elapsed)
{
    m_miner->update(elapsed);

    for (auto& t : m_minerIncomeTimers) {
        t += elapsed;
        if (t >= m_minerIncomeTimerMax) {
            t -= m_minerIncomeTimerMax;
            m_bank.receiveMoney(api::from_uint64(1u));
        }
    }
}

void PurchasedObjects::doDraw() const { drawMiners(); }

void PurchasedObjects::drawMiners() const
{
    for (auto i = 0; i != m_numberOfMiners; ++i) {
        int x = i % GP::PurchasedNumberOfMinersPerRow();
        int y = i / GP::PurchasedNumberOfMinersPerRow();
        float xOffset = 0.0f;
        if (y % 2 == 1) {
            xOffset = 5.0f;
        }
        m_miner->setPosition(
            jt::Vector2f { xOffset - 5, 5 } + jt::Vector2f { x * 10.0f, y * 8.0f });
        m_miner->update(0.0f);
        m_miner->draw(renderTarget());
    }
}

void PurchasedObjects::addMiner()
{
    ++m_numberOfMiners;
    m_minerIncomeTimers.push_back(0.0f);
}
