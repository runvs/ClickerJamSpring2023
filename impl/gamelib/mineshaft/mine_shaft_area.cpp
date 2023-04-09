
#include "mine_shaft_area.hpp"
#include "input/mouse/mouse_defines.hpp"
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <math_helper.hpp>
#include <random/random.hpp>

MineShaftArea::MineShaftArea(MineShaftModel& model, std::function<void(const api::API&)> callback)
    : m_mine_shaft_model { model }
    , m_callback { callback }
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
            continue;
        }
        layer->draw();
    }
}
void MineShaftArea::handleMouseClicks()
{
    auto const mouseJustPressed
        = getGame()->input().mouse()->justPressed(jt::MouseButtonCode::MBLeft);
    auto const spaceBarJustPressed = getGame()->input().keyboard()->justPressed(jt::KeyCode::Space);
    auto const isMouseInMineArea = (jt::MathHelper::checkIsIn(
        { GP::HudMineShaftActiveLayerOffset().x, GP::HudMineShaftActiveLayerOffset().y,
            GP::HudMineShaftLayerSize().x, GP::HudMineShaftLayerSize().y + 1.0f },
        getGame()->input().mouse()->getMousePositionScreen()));

    if (spaceBarJustPressed || (mouseJustPressed && isMouseInMineArea)) {
        m_callback(api::from_uint64(1u));
        auto active_layer = getActiveLayer();
        active_layer->progressAmount(1);
        if (active_layer->isMined()) {
            // TODO: add animation + sound for braking layer (flash?)
            cycleLayers();
            descend();
        }
    }
}

std::shared_ptr<RockLayer> MineShaftArea::getActiveLayer()
{
    return m_rock_layers[(m_rock_layers.getTail() + 8) % 17];
};

void MineShaftArea::cycleLayers()
{
    for (auto& layer : m_rock_layers) {
        if (layer == nullptr) {
            continue;
        }
        layer->ascend();
    }
    auto new_layer = std::make_shared<RockLayer>(
        jt::Random::getInt(1, m_mine_shaft_model.getNumberOfMinedLayers() + 5),
        jt::Random::getRandomColor(), 16.0f);
    new_layer->setGameInstance(getGame());
    new_layer->create();
    m_rock_layers.put(new_layer);
}
void MineShaftArea::descend() { m_mine_shaft_model.descend(); }
