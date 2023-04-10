#include "purchased_object.hpp"
#include <bank_interface.hpp>
#include <drawable_helpers.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <math_helper.hpp>
#include <random/random.hpp>
#include <imgui.h>

PurchasedObject::PurchasedObject(BankInterface& bank, PurchaseInfo const& info, int numberOfObjects)
    : m_bank { bank }
    , m_info { info }
    , m_numberOfObjects { numberOfObjects }
{
    m_timers.resize(m_numberOfObjects);
}

void PurchasedObject::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();
    m_animation->loadFromJson(m_info.animationFile, textureManager());
    m_animation->play(m_info.animationNamePurchased);

    m_baseOffset = jt::Vector2f { -8, 22.0f + 54.0f * m_info.index };

    m_text = jt::dh::createText(renderTarget(), "", 16);
    m_text->setTextAlign(jt::Text::TextAlign::RIGHT);
    m_text->setPosition(m_baseOffset + jt::Vector2f { GP::GetScreenSize().x / 3.0f + 4.0f, 48.0f });
}

void PurchasedObject::doUpdate(float const elapsed)
{
    m_animation->update(elapsed);
    for (auto& t : m_timers) {
        t += elapsed;
        if (t >= m_info.timerMax) {
            t -= m_info.timerMax;
            m_bank.receiveMoney(m_info.income);
        }
    }
    m_text->update(elapsed);
}

void PurchasedObject::doDraw() const
{
    for (auto i = 0; i != m_numberOfObjects; ++i) {
        int x = i % m_info.objectsPerLine;
        int y = i / m_info.objectsPerLine;
        if (y >= GP::PurchasedMaxNumberOfLines()) {
            break;
        }
        float xOffset = 0.0f;
        if (y % 2 == 1) {
            xOffset = 5.0f;
        }
        m_animation->setPosition(
            m_baseOffset + jt::Vector2f { xOffset, 0.0f } + jt::Vector2f { x * 10.0f, y * 8.0f });
        m_animation->update(0.0f);
        m_animation->draw(renderTarget());
    }
    m_text->draw(renderTarget());
    drawTooltip();
}
void PurchasedObject::drawTooltip() const
{
    auto mousePos = getGame()->input().mouse()->getMousePositionScreen();
    if (jt::MathHelper::checkIsIn(m_rect, mousePos)) {
        ImGui::SetNextWindowPos(
            ImVec2 { mousePos.x * GP::GetZoom() + 10, mousePos.y * GP::GetZoom() });
        ImGui::SetNextWindowSize(ImVec2 { 150, 65 });
        ImGui::Begin(
            m_info.name.c_str(), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Amount: %i", m_numberOfObjects);
        ImGui::Text("Income: %s$/s", m_incomePerSecond.to_exp_string().c_str());
        ImGui::End();
    }
}

void PurchasedObject::buyOne()
{
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

    m_animation->flash(0.2f, jt::Color { 255, 255, 255, 150 });
}

api::API PurchasedObject::getInputPerSecond() const { return m_incomePerSecond; }
int PurchasedObject::getNumberOfBoughtObjects() const { return m_numberOfObjects; }

void PurchasedObject::clean()
{
    m_numberOfObjects = 0;
    m_timers.clear();
}
