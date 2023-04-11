
#include "rock_layer.hpp"
#include <ease/expo.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <input/mouse/mouse_defines.hpp>
#include <random/random.hpp>
#include <shape.hpp>
#include <tweens/tween_scale.hpp>

RockLayer::RockLayer(std::uint64_t hardness, jt::Color const& color, float initialLayerOffset,
    std::function<void(std::shared_ptr<jt::TweenInterface>)> const& addTweenCallback, bool isSky)
    : m_hardness { hardness }
    , m_color { color }
    , m_progress { 0ULL }
    , m_layer_offset { initialLayerOffset }
    , m_isSky { isSky }
    , m_addTweenCallback { addTweenCallback }
{
}

void RockLayer::doCreate()
{
    float const y_offset = GP::HudMineShaftLayerSize().y * m_layer_offset;
    float const x_offset_left = static_cast<float>(jt::Random::getInt(10, 16));
    float const widthModifier = static_cast<float>(jt::Random::getInt(3, 9));

    m_spriteRocks = std::make_shared<jt::Sprite>("assets/rocks.png", textureManager());
    m_spriteRocks->setColor(m_color);

    m_shapeMinedOverlay = std::make_shared<jt::Shape>();
    jt::Vector2f const minedOverlaySize { GP::HudMineShaftLayerSize().x - widthModifier
            - x_offset_left - 8.0f,
        GP::HudMineShaftLayerSize().y };
    m_shapeMinedOverlay->makeRect(minedOverlaySize, textureManager());
    m_shapeMinedOverlay->setPosition({ GP::HudMineShaftOffset().x + x_offset_left, y_offset });
    m_shapeMinedOverlay->setColor(jt::Color { 0u, 0u, 0u, 210u });
    m_shapeMinedOverlay->setOrigin(jt::Vector2f { minedOverlaySize.x / 2.0f, 0.0f });
    m_shapeMinedOverlay->setOffset(jt::Vector2f { minedOverlaySize.x / 2.0f, 0.0f });

    m_shapeBackground = std::make_shared<jt::Shape>();
    m_shapeBackground->makeRect(GP::HudMineShaftLayerSize(), textureManager());
    m_shapeBackground->setPosition({ GP::HudMineShaftOffset().x, y_offset });
    m_shapeBackground->setColor(m_color);

    getGame()->logger().info("new layer with hardness " + std::to_string(m_hardness) + " created");
}

void RockLayer::doUpdate(const float elapsed)
{
    m_spriteRocks->update(elapsed);
    m_shapeMinedOverlay->update(elapsed);
    m_shapeBackground->update(elapsed);
}

void RockLayer::doDraw() const
{
    if (!m_isSky) {
        float const y_offset = GP::HudMineShaftLayerSize().y * m_layer_offset;
        for (auto i = 0; i != 8; ++i) {
            m_spriteRocks->setPosition(
                jt::Vector2f { GP::HudMineShaftOffset().x + static_cast<float>(i * 16), y_offset });
            m_spriteRocks->update(0.0f);
            m_spriteRocks->draw(renderTarget());
        }
    } else {
        m_shapeBackground->draw(renderTarget());
    }

    if (isMined()) {
        m_shapeMinedOverlay->draw(renderTarget());
    }
}

void RockLayer::progressAmount(std::uint64_t progress)
{
    if (isMined()) {
        return;
    };
    flash();
    m_progress += progress;
    if (isMined()) {
        auto tweenScale = jt::TweenScale::create(
            m_shapeMinedOverlay, 0.5f, jt::Vector2f { 1.0f, 0.0f }, jt::Vector2f { 1.0f, 1.0f });
        tweenScale->setAgePercentConversion(
            [](float in) { return jt::ease::expo::easeOut(in, 0.0f, 1.0f, 1.0f); });
        m_addTweenCallback(tweenScale);
    }
    getGame()->logger().debug("active layer progress is now: " + std::to_string(m_progress) + "/"
        + std::to_string(m_hardness));
}

bool RockLayer::isMined() const { return m_progress >= m_hardness; }

void RockLayer::ascend()
{
    m_layer_offset -= 1;
    m_shapeMinedOverlay->setPosition(
        { m_shapeMinedOverlay->getPosition().x, GP::HudMineShaftLayerSize().y * m_layer_offset });
    m_shapeBackground->setPosition(
        { GP::HudMineShaftOffset().x, GP::HudMineShaftLayerSize().y * m_layer_offset });
}

void RockLayer::flash() { m_spriteRocks->flash(0.2f, jt::Color { 255, 255, 255, 100 }); }
jt::Rectf RockLayer::getArea() const { return m_shapeMinedOverlay->getGlobalBounds(); }
std::uint64_t RockLayer::getHardness() const { return m_hardness; }
std::uint64_t RockLayer::getProgress() const { return m_progress; }
