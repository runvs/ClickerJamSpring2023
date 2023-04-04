#include "state_game.hpp"
#include <box2dwrapper/box2d_world_impl.hpp>
#include <color/color.hpp>
#include <drawable_helpers.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <hud/hud.hpp>
#include <random/random.hpp>
#include <screeneffects/vignette.hpp>
#include <shape.hpp>
#include <state_menu.hpp>
#include <tweens/tween_alpha.hpp>
#include <tweens/tween_color.hpp>
#include <tweens/tween_position.hpp>

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

    // TODO: refactor to own class
    m_sparks = jt::ParticleSystem<jt::Shape, 100>::createPS(
        [this]() {
            auto shape = std::make_shared<jt::Shape>();
            shape->makeRect({ 1, 1 }, textureManager());
            shape->setPosition({ -5000, -5000 });
            return shape;
        },
        [this](auto shape, auto pos) {
            pos = pos + jt::Random::getRandomPointIn({ -4, -4, 8, 8 });
            shape->setPosition(pos);
            auto twp = jt::TweenPosition::create(
                shape, 0.2f, pos, pos + jt::Random::getRandomPointInRadius(12));
            add(twp);

            auto twa = jt::TweenAlpha::create(shape, 0.2f, 255, 0);
            twa->setStartDelay(0.1f);
            add(twa);

            std::shared_ptr<jt::Tween> twc1
                = jt::TweenColor::create(shape, 0.1f, jt::colors::Red, jt::colors::Yellow);
            twc1->addCompleteCallback([this, shape]() {
                std::shared_ptr<jt::Tween> twc2
                    = jt::TweenColor::create(shape, 0.1f, jt::colors::Yellow, jt::colors::White);
                add(twc2);
            });
            add(twc1);
        });
    add(m_sparks);

    m_menuBackground = std::make_shared<jt::Shape>();
    m_menuBackground->makeRect(GP::HudMenuSize(), textureManager());
    m_menuBackground->setPosition(GP::HudMenuOffset());

    // group
    m_purchaseButtons = std::make_shared<jt::ObjectGroup<PurchaseButton>>();
    add(m_purchaseButtons);

    std::vector<PurchaseInfo> purchaseInfos;

    {
        PurchaseInfo miner;
        miner.name = "Miner";

        miner.animationFile = "assets/human/miner.json";
        miner.animationNameMenu = "idle";
        miner.animationNamePurchased = "mine";

        miner.initialCost = api::from_uint64(8u);
        miner.purchaseCallback = [this](api::API const& /*cost*/) {
            m_purchasedObjects->addObject("Miner");
            // TODO other effects
            // Note: No need to remove money from bank, button already takes care of this
        };

        miner.objectsPerLine = GP::PurchasedNumberOfMinersPerLine();
        miner.timerMax = 1.0f;
        miner.income = api::from_uint64(1u);

        purchaseInfos.push_back(miner);
    }
    {
        PurchaseInfo geologist;
        geologist.name = "Geologist";

        geologist.animationFile = "assets/human/geologist.json";
        geologist.animationNameMenu = "idle";
        geologist.animationNamePurchased = "mine";

        geologist.initialCost = api::from_uint64(100u);
        geologist.purchaseCallback = [this](api::API const& /*cost*/) {
            m_purchasedObjects->addObject("Geologist");
            // TODO other effects
            // Note: No need to remove money from bank, button already takes care of this
        };

        geologist.objectsPerLine = GP::PurchasedNumberOfMinersPerLine();
        geologist.timerMax = 1.0f;
        geologist.income = api::from_uint64(8u);

        purchaseInfos.push_back(geologist);
    }
    {
        PurchaseInfo driller;
        driller.name = "Driller";

        driller.animationFile = "assets/human/MiniArcherMan.json";
        driller.animationNameMenu = "idle";
        driller.animationNamePurchased = "attack";

        driller.initialCost = api::from_uint64(1000u);
        driller.purchaseCallback = [this](api::API const& /*cost*/) {
            m_purchasedObjects->addObject("Driller");
            // TODO other effects
            // Note: No need to remove money from bank, button already takes care of this
        };

        driller.objectsPerLine = GP::PurchasedNumberOfMinersPerLine();
        driller.timerMax = 1.0f;
        driller.income = api::from_uint64(70u);

        purchaseInfos.push_back(driller);
    }
    {
        PurchaseInfo blastMaster;
        blastMaster.name = "Blaster";

        blastMaster.animationFile = "assets/human/MiniArcherMan.json";
        blastMaster.animationNameMenu = "idle";
        blastMaster.animationNamePurchased = "attack";

        blastMaster.initialCost = api::from_uint64(10000u);
        blastMaster.purchaseCallback = [this](api::API const& /*cost*/) {
            m_purchasedObjects->addObject("Blaster");
            // TODO other effects
            // Note: No need to remove money from bank, button already takes care of this
        };

        blastMaster.objectsPerLine = GP::PurchasedNumberOfMinersPerLine();
        blastMaster.timerMax = 1.0f;
        blastMaster.income = api::from_uint64(600u);

        purchaseInfos.push_back(blastMaster);
    }

    // automatically fill index member variable
    for (auto i = 0u; i != purchaseInfos.size(); ++i) {
        purchaseInfos[i].index = static_cast<int>(i);
        auto button = std::make_shared<PurchaseButton>(*m_bank.get(), purchaseInfos[i]);
        m_purchaseButtons->push_back(button);
        add(button);
    }

    m_purchasedObjects = std::make_shared<PurchasedObjects>(*m_bank.get(), purchaseInfos);
    add(m_purchasedObjects);

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

        m_hud->getDepthScore()->notify(api::from_uint64(static_cast<std::uint64_t>(getAge())));
        m_hud->getMoneyScore()->notify(m_bank->getCurrentMoney());
        m_hud->getMoneyPerSecond()->notify(m_purchasedObjects->getInputPerSecond());

        if (getGame()->input().keyboard()->pressed(jt::KeyCode::LShift)
            && getGame()->input().keyboard()->pressed(jt::KeyCode::Escape)) {
            endGame();
        }

        if (getGame()->input().mouse()->justPressed(jt::MouseButtonCode::MBLeft)
            && (getGame()->input().mouse()->getMousePositionScreen().x < GP::HudMenuOffset().x)) {
            m_bank->receiveMoney(api::from_uint64(1u));
            getGame()->gfx().camera().shake(0.1, 3);
            m_sparks->fire(10, getGame()->input().mouse()->getMousePositionScreen());
        }
        m_menuBackground->update(elapsed);

#if JT_ENABLE_DEBUG
        if (getGame()->input().keyboard()->justPressed(jt::KeyCode::M)
            && getGame()->input().keyboard()->pressed(jt::KeyCode::LShift)) {
            m_bank->receiveMoney(api::from_uint64(50));
        }
        if (getGame()->input().keyboard()->justPressed(jt::KeyCode::N)
            && getGame()->input().keyboard()->pressed(jt::KeyCode::LShift)) {
            auto current = m_bank->getCurrentMoney();
            current = current * api::from_uint64(100);
            m_bank->receiveMoney(current);
        }
#endif
    }

    m_background->update(elapsed);
    m_vignette->update(elapsed);
}

void StateGame::onDraw() const
{
    //    std::cout << m_bank->getCurrentMoney().to_string() << " "
    //              << m_purchasedObjects->getInputPerMinute().to_exp_string() << std::endl;
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
