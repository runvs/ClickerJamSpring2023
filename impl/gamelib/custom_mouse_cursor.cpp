#include "custom_mouse_cursor.hpp"
#include <game_interface.hpp>

void CustomMouseCursor::doCreate()
{
    m_cursorAnimation = std::make_shared<jt::Animation>();
    m_cursorAnimation->loadFromJson("assets/pointer.json", textureManager());
    m_cursorAnimation->play("default");
    m_cursorAnimation->setIgnoreCamMovement(true);

    getGame()->gfx().window().setMouseCursorVisible(false);
}
void CustomMouseCursor::doUpdate(float const elapsed)
{
    m_cursorAnimation->setPosition(getGame()->input().mouse()->getMousePositionScreen());
    m_cursorAnimation->update(elapsed);
}
void CustomMouseCursor::doDraw() const { m_cursorAnimation->draw(renderTarget()); }
void CustomMouseCursor::playAnimation(std::string const& animName)
{
    m_cursorAnimation->play(animName);
}
