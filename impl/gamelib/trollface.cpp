//

#include "trollface.hpp"
#include "game_properties.hpp"
#include <color/palette_builder.hpp>
#include <game_interface.hpp>

void TrollFace::doCreate()
{
    m_troll_face = std::make_shared<jt::Sprite>("assets/troll_face_small.png", textureManager());
    m_troll_face->setOrigin(jt::OriginMode::CENTER);
    m_troll_face->setPosition(GP::GetScreenSize() * 0.5f);
    jt::PaletteBuilder builder;
    m_palette = builder.addGradientH(0.0f, 360.0f, 80.0f, 80.0f, 100u).create();
    m_paletteIndex = 0u;
    m_paletteTimer = 0.0f;
}
void TrollFace::doUpdate(const float elapsed)
{
    float rotation_rate = 0.0f;
    if (getGame()->input().keyboard()->pressed(jt::KeyCode::Q)) {
        rotation_rate += 30.0f;
    }
    if (getGame()->input().keyboard()->pressed(jt::KeyCode::E)) {
        rotation_rate -= 30.0f;
    }
    if (getGame()->input().keyboard()->pressed(jt::KeyCode::LShift)) {
        rotation_rate *= 2;
    }
    if (getGame()->input().keyboard()->pressed(jt::KeyCode::R)) {
        m_paletteTimer += elapsed;
        float maxTimer = 0.05f;
        if (m_paletteTimer >= maxTimer) {
            m_paletteTimer -= maxTimer;
            m_paletteIndex++;
            if (m_paletteIndex >= m_palette.size()) {
                m_paletteIndex = 0u;
            }
            m_troll_face->setColor(m_palette.getColor(m_paletteIndex));
        }
    }
    if (getGame()->input().keyboard()->justPressed(jt::KeyCode::Z)) {
        m_troll_face->flash(0.4f, jt::colors::Green);
    }

    m_troll_face->setRotation(m_troll_face->getRotation() + (rotation_rate * elapsed));
    m_troll_face->update(elapsed);
}
void TrollFace::doDraw() const { m_troll_face->draw(renderTarget()); }
