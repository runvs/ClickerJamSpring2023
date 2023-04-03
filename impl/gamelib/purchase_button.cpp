#include "purchase_button.hpp"
#include "drawable_helpers.hpp"
#include "game_properties.hpp"

PurchaseButton::PurchaseButton(BankInterface& finances)
    : m_finances { finances }
{
}

void PurchaseButton::doCreate()
{
    m_button = std::make_shared<jt::Button>(GP::HudButtonSize(), textureManager());
    m_buttonText = jt::dh::createText(renderTarget(), "    Miner", 16);
    m_buttonText->setTextAlign(jt::Text::TextAlign::LEFT);
    m_button->setDrawable(m_buttonText);
    m_button->setPosition(GP::HudMenuOffset() + GP::HudMenuMargin());

    m_button->setGameInstance(getGame());
    m_button->create();

    m_buttonAnimation = std::make_shared<jt::Animation>();
    m_buttonAnimation->loadFromJson("assets/human/MiniArcherMan.json", textureManager());
    m_buttonAnimation->play("idle");

    m_cost = api::from_uint64(10);
}

void PurchaseButton::doUpdate(float const elapsed)
{
    m_button->update(elapsed);
    m_buttonText->update(elapsed);

    m_buttonAnimation->setOffset({ -8, -14 });
    m_buttonAnimation->setPosition(m_buttonText->getPosition());
    m_buttonAnimation->update(elapsed);

    auto const cmp = api::compare(m_finances.getCurrentMoney(), m_cost);
    m_button->setActive(cmp >= 0);
}

void PurchaseButton::doDraw() const
{
    m_button->draw();
    m_buttonText->draw(renderTarget());
    m_buttonAnimation->draw(renderTarget());
}
