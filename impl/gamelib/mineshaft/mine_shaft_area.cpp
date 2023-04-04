
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

    for (auto i = 0u; i != m_rock_layers.size(); i++) {
        auto layer = std::make_shared<RockLayer>(
            jt::Random::getInt(1, i + 2), jt::Random::getRandomColor(), static_cast<float>(i));
        m_rock_layers[i] = (layer);
        layer->setGameInstance(getGame());
        layer->create();
    }
}
void MineShaftArea::doUpdate(const float elapsed)
{
    if (getGame()->input().mouse()->justPressed(jt::MouseButtonCode::MBLeft)
        && (jt::MathHelper::checkIsIn({ GP::HudMineShaftOffset().x, GP::HudMineShaftOffset().y,
                                          GP::HudMineShaftSize().x, GP::HudMineShaftSize().y },
            getGame()->input().mouse()->getMousePositionScreen()))) {
        m_callback(api::from_uint64(5u));
    }
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
