#include "purchase_button.hpp"
#include <drawable_helpers.hpp>
#include <ease/expo.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <math_helper.hpp>
#include <strutils.hpp>
#include <tweens/tween_position.hpp>
#include <imgui.h>

PurchaseButton::PurchaseButton(BankInterface& finances, PurchaseInfo const& info,
    std::function<void(std::shared_ptr<jt::TweenInterface>)> const& addTweenCallback)
    : m_bank { finances }
    , m_purchaseInfo { info }
    , m_addTweenCallback { addTweenCallback }
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
    m_button->addCallback([this]() { purchaseAction(); });
    m_mouseOverRect = jt::Rectf { m_button->getPosition().x, m_button->getPosition().y,
        static_cast<float>(GP::HudButtonSize().x), static_cast<float>(GP::HudButtonSize().y) };

    m_buttonAnimation = std::make_shared<jt::Animation>();
    m_buttonAnimation->loadFromJson(m_purchaseInfo.animationFile, textureManager());
    m_buttonAnimation->play(m_purchaseInfo.animationNameMenu);
    m_buttonAnimation->setOffset({ -8, -14 });
    m_buttonAnimation->setPosition(m_buttonText->getPosition());

    std::vector<std::shared_ptr<jt::SoundInterface>> soundGroupSounds {};
    for (auto i = 0; i != 5; ++i) {
        std::string const fileName = "assets/sfx/pling" + std::to_string(i) + ".wav";
        for (int j = -3; j != 4; ++j) {
            auto snd = getGame()->audio().addTemporarySound(fileName);
            snd->setPitch(1.0f + static_cast<float>(j) / 50.0f);
            snd->setVolume(0.75f);
            soundGroupSounds.push_back(snd);
        }
    }
    m_soundGroup = getGame()->audio().addTemporarySoundGroup(soundGroupSounds);
}

void PurchaseButton::purchaseAction()
{
    getGame()->logger().info("Button '" + m_purchaseInfo.name + "' pressed", { "PurchaseButton" });
    m_bank.spendMoney(m_cost);
    m_purchaseInfo.purchaseCallback(m_cost);
    m_cost = m_cost * api::from_uint64(1000 * GP::PurchaseButtonCostIncreasePercent())
            / api::from_uint64(1000)
        + api::from_uint64(1u);
    updateText();
    m_button->getBackground()->flash(0.2f, jt::Color { 255, 255, 255, 150 });
    m_soundGroup->play();

    m_wiggleTimer = 0.0f;
    m_isInWiggle = true;
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

            float const tweenTime = 0.35f;
            auto tw1 = jt::TweenPosition::create(m_button->getBackground(), tweenTime,
                m_button->getPosition() + jt::Vector2f { GP::HudMenuSize().x, 0.0f },
                m_button->getPosition());
            tw1->setAgePercentConversion(
                [](float in) { return jt::ease::expo::easeOut(in, 0.0f, 1.0f, 1.0f); });
            m_addTweenCallback(tw1);

            auto tw2 = jt::TweenPosition::create(m_button->getDrawable(), tweenTime,
                m_button->getDrawable()->getPosition() + jt::Vector2f { GP::HudMenuSize().x, 0.0f },
                m_button->getDrawable()->getPosition());
            tw2->setAgePercentConversion(
                [](float in) { return jt::ease::expo::easeOut(in, 0.0f, 1.0f, 1.0f); });
            m_addTweenCallback(tw2);

            auto tw3 = jt::TweenPosition::create(m_buttonAnimation, tweenTime,
                m_buttonAnimation->getPosition() + jt::Vector2f { GP::HudMenuSize().x, 0.0f },
                m_buttonAnimation->getPosition());
            tw3->addCompleteCallback([button = this->m_button]() {
                button->getBackground()->flash(0.35f, jt::colors::Green);
            });
            tw3->setAgePercentConversion(
                [](float in) { return jt::ease::expo::easeOut(in, 0.0f, 1.0f, 1.0f); });
            m_addTweenCallback(tw3);

            m_button->setPosition(
                m_button->getPosition() + jt::Vector2f { GP::HudMenuSize().x, 0.0f });
            m_button->update(0.0f);

            m_buttonAnimation->setPosition(
                m_buttonAnimation->getPosition() + jt::Vector2f { GP::HudMenuSize().x, 0.0f });
            m_buttonAnimation->update(0.0f);
        }
    }
    m_button->update(elapsed);
    m_buttonText->update(elapsed);
    m_buttonAnimation->update(elapsed);

    m_canPurchase = m_bank.canAffordAmount(m_cost);
    m_button->setActive(m_canPurchase);
    if (m_canPurchase) {
        if (getGame()->input().keyboard()->justPressed(m_purchaseInfo.keyCode)) {
            purchaseAction();
        }

        m_buttonAnimation->setColor(jt::colors::White);
        m_buttonText->setColor(jt::colors::White);
    } else {
        m_buttonAnimation->setColor(jt::colors::Gray);
        m_buttonText->setColor(jt::colors::Gray);
    }

    if (m_isInWiggle) {
        m_wiggleTimer += elapsed;

        float const sinPart = sin(m_wiggleTimer * 35.0f);
        float const decayPart = 1.0f - m_wiggleTimer / m_wiggleTimerMax;

        float const value = 1.0f + 0.2f * sinPart * decayPart;
        m_button->getBackground()->setScale({ value, 1.0f });
        if (m_wiggleTimer > m_wiggleTimerMax) {
            m_isInWiggle = false;
            m_button->getBackground()->setScale({ 1.0f, 1.0f });
        }
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
            ImGui::SetNextWindowSize(ImVec2 { 150, 82 });
            std::string windowName = "Purchase " + m_purchaseInfo.name;
            ImGui::Begin(
                windowName.c_str(), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
            ImGui::Text("Cost: %s$", m_cost.to_exp_string().c_str());
            ImGui::Text("Income: %s$/s", m_purchaseInfo.income.to_exp_string().c_str());
            std::string keyCodeText = m_purchaseInfo.keyCode._to_string();
            strutil::replace_all(keyCodeText, "Num", "");
            ImGui::Text("Hotkey: '%s'", keyCodeText.c_str());
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
