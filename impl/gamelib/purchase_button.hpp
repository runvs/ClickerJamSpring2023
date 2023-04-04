#ifndef CLICKERJAMSPRING2023_PURCHASE_BUTTON_HPP
#define CLICKERJAMSPRING2023_PURCHASE_BUTTON_HPP

#include <animation.hpp>
#include <arbitrary_precision_int/arbitrary_precision_int.hpp>
#include <bank_interface.hpp>
#include <button.hpp>
#include <game_object.hpp>
#include <purchase_info.hpp>
#include <text.hpp>
#include <memory>

class PurchaseButton : public ::jt::GameObject {
public:
    PurchaseButton(BankInterface& finances, PurchaseInfo const& info);

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    void updateText();

    std::shared_ptr<jt::Button> m_button;
    std::shared_ptr<jt::Text> m_buttonText;
    std::shared_ptr<jt::Animation> m_buttonAnimation;

    api::API m_cost;
    BankInterface& m_bank;

    PurchaseInfo m_purchaseInfo;

    bool m_canPurchase { false };
    bool m_hasBeenShown { false };
};

#endif // CLICKERJAMSPRING2023_PURCHASE_BUTTON_HPP
