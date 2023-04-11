#ifndef GAME_STATE_GAME_HPP
#define GAME_STATE_GAME_HPP

#include "text.hpp"
#include <animation.hpp>
#include <bank.hpp>
#include <bank_interface.hpp>
#include <box2dwrapper/box2d_world_interface.hpp>
#include <button.hpp>
#include <custom_mouse_cursor.hpp>
#include <game_state.hpp>
#include <mineshaft/mine_shaft_area.hpp>
#include <mineshaft/mine_shaft_model.hpp>
#include <object_group.hpp>
#include <particle_system.hpp>
#include <purchase_button.hpp>
#include <purchased_objects.hpp>
#include <serialization.hpp>
#include <memory>
#include <vector>

// fwd decls
namespace jt {
class Shape;
class Sprite;
class Vignette;
} // namespace jt

class Hud;

class StateGame : public jt::GameState {
public:
    std::string getName() const override;

    std::string serialize() const;
    void deserialize(std::string const&);

    void save();
    void load();

private:
    std::shared_ptr<jt::Shape> m_background;
    std::shared_ptr<jt::Vignette> m_vignette;
    std::shared_ptr<Hud> m_hud;
    std::shared_ptr<MineShaftArea> m_mine_shaft_area;

    std::shared_ptr<CustomMouseCursor> m_mousePointer;

    std::shared_ptr<jt::Shape> m_menuBackground;
    std::shared_ptr<jt::Shape> m_separator;

    std::shared_ptr<jt::ObjectGroup<PurchaseButton>> m_purchaseButtons;
    std::shared_ptr<Bank> m_bank;
    std::shared_ptr<MineShaftModel> m_mine_shaft_model;
    std::shared_ptr<PurchasedObjects> m_purchasedObjects;

    std::shared_ptr<jt::ParticleSystem<jt::Shape, 100>> m_sparks;

    std::shared_ptr<jt::Button> m_btnSave;
    std::shared_ptr<jt::Button> m_btnLoad;

    api::API m_money;
    std::shared_ptr<jt::SoundInterface> m_digSound;

    void onCreate() override;
    void onEnter() override;
    void onUpdate(float const elapsed) override;
    void onDraw() const override;

    void endGame();
    void updateMousePointer();
    void updateCheats();
    void drawSeparators() const;
};

#endif
