#ifndef CLICKERJAMSPRING2023_PURCHASE_BUTTON_HPP
#define CLICKERJAMSPRING2023_PURCHASE_BUTTON_HPP

#include "audio/sound/sound_interface.hpp"
#include <animation.hpp>
#include <arbitrary_precision_int/arbitrary_precision_int.hpp>
#include <bank_interface.hpp>
#include <button.hpp>
#include <game_object.hpp>
#include <purchase_info.hpp>
#include <text.hpp>
#include <tweens/tween_interface.hpp>
#include <functional>
#include <memory>

class PurchaseButton : public ::jt::GameObject {
public:
    PurchaseButton(BankInterface& finances, PurchaseInfo const& info,
        std::function<void(std::shared_ptr<jt::TweenInterface>)> const& addTweenCallback);

    api::API getPrice() const;
    void setPrice(api::API const& price);
    void hide();
    std::string getButtonName() const;
    bool wasVisible() const;
    void setVisible(bool visible);

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    void updateText();

    void purchaseAction();

    BankInterface& m_bank;
    PurchaseInfo m_purchaseInfo;
    std::function<void(std::shared_ptr<jt::TweenInterface>)> m_addTweenCallback;

    std::shared_ptr<jt::Button> m_button;
    std::shared_ptr<jt::Text> m_buttonText;
    std::shared_ptr<jt::Animation> m_buttonAnimation;

    api::API m_cost;

    float m_wiggleTimer = 0.0f;
    float m_wiggleTimerMax = 0.4f;
    bool m_isInWiggle = false;

    bool m_canPurchase { false };
    bool m_hasBeenShown { false };
    std::shared_ptr<jt::SoundInterface> m_soundGroup;

    jt::Rectf m_mouseOverRect;
};

#endif // CLICKERJAMSPRING2023_PURCHASE_BUTTON_HPP
