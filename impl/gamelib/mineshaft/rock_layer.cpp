
#include "rock_layer.hpp"
#include "input/mouse/mouse_defines.hpp"
#include "shape.hpp"
#include <game_interface.hpp>
#include <game_properties.hpp>

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
}

bool RockLayer::isMined() { return m_progress > m_hardness; }
void RockLayer::ascend() { m_layer_offset += 1; }

void RockLayer::doCreate()
{
    m_shape = std::make_shared<jt::Shape>();
    m_shape->makeRect(GP::HudMineShaftSize(), textureManager());
    m_shape->setPosition(
        { GP::HudMineShaftOffset().x, GP::HudMineShaftLayerHeight() * m_layer_offset });
    m_shape->setColor(m_color);
    m_shape->update(1.0f);
}
void RockLayer::doUpdate(const float elapsed) { m_shape->update(elapsed); }
void RockLayer::doDraw() const { m_shape->draw(renderTarget()); }
