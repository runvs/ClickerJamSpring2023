#include "state_game.hpp"
#include <box2dwrapper/box2d_world_impl.hpp>
#include <color/color.hpp>
#include <drawable_helpers.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <hud/hud.hpp>
#include <screeneffects/vignette.hpp>
#include <shape.hpp>
#include <state_menu.hpp>

void StateGame::onCreate()
{
    m_world = std::make_shared<jt::Box2DWorldImpl>(jt::Vector2f { 0.0f, 0.0f });

    float const w = static_cast<float>(GP::GetWindowSize().x);
    float const h = static_cast<float>(GP::GetWindowSize().y);

    using jt::Shape;

    m_bank = std::make_shared<Bank>();

    m_background = std::make_shared<Shape>();
    m_background->makeRect({ w, h }, textureManager());
    m_background->setColor(GP::PaletteBackground());
    m_background->setIgnoreCamMovement(true);
    m_background->update(0.0f);

    createPlayer();

    m_menuBackground = std::make_shared<jt::Shape>();
    m_menuBackground->makeRect(GP::HudMenuSize(), textureManager());
    m_menuBackground->setPosition(GP::HudMenuOffset());

    // group
    m_purchaseButtons = std::make_shared<jt::ObjectGroup<PurchaseButton>>();
    add(m_purchaseButtons);

    std::vector<PurchaseInfo> purchaseInfos;
    purchaseInfos.push_back(PurchaseInfo { "Miner", api::from_uint64(10u),
        "assets/human/MiniArcherMan.json", "idle", [this](api::API const& /*cost*/) {
            // TODO spawn miner
            // TODO other effects
            // Note: No need to remove money from bank, button already takes care of this
        } });
    purchaseInfos.push_back(PurchaseInfo { "Geologist", api::from_uint64(100u),
        "assets/human/MiniArcherMan.json", "idle", [this](api::API const& /*cost*/) {
            // TODO spawn geologist
            // TODO other effects
            // Note: No need to remove money from bank, button already takes care of this
        } });

    // automatically fill index member variable
    for (auto i = 0u; i != purchaseInfos.size(); ++i) {
        purchaseInfos[i].index = static_cast<int>(i);
        auto button = std::make_shared<PurchaseButton>(*m_bank.get(), purchaseInfos[i]);
        m_purchaseButtons->push_back(button);
        add(button);
    }

    m_vignette = std::make_shared<jt::Vignette>(GP::GetScreenSize());
    add(m_vignette);
    m_hud = std::make_shared<Hud>();
    add(m_hud);

    // StateGame will call drawObjects itself.
    setAutoDraw(false);
}

void StateGame::onEnter() { }

void StateGame::createPlayer() { }

void StateGame::onUpdate(float const elapsed)
{
    if (m_running) {
        m_world->step(elapsed, GP::PhysicVelocityIterations(), GP::PhysicPositionIterations());
        // update game logic here
        if (getGame()->input().keyboard()->justPressed(jt::KeyCode::A)) {
            m_scoreP1++;
            m_hud->getObserverScoreP1()->notify(m_scoreP1);
        }
        if (getGame()->input().keyboard()->justPressed(jt::KeyCode::D)) {
            m_scoreP2++;
            m_hud->getObserverScoreP2()->notify(m_scoreP2);
        }
        if (getGame()->input().keyboard()->pressed(jt::KeyCode::LShift)
            && getGame()->input().keyboard()->pressed(jt::KeyCode::Escape)) {
            endGame();
        }
        m_menuBackground->update(elapsed);

#if JT_ENABLE_DEBUG
        if (getGame()->input().keyboard()->justPressed(jt::KeyCode::M)
            && getGame()->input().keyboard()->pressed(jt::KeyCode::LShift)) {
            m_bank->receiveMoney(api::from_uint64(50));
        }
#endif
    }

    m_background->update(elapsed);
    m_vignette->update(elapsed);
}

void StateGame::onDraw() const
{
    m_background->draw(renderTarget());

    m_menuBackground->draw(renderTarget());
    drawObjects();
    m_vignette->draw();
    m_hud->draw();
}

void StateGame::endGame()
{
    if (m_hasEnded) {
        // trigger this function only once
        return;
    }
    m_hasEnded = true;
    m_running = false;

    getGame()->stateManager().switchToStoredState("menu");
}

std::string StateGame::getName() const { return "State Game"; }
