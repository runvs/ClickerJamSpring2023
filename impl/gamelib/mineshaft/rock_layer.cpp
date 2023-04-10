
#include "rock_layer.hpp"
#include "input/mouse/mouse_defines.hpp"
#include "shape.hpp"
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <random/random.hpp>

RockLayer::RockLayer(int hardness, jt::Color const& color, float initial_layer_offset)
    : m_hardness { hardness }
    , m_color { color }
    , m_progress { 0 }
    , m_layer_offset { initial_layer_offset }
{
}

void RockLayer::progressAmount(int progress)
{
    if (isMined()) {
        return;
    };
    flash();
    m_progress += progress;
    getGame()->logger().debug("active layer progress is now: " + std::to_string(m_progress) + "/"
        + std::to_string(m_hardness));
}

bool RockLayer::isMined() const { return m_progress > m_hardness; }

void RockLayer::ascend()
{
    m_layer_offset -= 1;
    m_shape_middle->setPosition(
        { m_shape_middle->getPosition().x, GP::HudMineShaftLayerSize().y * m_layer_offset });
}

void RockLayer::doCreate()
{
    float const y_offset = GP::HudMineShaftLayerSize().y * m_layer_offset;
    float const x_offset_left = static_cast<float>(jt::Random::getInt(3, 9));
    float const widthModifier = static_cast<float>(jt::Random::getInt(3, 9));

    m_rocks = std::make_shared<jt::Sprite>("assets/rocks.png", textureManager());
    m_rocks->setColor(m_color);

    m_shape_middle = std::make_shared<jt::Shape>();
    m_shape_middle->makeRect({ GP::HudMineShaftLayerSize().x - widthModifier - x_offset_left - 8.0f,
                                 GP::HudMineShaftLayerSize().y },
        textureManager());
    m_shape_middle->setPosition({ GP::HudMineShaftOffset().x + x_offset_left, y_offset });
    m_shape_middle->setColor(jt::colors::Black);

    getGame()->logger().info("new layer with hardness " + std::to_string(m_hardness) + " created");
}

void RockLayer::doUpdate(const float elapsed)
{
    m_rocks->update(elapsed);
    m_shape_middle->update(elapsed);
}

void RockLayer::doDraw() const
{
    float const y_offset = GP::HudMineShaftLayerSize().y * m_layer_offset;
    for (auto i = 0; i != 8; ++i) {
        m_rocks->setPosition(
            jt::Vector2f { GP::HudMineShaftOffset().x + static_cast<float>(i * 16), y_offset });
        m_rocks->update(0.0f);
        m_rocks->draw(renderTarget());
    }

    if (isMined()) {
        m_shape_middle->draw(renderTarget());
    }
}

void RockLayer::flash() { m_shape_middle->flash(0.2f, jt::Color { 255, 255, 255, 100 }); }
jt::Rectf RockLayer::getArea() const { return m_shape_middle->getGlobalBounds(); }
