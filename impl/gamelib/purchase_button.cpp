#include "purchase_button.hpp"
#include "drawable_helpers.hpp"
#include "game_properties.hpp"
#include <game_interface.hpp>

PurchaseButton::PurchaseButton(BankInterface& finances, PurchaseInfo const& info)
    : m_bank { finances }
    , m_purchaseInfo { info }
{
}

void PurchaseButton::doCreate()
{
    m_cost = m_purchaseInfo.initialCost;

    m_button = std::make_shared<jt::Button>(GP::HudButtonSize(), textureManager());

    m_buttonText = jt::dh::createText(renderTarget(), "", 16);
    updateText();
    m_buttonText->setTextAlign(jt::Text::TextAlign::LEFT);
    m_button->setDrawable(m_buttonText);
    m_button->setPosition(GP::HudMenuOffset() + GP::HudMenuMargin()
        + m_purchaseInfo.index
            * jt::Vector2f { 0.0f, GP::HudButtonSize().y + GP::HudMenuMargin().y });

    m_button->setGameInstance(getGame());
    m_button->create();
    m_button->addCallback([this]() {
        getGame()->logger().info(
            "Button '" + m_purchaseInfo.name + "' pressed", { "PurchaseButton" });
        m_bank.spendMoney(m_cost);
        m_purchaseInfo.purchaseCallback(m_cost);
        // TODO move cost increase into GP
        m_cost = m_cost * api::from_uint64(105) / api::from_uint64(100) + api::from_uint64(1);
        updateText();
    });

    m_buttonAnimation = std::make_shared<jt::Animation>();
    m_buttonAnimation->loadFromJson(m_purchaseInfo.animation_file, textureManager());
    m_buttonAnimation->play(m_purchaseInfo.animation_name);
}

void PurchaseButton::doUpdate(float const elapsed)
{
    m_button->update(elapsed);
    m_buttonText->update(elapsed);

    m_buttonAnimation->setOffset({ -8, -14 });
    m_buttonAnimation->setPosition(m_buttonText->getPosition());
    m_buttonAnimation->update(elapsed);

    m_button->setActive(m_bank.canAffordAmount(m_cost));
}

void PurchaseButton::doDraw() const
{
    m_button->draw();
    m_buttonText->draw(renderTarget());
    m_buttonAnimation->draw(renderTarget());
}
void PurchaseButton::updateText()
{
    std::string const text = "    " + m_purchaseInfo.name + " " + m_cost.to_exp_string();
    m_buttonText->setText(text);
}