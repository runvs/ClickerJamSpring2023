//

#include "trollface.hpp"
#include "game_properties.hpp"
#include <game_interface.hpp>

void TrollFace::doCreate()
{
    m_troll_face = std::make_shared<jt::Sprite>("assets/troll_face_small.png", textureManager());
    m_troll_face->setOrigin(jt::OriginMode::CENTER);
    m_troll_face->setPosition(GP::GetScreenSize() * 0.5f);
}
void TrollFace::doUpdate(const float elapsed)
{

    float rotation_rate = 0.0f;
    if (getGame()->input().keyboard()->pressed(jt::KeyCode::Q)) {
        rotation_rate = +15.0f;
    } else if (getGame()->input().keyboard()->pressed(jt::KeyCode::E)) {
        rotation_rate = -15.0f;
    }

    m_troll_face->setRotation(m_troll_face->getRotation() + (rotation_rate * elapsed));
    m_troll_face->update(elapsed);
}
void TrollFace::doDraw() const { m_troll_face->draw(renderTarget()); }
