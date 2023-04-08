#include "purchase_button.hpp"
#include <drawable_helpers.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <math_helper.hpp>
#include "imgui.h"

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
        + static_cast<float>(m_purchaseInfo.index)
            * jt::Vector2f {
                0.0f, static_cast<float>(GP::HudButtonSize().y) + GP::HudMenuMargin().y });

    m_button->setGameInstance(getGame());
    m_button->create();
    m_button->addCallback([this]() {
        getGame()->logger().info(
            "Button '" + m_purchaseInfo.name + "' pressed", { "PurchaseButton" });
        m_bank.spendMoney(m_cost);
        m_purchaseInfo.purchaseCallback(m_cost);
        m_cost = m_cost * api::from_uint64(1000 * GP::PurchaseButtonCostIncreasePercent())
                / api::from_uint64(1000)
            + api::from_uint64(1u);
        updateText();

        m_soundGroup->play();
    });
    m_mouseOverRect = jt::Rectf { m_button->getPosition().x, m_button->getPosition().y,
        static_cast<float>(GP::HudButtonSize().x), static_cast<float>(GP::HudButtonSize().y) };

    m_buttonAnimation = std::make_shared<jt::Animation>();
    m_buttonAnimation->loadFromJson(m_purchaseInfo.animationFile, textureManager());
    m_buttonAnimation->play(m_purchaseInfo.animationNameMenu);

    std::vector<std::shared_ptr<jt::SoundInterface>> soundGroupSounds {};
    for (auto i = 0; i != 5; ++i) {
        std::string const fileName = "assets/sfx/pling" + std::to_string(i) + ".wav";
        auto snd = getGame()->audio().addTemporarySound(fileName);
        soundGroupSounds.push_back(snd);
    }
    m_soundGroup = getGame()->audio().addTemporarySoundGroup(soundGroupSounds);
}

void PurchaseButton::doUpdate(float const elapsed)
{
    if (!m_hasBeenShown) {
        api::API const showAt = m_cost
            * api::from_uint64(
                static_cast<std::uint64_t>(100 * GP::PurchaseButtonRevealAtPercentage()))
            / api::from_uint64(100);

        if (api::compare(m_bank.getCurrentMoney(), showAt) >= 0) {
            m_hasBeenShown = true;
            m_button->getBackground()->flash(0.4f, jt::colors::Green);
            // TODO play sound
        }
    }
    m_button->update(elapsed);
    m_buttonText->update(elapsed);

    m_buttonAnimation->setOffset({ -8, -14 });
    m_buttonAnimation->setPosition(m_buttonText->getPosition());
    m_buttonAnimation->update(elapsed);

    m_canPurchase = m_bank.canAffordAmount(m_cost);
    m_button->setActive(m_canPurchase);
    if (m_canPurchase) {
        m_buttonAnimation->setColor(jt::colors::White);
        m_buttonText->setColor(jt::colors::White);
    } else {
        m_buttonAnimation->setColor(jt::colors::Gray);
        m_buttonText->setColor(jt::colors::Gray);
    }
}

void PurchaseButton::doDraw() const
{
    if (m_hasBeenShown) {
        m_button->draw();
        m_buttonText->draw(renderTarget());
        m_buttonAnimation->draw(renderTarget());
    }

    if (wasVisible()) {
        auto mousePos = getGame()->input().mouse()->getMousePositionScreen();
        if (jt::MathHelper::checkIsIn(m_mouseOverRect, mousePos)) {
            ImGui::SetNextWindowPos(
                ImVec2 { mousePos.x * GP::GetZoom() + 10, mousePos.y * GP::GetZoom() });
            ImGui::SetNextWindowSize(ImVec2 { 150, 65 });
            std::string windowName = "Purchase " + m_purchaseInfo.name;
            ImGui::Begin(
                windowName.c_str(), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
            ImGui::Text("Cost: %s$", m_cost.to_exp_string().c_str());
            ImGui::Text("Income: %s$/s", m_purchaseInfo.income.to_exp_string().c_str());
            ImGui::End();
        }
    }
}
void PurchaseButton::updateText()
{
    std::string const text = "   " + m_purchaseInfo.name + " " + m_cost.to_exp_string();
    m_buttonText->setText(text);
}
api::API PurchaseButton::getPrice() const { return m_cost; }

void PurchaseButton::setPrice(api::API const& price)
{
    m_cost = price;
    updateText();
}
std::string PurchaseButton::getButtonName() const { return m_purchaseInfo.name; }
void PurchaseButton::hide() { m_hasBeenShown = false; }
bool PurchaseButton::wasVisible() const { return m_hasBeenShown; }
void PurchaseButton::setVisible(bool visible) { m_hasBeenShown = visible; }
