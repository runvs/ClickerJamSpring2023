#include "purchased_object.hpp"
#include <bank_interface.hpp>
#include <drawable_helpers.hpp>
#include <ease/expo.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <math_helper.hpp>
#include <random/random.hpp>
#include <tweens/tween_alpha.hpp>
#include <tweens/tween_scale.hpp>
#include <imgui.h>

PurchasedObject::PurchasedObject(BankInterface& bank, PurchaseInfo const& info,
    std::function<void(std::shared_ptr<jt::TweenInterface>)> const& addTweenCallback)
    : m_bank { bank }
    , m_info { info }
    , m_addTweenCallback { addTweenCallback }
{
    m_timers.resize(m_numberOfObjects);
}

void PurchasedObject::doCreate()
{
    m_baseOffset = jt::Vector2f { -8, 22.0f + 54.0f * m_info.index };

    m_text = jt::dh::createText(renderTarget(), "", 16);
    m_text->setTextAlign(jt::Text::TextAlign::RIGHT);
    m_text->setPosition(m_baseOffset + jt::Vector2f { GP::GetScreenSize().x / 3.0f + 4.0f, 48.0f });
}

void PurchasedObject::doUpdate(float const elapsed)
{
    m_animRestartTimer += elapsed;
    bool restart = false;
    if (m_animRestartTimer > m_animRestartTimerMax) {
        restart = true;
        m_animRestartTimer -= m_animRestartTimerMax;
    }
    for (auto& a : m_animations) {
        a->update(elapsed * m_miningTimerSpeedFactor);
        if (restart) {
            a->play(m_info.animationNamePurchased, 0, true);
        }
    }
    for (auto& t : m_timers) {
        t += elapsed;
        if (t >= m_info.timerMax) {
            t -= m_info.timerMax;
            m_bank.receiveMoney(m_info.income);
        }
    }

    updateAutomaticMining(elapsed);
    m_text->update(elapsed);
}
void PurchasedObject::updateAutomaticMining(float const elapsed)
{
    if (m_numberOfObjects != 0) {
        m_progressMiningTimer += elapsed * m_miningTimerSpeedFactor;
        if (m_progressMiningTimer >= m_info.progressMiningTimerMax) {
            m_progressMiningTimer -= m_info.progressMiningTimerMax;
            if (m_progressMiningCallback) {
                m_progressMiningCallback(m_info.progressMiningValue * m_numberOfObjects);
            }
        }
    }
}

void PurchasedObject::doDraw() const
{
    for (auto i = 0; i != m_numberOfObjects; ++i) { }
    for (auto const& a : m_animations) {
        a->draw(renderTarget());
    }

    m_text->draw(renderTarget());
    drawTooltip();
}

void PurchasedObject::drawTooltip() const
{
    auto const mousePos = getGame()->input().mouse()->getMousePositionScreen();
    if (jt::MathHelper::checkIsIn(m_rect, mousePos)) {
        ImGui::SetNextWindowPos(
            ImVec2 { mousePos.x * GP::GetZoom() + 10, mousePos.y * GP::GetZoom() });
        ImGui::SetNextWindowSize(ImVec2 { GP::HudTooltipWidth(), 116 });
        ImGui::Begin(
            m_info.name.c_str(), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Amount: %i", m_numberOfObjects);
        ImGui::Text("Income: %s$/s", m_incomePerSecond.to_exp_string().c_str());
        ImGui::Text(
            "Mine Timer: %.1f / %.1f", m_progressMiningTimer, m_info.progressMiningTimerMax);
        ImGui::Text("Time Factor: %.1f (+%.1f/full line)", m_miningTimerSpeedFactor,
            GP::PurchasedSpeedIncreasePerLine());
        ImGui::Text("Mine Strength: %s",
            std::to_string(m_info.progressMiningValue * m_numberOfObjects).c_str());
        ImGui::End();
    }
}

void PurchasedObject::buyOne()
{
    for (auto& a : m_animations) {
        a->flash(0.15f, jt::Color { 255, 255, 255, 150 });
    }
    addNewAnimation();

    m_numberOfObjects++;

    m_timers.push_back(jt::Random::getFloat(0, m_info.timerMax));
    auto const maxObjects = GP::PurchasedMaxNumberOfLines() * GP::PurchasedNumberOfObjectsPerLine();
    if (m_numberOfObjects > maxObjects) {
        m_text->setText("+" + std::to_string(m_numberOfObjects - maxObjects));
    }
    bool multipleLines = m_numberOfObjects > GP::PurchasedNumberOfObjectsPerLine();
    float const width
        = multipleLines ? GP::PurchasedNumberOfObjectsPerLine() * 10.0f : m_numberOfObjects * 10.0f;
    float const height = (m_numberOfObjects / GP::PurchasedNumberOfObjectsPerLine() + 1) * 8.0f;

    m_rect = jt::Rectf { m_baseOffset.x + 7, m_baseOffset.y + 17, width + 5, height + 7 };
    m_incomePerSecond = m_info.income * api::from_uint64(m_numberOfObjects)
        * api::from_uint64(1000u)
        / api::from_uint64(static_cast<std::uint64_t>(m_info.timerMax) * 1000);

    m_miningTimerSpeedFactor = 1.0f
        + GP::PurchasedSpeedIncreasePerLine()
            * (m_numberOfObjects / GP::PurchasedNumberOfObjectsPerLine());
}
void PurchasedObject::addNewAnimation()
{
    auto const maxObjects = GP::PurchasedMaxNumberOfLines() * GP::PurchasedNumberOfObjectsPerLine();
    if (m_numberOfObjects < maxObjects) {
        auto newAnimation = std::make_shared<jt::Animation>();
        newAnimation = std::make_shared<jt::Animation>();
        newAnimation->loadFromJson(m_info.animationFile, textureManager());
        newAnimation->play(m_info.animationNamePurchased);

        int const x = m_numberOfObjects % m_info.objectsPerLine;
        int const y = m_numberOfObjects / m_info.objectsPerLine;
        float xOffset = 0.0f;
        if (y % 2 == 1) {
            xOffset = 5.0f;
        }
        newAnimation->setPosition(
            m_baseOffset + jt::Vector2f { xOffset, 0.0f } + jt::Vector2f { x * 10.0f, y * 8.0f });
        newAnimation->update(0.0f);
        newAnimation->draw(renderTarget());
        m_animations.push_back(newAnimation);

        if (m_animRestartTimerMax == 0.0f) {
            m_animRestartTimerMax = newAnimation->getCurrentAnimTotalTime();
        }
        auto const tweenTime = 0.35f;
        auto const tweenScale = jt::TweenScale::create(
            newAnimation, tweenTime, jt::Vector2f { 2.0f, 2.0f }, jt::Vector2f { 1.0f, 1.0f });
        tweenScale->setAgePercentConversion(
            [](float in) { return jt::ease::expo::easeOut(in, 0.0f, 1.0f, 1.0f); });
        m_addTweenCallback(tweenScale);

        auto tweenAlpha = jt::TweenAlpha::create(newAnimation, tweenTime, 0u, 255u);
        tweenAlpha->addCompleteCallback(
            [newAnimation]() { newAnimation->flash(0.2f, jt::colors::White); });
        m_addTweenCallback(tweenAlpha);
    } else {
        auto const tweenTime = 0.15f;
        auto const tweenScale = jt::TweenScale::create(
            m_text, tweenTime, jt::Vector2f { 1.5f, 1.5f }, jt::Vector2f { 1.0f, 1.0f });
        m_addTweenCallback(tweenScale);
    }
}

api::API PurchasedObject::getInputPerSecond() const { return m_incomePerSecond; }
int PurchasedObject::getNumberOfBoughtObjects() const { return m_numberOfObjects; }

void PurchasedObject::clean()
{
    m_numberOfObjects = 0;
    m_timers.clear();
    m_animations.clear();
}
void PurchasedObject::setProgressMiningCallback(std::function<void(std::uint64_t)> const& callback)
{
    m_progressMiningCallback = callback;
}
