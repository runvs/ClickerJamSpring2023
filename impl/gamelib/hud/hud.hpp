#ifndef GAME_HUD_HPP
#define GAME_HUD_HPP

#include "arbitrary_precision_int/arbitrary_precision_int.hpp"
#include "depth_display.hpp"
#include "money_display.hpp"
#include <game_object.hpp>
#include <hud/observer_interface.hpp>
#include <sprite.hpp>
#include <text.hpp>

class ScoreDisplay;

class Hud : public jt::GameObject {
public:
    std::shared_ptr<ObserverInterface<api::API>> getDepthScore() const;
    std::shared_ptr<ObserverInterface<api::API const&>> getMoneyScore() const;
    std::shared_ptr<ObserverInterface<api::API const&>> getMoneyPerSecond() const;

private:
    std::shared_ptr<DepthDisplay> m_depthDisplay;
    std::shared_ptr<MoneyDisplay> m_moneyDisplay;
    std::shared_ptr<MoneyDisplay> m_moneyPerSecondDisplay;

    jt::Text::Sptr m_depthText;
    jt::Text::Sptr m_moneyText;
    jt::Text::Sptr m_moneyPerSecondText;

    void doCreate() override;

    void doUpdate(float const elapsed) override;

    void doDraw() const override;
};

#endif
