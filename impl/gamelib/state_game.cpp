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

    m_separator = std::make_shared<jt::Shape>();
    m_separator->makeRect({ 2.0f, GP::GetScreenSize().y }, textureManager());
    m_separator->setColor(jt::colors::Black);

    m_hud = std::make_shared<Hud>();
    add(m_hud);

    m_mine_shaft_model = std::make_shared<MineShaftModel>();
    m_mine_shaft_area = std::make_shared<MineShaftArea>(
        *m_mine_shaft_model.get(),
        [this](api::API const& value) {
            m_bank->receiveMoney(value);
            getGame()->gfx().camera().shake(0.1f, 2.0f);
            m_sparks->fire(
                10, jt::Random::getRandomPointIn(m_mine_shaft_area->getCurrentLayer()->getArea()));
            m_sparks->fire(
                10, jt::Random::getRandomPointIn(m_mine_shaft_area->getCurrentLayer()->getArea()));
            m_sparks->fire(
                10, jt::Random::getRandomPointIn(m_mine_shaft_area->getCurrentLayer()->getArea()));

            m_digSound->play();
        },
        [this](auto tween) { add(tween); });

    m_mine_shaft_area->setDescendHudObservers(
        m_hud->getMoneyPerClickObserver(), m_hud->getDepthObserver());
    add(m_mine_shaft_area);

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
            // Note: No need to remove money from bank, button already takes care of this
        };

        miner.objectsPerLine = GP::PurchasedNumberOfObjectsPerLine();
        miner.timerMax = 1.0f;
        miner.income = api::from_uint64(1u);
        miner.keyCode = jt::KeyCode::Num1;

        miner.progressMiningTimerMax = 8.0f;
        miner.progressMiningValue = 1u;

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
            // Note: No need to remove money from bank, button already takes care of this
        };

        geologist.objectsPerLine = GP::PurchasedNumberOfObjectsPerLine();
        geologist.timerMax = 1.0f;
        geologist.income = api::from_uint64(8u);
        geologist.keyCode = jt::KeyCode::Num2;

        geologist.progressMiningTimerMax = 6.0f;
        geologist.progressMiningValue = 2u;

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
            // Note: No need to remove money from bank, button already takes care of this
        };

        driller.objectsPerLine = GP::PurchasedNumberOfObjectsPerLine();
        driller.timerMax = 1.0f;
        driller.income = api::from_uint64(65u);
        driller.keyCode = jt::KeyCode::Num3;

        driller.progressMiningTimerMax = 4.0f;
        driller.progressMiningValue = 4u;

        purchaseInfos.push_back(driller);
    }
    {
        PurchaseInfo blastMaster;
        blastMaster.name = "Blaster";
        blastMaster.animationFile = "assets/human/blaster.json";
        blastMaster.animationNameMenu = "idle";
        blastMaster.animationNamePurchased = "mine";

        blastMaster.initialCost = api::from_uint64(35000u);
        blastMaster.purchaseCallback = [this](api::API const& /*cost*/) {
            m_purchasedObjects->addObject("Blaster");
            // Note: No need to remove money from bank, button already takes care of this
        };

        blastMaster.objectsPerLine = GP::PurchasedNumberOfObjectsPerLine();
        blastMaster.timerMax = 1.0f;
        blastMaster.income = api::from_uint64(600u);
        blastMaster.keyCode = jt::KeyCode::Num4;

        blastMaster.progressMiningTimerMax = 3.0f;
        blastMaster.progressMiningValue = 8u;

        purchaseInfos.push_back(blastMaster);
    }

    // automatically fill index member variable
    for (auto i = 0u; i != purchaseInfos.size(); ++i) {
        purchaseInfos[i].index = static_cast<int>(i);
        auto button = std::make_shared<PurchaseButton>(*m_bank.get(), purchaseInfos[i],
            [this](std::shared_ptr<jt::TweenInterface> tween) { add(tween); });
        m_purchaseButtons->push_back(button);
        add(button);
    }

    m_purchasedObjects = std::make_shared<PurchasedObjects>(
        *m_bank.get(), purchaseInfos, [this](auto tw) { add(tw); });
    m_purchasedObjects->setProgressMiningCallback(
        [this](std::uint64_t value) { m_mine_shaft_area->progressMining(value); });
    add(m_purchasedObjects);

    m_vignette = std::make_shared<jt::Vignette>(GP::GetScreenSize());
    add(m_vignette);

    {
        m_btnSave = std::make_shared<jt::Button>(
            jt::Vector2u { GP::HudButtonSize().x / 2, GP::HudButtonSize().y }, textureManager());
        auto textSave = jt::dh::createText(renderTarget(), " Save", 16);
        textSave->setTextAlign(jt::Text::TextAlign::LEFT);
        m_btnSave->setDrawable(textSave);
        m_btnSave->setPosition({ 0, GP::GetScreenSize().y - GP::HudButtonSize().y });
        m_btnSave->addCallback([this]() { save(); });
        add(m_btnSave);
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
        // nothing to be done
#else
        std::ifstream infile { "savegame.dat" };
        if (!infile.good()) {
            m_btnLoad->setActive(false);
        }
#endif
    }

    // StateGame will call drawObjects itself.
    setAutoDraw(false);

    std::vector<std::shared_ptr<jt::SoundInterface>> soundGroupSounds {};
    for (auto i = 0; i != 6; ++i) {
        std::string const fileName = "assets/sfx/dig" + std::to_string(i) + ".wav";
        auto snd = getGame()->audio().addTemporarySound(fileName);
        snd->setVolume(0.4f);
        soundGroupSounds.push_back(snd);
    }
    m_digSound = getGame()->audio().addTemporarySoundGroup(soundGroupSounds);
}

void StateGame::onEnter() { }

void StateGame::onUpdate(float const elapsed)
{
    // note: depth and moneyPerClick are updated only on value changes
    m_hud->getMoneyScore()->notify(m_bank->getCurrentMoney());
    m_hud->getMoneyPerSecond()->notify(m_purchasedObjects->getInputPerSecond());

    updateCheats();
    updateMousePointer();

    m_background->update(elapsed);
    m_menuBackground->update(elapsed);
    m_vignette->update(elapsed);
}

void StateGame::updateCheats()
{
#if JT_ENABLE_DEBUG
    if (getGame()->input().keyboard()->pressed(jt::KeyCode::LShift)
        && getGame()->input().keyboard()->pressed(jt::KeyCode::Escape)) {
        endGame();
    }
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

void StateGame::updateMousePointer()
{
    if (m_mine_shaft_area->checkIfMouseIsOverArea()) {
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
    m_btnSave->draw();
    m_btnLoad->draw();

    drawSeparators();

    m_hud->draw();
    m_mousePointer->draw();
}
void StateGame::drawSeparators() const
{
    m_separator->setPosition({ GP::HudMineShaftOffset().x, 0.0f });
    m_separator->update(0.0f);
    m_separator->draw(renderTarget());

    m_separator->setPosition({ GP::HudMenuOffset().x - 2, 0.0f });
    m_separator->update(0.0f);
    m_separator->draw(renderTarget());
}

void StateGame::endGame() { getGame()->stateManager().switchToStoredState("menu"); }

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
    j["depth"] = m_mine_shaft_model->getCurrentDepth();
    j["moneyPerClick"] = m_mine_shaft_area->getMoneyPerClick();
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

    api::API depth;
    j["depth"].get_to(depth);
    m_mine_shaft_model->setCurrentDepth(depth);
    m_hud->getDepthObserver()->notify(depth);

    api::API moneyPerClick;
    j["moneyPerClick"].get_to(moneyPerClick);
    m_hud->getMoneyPerClickObserver()->notify(moneyPerClick);
    m_mine_shaft_area->setMoneyPerClick(moneyPerClick);
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
