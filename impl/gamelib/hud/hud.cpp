#include "hud.hpp"
#include "game_properties.hpp"
#include <color/color.hpp>
#include <drawable_helpers.hpp>
#include <game_interface.hpp>
#include <hud/score_display.hpp>

std::shared_ptr<ObserverInterface<api::API>> Hud::getDepthScore() const { return m_depthDisplay; }
std::shared_ptr<ObserverInterface<api::API>> Hud::getMoneyScore() const { return m_moneyDisplay; }
std::shared_ptr<ObserverInterface<api::API>> Hud::getMoneyPerSecond() const
{
    return m_moneyPerSecondDisplay;
}

void Hud::doCreate()
{
    m_depthText = std::make_shared<jt::Text>();
    m_depthText = jt::dh::createText(renderTarget(), "", 16, jt::Color { 192, 194, 201 });
    m_depthText->setTextAlign(jt::Text::TextAlign::LEFT);
    m_depthText->setPosition({ 10, 2 });
    m_depthDisplay = std::make_shared<DepthDisplay>(m_depthText, "Depth: -");

    m_moneyText = std::make_shared<jt::Text>();
    m_moneyText = jt::dh::createText(renderTarget(), "", 16, jt::Color { 255, 215, 0 });
    m_moneyText->setTextAlign(jt::Text::TextAlign::LEFT);
    m_moneyText->setPosition({ 10, 18 });
    m_moneyDisplay = std::make_shared<MoneyDisplay>(m_moneyText, "Coins: $");

    m_moneyPerSecondText = std::make_shared<jt::Text>();
    m_moneyPerSecondText = jt::dh::createText(renderTarget(), "", 16, jt::Color { 255, 215, 0 });
    m_moneyPerSecondText->setTextAlign(jt::Text::TextAlign::LEFT);
    m_moneyPerSecondText->setPosition({ 10, 34 });
    m_moneyPerSecondDisplay = std::make_shared<MoneyDisplay>(m_moneyPerSecondText, "CPS: $/s");
}

void Hud::doUpdate(float const elapsed)
{
    m_depthText->update(elapsed);
    m_moneyText->update(elapsed);
    m_moneyPerSecondText->update(elapsed);
}

void Hud::doDraw() const
{
    m_depthText->draw(renderTarget());
    m_moneyText->draw(renderTarget());
    m_moneyPerSecondText->draw(renderTarget());
}
