
#include "rock_layer.hpp"
#include "input/mouse/mouse_defines.hpp"
#include "shape.hpp"
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <random/random.hpp>

RockLayer::RockLayer(const int hardness, const jt::Color color, const float initial_layer_offset)
    : m_hardness { hardness }
    , m_color { color }
    , m_layer_offset { initial_layer_offset }
{
    m_progress = 0;
}

void RockLayer::progressAmount(int progress)
{
    if (isMined()) {
        return;
    };

    m_progress += progress;
    std::cout << "active layer progress is now: " << m_progress << "/" << m_hardness << std::endl;
}

bool RockLayer::isMined() { return m_progress > m_hardness; }

void RockLayer::ascend()
{
    m_layer_offset -= 1;

    m_shape_left->setPosition(
        { m_shape_left->getPosition().x, GP::HudMineShaftLayerSize().y * m_layer_offset });
    m_shape_right->setPosition(
        { m_shape_right->getPosition().x, GP::HudMineShaftLayerSize().y * m_layer_offset });
    m_shape_middle->setPosition(
        { m_shape_middle->getPosition().x, GP::HudMineShaftLayerSize().y * m_layer_offset });
    if (isMined()) {
        m_shape_middle->setColor({ m_shape_middle->getColor().r, m_shape_middle->getColor().g,
            m_shape_middle->getColor().b, 16 });
    }
}

void RockLayer::doCreate()
{
    float y_offset = GP::HudMineShaftLayerSize().y * m_layer_offset;
    float x_offset_left = static_cast<float>(jt::Random::getInt(3, 9));
    float x_offset_right = static_cast<float>(jt::Random::getInt(3, 9));

    m_shape_left = std::make_shared<jt::Shape>();
    m_shape_left->makeRect({ x_offset_left, GP::HudMineShaftLayerSize().y }, textureManager());
    m_shape_left->setPosition({ GP::HudMineShaftOffset().x, y_offset });
    m_shape_left->setColor(m_color);

    m_shape_right = std::make_shared<jt::Shape>();
    m_shape_right->makeRect({ x_offset_right, GP::HudMineShaftLayerSize().y }, textureManager());
    m_shape_right->setPosition(
        { GP::HudMineShaftOffset().x + GP::HudMineShaftLayerSize().x - x_offset_right, y_offset });
    m_shape_right->setColor(m_color);

    // middle just covers left & right shapes and is set to low alpha when layer is mined
    m_shape_middle = std::make_shared<jt::Shape>();
    m_shape_middle->makeRect(GP::HudMineShaftLayerSize(), textureManager());
    m_shape_middle->setPosition({ GP::HudMineShaftOffset().x, y_offset });
    m_shape_middle->setColor(m_color);

    m_shape_left->update(1.0f);
    m_shape_right->update(1.0f);
    m_shape_middle->update(1.0f);

    std::cout << "new layer with hardness " << m_hardness << " created" << std::endl;
}

void RockLayer::doUpdate(const float elapsed)
{
    m_shape_left->update(elapsed);
    m_shape_right->update(elapsed);
    m_shape_middle->update(elapsed);
}

void RockLayer::doDraw() const
{
    m_shape_left->draw(renderTarget());
    m_shape_right->draw(renderTarget());
    m_shape_middle->draw(renderTarget());
}
void RockLayer::flash() { m_shape_middle->flash(0.2f); }
