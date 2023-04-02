#ifndef CLICKERJAMSPRING2023_PURCHASE_BUTTON_HPP
#define CLICKERJAMSPRING2023_PURCHASE_BUTTON_HPP

#include <animation.hpp>
#include <button.hpp>
#include <game_object.hpp>
#include <text.hpp>
#include <memory>

class PurchaseButton : public ::jt::GameObject {
public:
private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    std::shared_ptr<jt::Button> m_button;
    std::shared_ptr<jt::Text> m_buttonText;
    std::shared_ptr<jt::Animation> m_buttonAnimation;
};

#endif // CLICKERJAMSPRING2023_PURCHASE_BUTTON_HPP
