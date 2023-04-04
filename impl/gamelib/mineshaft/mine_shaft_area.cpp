
#include "mine_shaft_area.hpp"
#include "input/mouse/mouse_defines.hpp"
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <math_helper.hpp>

MineShaftArea::MineShaftArea(std::function<void(const api::API&)> callback)
    : m_callback { callback }
{
}
void MineShaftArea::doCreate()
{
    m_shape = std::make_shared<jt::Shape>();
    m_shape->makeRect(GP::HudMineShaftSize(), textureManager());
    m_shape->setOffset(GP::HudMineShaftOffset());
    m_shape->setColor(jt::colors::Black);
    m_shape->update(1.0f);
}
void MineShaftArea::doUpdate(const float elapsed)
{
    if (getGame()->input().mouse()->justPressed(jt::MouseButtonCode::MBLeft)
        && (jt::MathHelper::checkIsIn({ GP::HudMineShaftOffset().x, GP::HudMineShaftOffset().y,
                                          GP::HudMineShaftSize().x, GP::HudMineShaftSize().y },
            getGame()->input().mouse()->getMousePositionScreen()))) {
        m_callback(api::from_uint64(5u));
    }
}
void MineShaftArea::doDraw() const { m_shape->draw(renderTarget()); }
