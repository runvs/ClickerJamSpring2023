#include "state_game.hpp"
#include <box2dwrapper/box2d_world_impl.hpp>
#include <color/color.hpp>
#include <drawable_helpers.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <hud/hud.hpp>
#include <nlohmann.hpp>
#include <random/random.hpp>
#include <screeneffects/vignette.hpp>
#include <shape.hpp>
#include <state_menu.hpp>
#include <tweens/tween_alpha.hpp>
#include <tweens/tween_color.hpp>
#include <tweens/tween_position.hpp>

#ifdef JT_ENABLE_WEB
#include <cstring>
#include <emscripten.h>
#endif

void StateGame::onCreate()
{
    float const w = static_cast<float>(GP::GetWindowSize().x);
    float const h = static_cast<float>(GP::GetWindowSize().y);

    using jt::Shape;

    m_bank = std::make_shared<Bank>();

    m_background = std::make_shared<Shape>();
    m_background->makeRect({ w, h }, textureManager());
    m_background->setColor(GP::PaletteBackground());
    m_background->setIgnoreCamMovement(true);
    m_background->update(0.0f);

    m_mine_shaft_model = std::make_shared<MineShaftModel>();
    m_mine_shaft_area
        = std::make_shared<MineShaftArea>(*m_mine_shaft_model.get(), [this](api::API const& value) {
              m_bank->receiveMoney(value);
              getGame()->gfx().camera().shake(0.1f, 3.0f);

              auto const isMouseInMineArea = (jt::MathHelper::checkIsIn(
                  { GP::HudMineShaftActiveLayerOffset().x, GP::HudMineShaftActiveLayerOffset().y,
                      GP::HudMineShaftLayerSize().x, GP::HudMineShaftLayerSize().y + 1.0f },
                  getGame()->input().mouse()->getMousePositionScreen()));
              if(isMouseInMineArea) {
                  m_sparks->fire(10, getGame()->input().mouse()->getMousePositionScreen());
              }
              m_digSound->play();
          });
    add(m_mine_shaft_area);

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
            twa->addCompleteCallback([shape]() { shape->setPosition({ -5000, -5000 }); });
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

    m_mousePointer = std::make_shared<CustomMouseCursor>();
    add(m_mousePointer);

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

        miner.objectsPerLine = GP::PurchasedNumberOfObjectsPerLine();
        miner.timerMax = 1.0f;
        miner.income = api::from_uint64(1u);
        miner.keyCode = jt::KeyCode::Num1;

        purchaseInfos.push_back(miner);
    }
    {
        PurchaseInfo geologist;
        geologist.name = "Geologist";

        geologist.animationFile = "assets/human/geologist.json";
        geologist.animationNameMenu = "idle";
        geologist.animationNamePurchased = "mine";

        geologist.initialCost = api::from_uint64(250u);
        geologist.purchaseCallback = [this](api::API const& /*cost*/) {
            m_purchasedObjects->addObject("Geologist");
            // TODO other effects
            // Note: No need to remove money from bank, button already takes care of this
        };

        geologist.objectsPerLine = GP::PurchasedNumberOfObjectsPerLine();
        geologist.timerMax = 1.0f;
        geologist.income = api::from_uint64(8u);
        geologist.keyCode = jt::KeyCode::Num2;

        purchaseInfos.push_back(geologist);
    }
    {
        PurchaseInfo driller;
        driller.name = "Driller";

        driller.animationFile = "assets/human/driller.json";
        driller.animationNameMenu = "idle";
        driller.animationNamePurchased = "mine";

        driller.initialCost = api::from_uint64(3000u);
        driller.purchaseCallback = [this](api::API const& /*cost*/) {
            m_purchasedObjects->addObject("Driller");
            // TODO other effects
            // Note: No need to remove money from bank, button already takes care of this
        };

        driller.objectsPerLine = GP::PurchasedNumberOfObjectsPerLine();
        driller.timerMax = 1.0f;
        driller.income = api::from_uint64(65u);
        driller.keyCode = jt::KeyCode::Num3;

        purchaseInfos.push_back(driller);
    }
    {
        PurchaseInfo blastMaster;
        blastMaster.name = "Blaster";
        blastMaster.animationFile = "assets/human/blaster.json";
        blastMaster.animationNameMenu = "idle";
        blastMaster.animationNamePurchased = "mine";

        blastMaster.initialCost = api::from_uint64(45000u);
        blastMaster.purchaseCallback = [this](api::API const& /*cost*/) {
            m_purchasedObjects->addObject("Blaster");
            // TODO other effects
            // Note: No need to remove money from bank, button already takes care of this
        };

        blastMaster.objectsPerLine = GP::PurchasedNumberOfObjectsPerLine();
        blastMaster.timerMax = 1.0f;
        blastMaster.income = api::from_uint64(600u);
        blastMaster.keyCode = jt::KeyCode::Num4;

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

    {
        auto btnSave = std::make_shared<jt::Button>(
            jt::Vector2u { GP::HudButtonSize().x / 2, GP::HudButtonSize().y }, textureManager());
        auto textSave = jt::dh::createText(renderTarget(), " Save", 16);
        textSave->setTextAlign(jt::Text::TextAlign::LEFT);
        btnSave->setDrawable(textSave);
        btnSave->setPosition({ 0, GP::GetScreenSize().y - GP::HudButtonSize().y });
        btnSave->addCallback([this]() { save(); });
        add(btnSave);
    }
    {
        m_btnLoad = std::make_shared<jt::Button>(
            jt::Vector2u { GP::HudButtonSize().x / 2, GP::HudButtonSize().y }, textureManager());
        auto textLoad = jt::dh::createText(renderTarget(), " Load", 16);
        textLoad->setTextAlign(jt::Text::TextAlign::LEFT);
        m_btnLoad->setDrawable(textLoad);
        m_btnLoad->setPosition(
            { GP::HudButtonSize().x / 2.0f, GP::GetScreenSize().y - GP::HudButtonSize().y });
        m_btnLoad->addCallback([this]() { load(); });
        add(m_btnLoad);
#if JT_ENABLE_WEB
        // TODO
#else
        std::ifstream infile { "savegame.dat" };
        if (!infile.good()) {
            m_btnLoad->setActive(false);
        }
#endif
    }
    m_hud = std::make_shared<Hud>();
    add(m_hud);
    // StateGame will call drawObjects itself.
    setAutoDraw(false);

    std::vector<std::shared_ptr<jt::SoundInterface>> soundGroupSounds {};
    for (auto i = 0; i != 6; ++i) {
        std::string const fileName = "assets/sfx/dig" + std::to_string(i) + ".wav";
        auto snd = getGame()->audio().addTemporarySound(fileName);
        snd->setVolume(0.6f);
        soundGroupSounds.push_back(snd);
    }
    m_digSound = getGame()->audio().addTemporarySoundGroup(soundGroupSounds);
}

void StateGame::onEnter() { }

void StateGame::createPlayer() { }

void StateGame::onUpdate(float const elapsed)
{
    if (m_running) {
        // update game logic here

        m_hud->getDepthScore()->notify(m_mine_shaft_model->getCurrentDepth());
        m_hud->getMoneyScore()->notify(m_bank->getCurrentMoney());
        m_hud->getMoneyPerSecond()->notify(m_purchasedObjects->getInputPerSecond());

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

    if ((jt::MathHelper::checkIsIn(
            { GP::HudMineShaftActiveLayerOffset().x, GP::HudMineShaftActiveLayerOffset().y,
                GP::HudMineShaftLayerSize().x, GP::HudMineShaftLayerSize().y + 1.0f },
            getGame()->input().mouse()->getMousePositionScreen()))) {
        m_mousePointer->playAnimation("mine");
    } else {
        m_mousePointer->playAnimation("default");
    }
}

void StateGame::onDraw() const
{
    m_background->draw(renderTarget());

    m_menuBackground->draw(renderTarget());
    drawObjects();
    m_sparks->draw();
    m_vignette->draw();
    m_hud->draw();
    m_mousePointer->draw();
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

std::string StateGame::serialize() const
{
    nlohmann::json j;
    std::map<std::string, std::pair<api::API, bool>> buttonPrices;
    for (auto const& btn : *m_purchaseButtons) {
        buttonPrices[btn.lock()->getButtonName()]
            = std::make_pair(btn.lock()->getPrice(), btn.lock()->wasVisible());
    }
    j["buttons"] = buttonPrices;
    j["bank"] = *m_bank;
    j["purchased"] = *m_purchasedObjects;
    return j.dump();
}

void StateGame::deserialize(std::string const& str)
{
    nlohmann::json j = nlohmann::json::parse(str);
    // buttons
    std::map<std::string, std::pair<api::API, bool>> buttonPrices = j["buttons"];
    for (auto const& btn : *m_purchaseButtons) {
        auto b = btn.lock();
        b->setPrice(buttonPrices.at(b->getButtonName()).first);
        b->setVisible(buttonPrices.at(b->getButtonName()).second);
    }

    // bank
    *m_bank = j["bank"];

    // purchased objects
    m_purchasedObjects->clean();
    auto const purchased = j["purchased"];
    for (auto kvp : purchased.items()) {
        for (int i = 0; i != kvp.value(); ++i) {
            m_purchasedObjects->addObject(kvp.key());
        }
    }
}
void StateGame::save()
{
    getGame()->logger().info("Save");
#if JT_ENABLE_WEB
    std::string const json_string = serialize();
    std::string const command = "save(" + json_string + ")";
    emscripten_run_script(command.c_str());
#else
    std::ofstream outfile { "savegame.dat" };
    outfile << serialize() << std::endl;
    m_btnLoad->setActive(true);
#endif
}

void StateGame::load()
{
    getGame()->logger().info("Load");
#if JT_ENABLE_WEB
    //    std::cout << "pre load\n";
    auto const savedata = emscripten_run_script_string("load()");
    std::cout << "savedata: '" << savedata << "'" << std::endl;
    //    std::cout << "post load\n";
    if (strcmp("", savedata) == 0) {
        getGame()->logger().warning("empty savedata");
        return;
    }

    deserialize(savedata);
//    std::cout << "post deserialize\n";
#else
    std::ifstream infile { "savegame.dat" };
    if (!infile.good()) {
        getGame()->logger().warning("empty savedata");
        return;
    }
    std::string savedata;
    infile >> savedata;
    deserialize(savedata);
#endif
}
