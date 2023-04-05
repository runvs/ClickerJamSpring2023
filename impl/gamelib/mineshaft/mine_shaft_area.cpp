
#include "mine_shaft_area.hpp"
#include "input/mouse/mouse_defines.hpp"
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <math_helper.hpp>
#include <random/random.hpp>

MineShaftArea::MineShaftArea(std::function<void(const api::API&)> callback)
    : m_callback { callback }
{
}
void MineShaftArea::doCreate()
{
    m_background_shape = std::make_shared<jt::Shape>();
    m_background_shape->makeRect(GP::HudMineShaftSize(), textureManager());
    m_background_shape->setPosition(GP::HudMineShaftOffset());
    m_background_shape->setColor(jt::colors::Black);
    m_background_shape->update(1.0f);

    std::uint8_t active_layer_index = m_rock_layers.capacity() / 2;
    std::cout << "active layer index: " << active_layer_index << std::endl;
    for (auto i = 0u; i != m_rock_layers.capacity(); i++) {
        jt::Color color;
        if (i < active_layer_index) {
            std::uint8_t offset = 5u * i;
            std::uint8_t r = (10u + offset) % 255;
            std::uint8_t g = (10u + offset) % 255;
            std::uint8_t b = (100u + offset) % 255;
            color = jt::Color { r, g, b };
        } else if (i > active_layer_index) {
            color = jt::Random::getRandomColor();
        } else {
            color = jt::Color { 112u, 128u, 144u };
        }
        auto layer = std::make_shared<RockLayer>(
            jt::Random::getInt(1, i + 2), color, static_cast<float>(i));
        m_rock_layers[i] = (layer);
        layer->setGameInstance(getGame());
        layer->create();
    }
}
void MineShaftArea::doUpdate(const float elapsed)
{
    handleMouseClicks();

    for (auto& layer : m_rock_layers) {
        if (layer == nullptr) {
            continue;
        }
        layer->update(elapsed);
    }
}
void MineShaftArea::doDraw() const
{
    m_background_shape->draw(renderTarget());
    for (auto const& layer : m_rock_layers) {
        if (layer == nullptr) {
            std::cout << "skip layer" << std::endl;
            continue;
        }
        layer->draw();
    }
}
void MineShaftArea::handleMouseClicks()
{
    if (getGame()->input().mouse()->justPressed(jt::MouseButtonCode::MBLeft)
        && (jt::MathHelper::checkIsIn(
            { GP::HudMineShaftActiveLayerOffset().x, GP::HudMineShaftActiveLayerOffset().y,
                GP::HudMineShaftActiveLayerSize().x, GP::HudMineShaftActiveLayerSize().y },
            getGame()->input().mouse()->getMousePositionScreen()))) {
        m_callback(api::from_uint64(5u));
    }
}
