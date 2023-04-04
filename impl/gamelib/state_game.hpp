#ifndef GAME_STATE_GAME_HPP
#define GAME_STATE_GAME_HPP

#include "animation.hpp"
#include "bank_interface.hpp"
#include "button.hpp"
#include "object_group.hpp"
#include "purchase_button.hpp"
#include "purchased_objects.hpp"
#include "text.hpp"
#include <bank.hpp>
#include <box2dwrapper/box2d_world_interface.hpp>
#include <game_state.hpp>
#include <particle_system.hpp>
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

    std::string save();
    void load(std::string const& str);

private:
    std::shared_ptr<jt::Shape> m_background;
    std::shared_ptr<jt::Vignette> m_vignette;
    std::shared_ptr<Hud> m_hud;
    std::shared_ptr<jt::Box2DWorldInterface> m_world { nullptr };

    std::shared_ptr<jt::Shape> m_menuBackground;

    std::shared_ptr<jt::ObjectGroup<PurchaseButton>> m_purchaseButtons;
    std::shared_ptr<Bank> m_bank;
    std::shared_ptr<PurchasedObjects> m_purchasedObjects;

    std::shared_ptr<jt::ParticleSystem<jt::Shape, 100>> m_sparks;

    bool m_running { true };
    bool m_hasEnded { false };

    api::API m_money;

    void onCreate() override;
    void onEnter() override;
    void onUpdate(float const elapsed) override;
    void onDraw() const override;

    void endGame();
    void createPlayer();
};

#endif
