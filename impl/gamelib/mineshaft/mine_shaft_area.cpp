
#include "mine_shaft_area.hpp"
#include "input/mouse/mouse_defines.hpp"
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <math_helper.hpp>
#include <random/random.hpp>

MineShaftArea::MineShaftArea(MineShaftModel& model, std::function<void(const api::API&)> callback,
    std::function<void(std::shared_ptr<jt::TweenInterface>)> const& addTweenCallback)
    : m_onClickCallback { callback }
    , m_mine_shaft_model { model }
    , m_addTweenCallback { addTweenCallback }
{
    m_clickReturn = api::from_uint64(1);
}
void MineShaftArea::doCreate()
{
    m_background_shape = std::make_shared<jt::Shape>();
    m_background_shape->makeRect(GP::HudMineShaftLayerSize(), textureManager());
    m_background_shape->setPosition(GP::HudMineShaftOffset());
    m_background_shape->setColor(jt::colors::Black);
    m_background_shape->update(1.0f);

    std::size_t activeLayerIndex = m_rock_layers.capacity() / 2;
    for (auto i = 0u; i != m_rock_layers.capacity(); i++) {
        jt::Color color;
        auto isSky = false;
        if (i < activeLayerIndex) {
            std::uint8_t offset = 5u * i;
            std::uint8_t r = (10u + offset) % 255;
            std::uint8_t g = (10u + offset) % 255;
            std::uint8_t b = (100u + offset) % 255;
            color = jt::Color { r, g, b };
            isSky = true;
        } else if (i > activeLayerIndex) {
            color = jt::Random::getRandomColor();
        } else {
            color = jt::Color { 112u, 128u, 144u };
        }
        auto layer = std::make_shared<RockLayer>(jt::Random::getInt(1, static_cast<int>(i) + 2),
            color, static_cast<float>(i), m_addTweenCallback, isSky);
        m_rock_layers[i] = (layer);
        layer->setGameInstance(getGame());
        layer->create();
    }

    m_descentSound = getGame()->audio().addTemporarySound("assets/sfx/mined1.wav");
    m_descentSound->setVolume(0.5f);

    updateHudObservers();
}
void MineShaftArea::doUpdate(const float elapsed)
{
    handleMouseClicks();

    for (auto& layer : m_rock_layers) {
        if (layer == nullptr) {
            continue;
        }
        layer->update(elapsed);
    }
}
void MineShaftArea::doDraw() const
{
    m_background_shape->draw(renderTarget());
    for (auto const& layer : m_rock_layers) {
        if (layer == nullptr) {
            continue;
        }
        layer->draw();
    }
}

void MineShaftArea::handleMouseClicks()
{
    auto const mouseJustPressed
        = getGame()->input().mouse()->justPressed(jt::MouseButtonCode::MBLeft);
    auto const spaceBarJustPressed = getGame()->input().keyboard()->justPressed(jt::KeyCode::Space);
    auto const isMouseInMineArea = (jt::MathHelper::checkIsIn(
        { GP::HudMineShaftOffset().x, GP::HudMineShaftActiveLayerOffset().y,
            GP::HudMineShaftLayerSize().x, GP::HudMineShaftLayerSize().y + 1.0f },
        getGame()->input().mouse()->getMousePositionScreen()));

    if (spaceBarJustPressed || (mouseJustPressed && isMouseInMineArea)) {
        m_onClickCallback(m_clickReturn);
        progressMining();
    }
}

void MineShaftArea::progressMining(std::uint64_t value)
{
    auto active_layer = getActiveLayer();
    active_layer->progressAmount(value);
    if (active_layer->isMined()) {
        m_mine_shaft_model.addMinedLayer();
        flashActiveLayer();
        cycleLayers();
        descend();
    }
}

std::shared_ptr<RockLayer> MineShaftArea::getActiveLayer()
{
    return m_rock_layers[(m_rock_layers.getTail() + 8) % 17];
};

void MineShaftArea::cycleLayers()
{
    for (auto& layer : m_rock_layers) {
        if (layer == nullptr) {
            continue;
        }
        layer->ascend();
    }

    // TODO do not pick random color, but limit hsv. Or use a predefined palette.
    auto const r = static_cast<std::uint8_t>(jt::Random::getInt(10, 255));
    auto const g = static_cast<std::uint8_t>(jt::Random::getInt(10, 255));
    auto const b = static_cast<std::uint8_t>(jt::Random::getInt(10, 255));

    std::uint64_t const hardness
        = m_mine_shaft_model.getNumberOfMinedLayers() / GP::MineShaftLevelForHardnessIncrease()
        + jt::Random::getInt(4, 7);

    jt::Color const col { r, g, b };
    std::stringstream ss;
    ss << "Create rock layer with color " << col << " and hardness " << hardness;
    getGame()->logger().info(ss.str());
    auto new_layer = std::make_shared<RockLayer>(hardness, col, 16.0f, m_addTweenCallback);
    new_layer->setGameInstance(getGame());
    new_layer->create();
    m_rock_layers.put(new_layer);
}

void MineShaftArea::descend()
{
    m_mine_shaft_model.descend();
    m_descentSound->play();
    getGame()->gfx().camera().shake(0.6f, 6.0f);

    api::API const clickReturnOffset = api::from_uint64(1u);
    api::API const levelsNeededForClickReturnIncrease
        = api::from_uint64(GP::MineShaftDepthForClickReturnIncrease());
    m_clickReturn = m_mine_shaft_model.getCurrentDepth() / levelsNeededForClickReturnIncrease
        + clickReturnOffset;
    updateHudObservers();
}
void MineShaftArea::updateHudObservers() const
{
    {
        auto obs = m_moneyPerClickObserver.lock();
        if (obs) {
            obs->notify(m_clickReturn);
        }
    }

    {
        auto obs = m_depthObserver.lock();
        if (obs) {
            obs->notify(m_mine_shaft_model.getCurrentDepth());
        }
    }
}

void MineShaftArea::flashActiveLayer() { getActiveLayer()->flash(); }

void MineShaftArea::setDescendHudObservers(
    std::shared_ptr<ObserverInterface<api::API const&>> moneyPerClickObserver,
    std::shared_ptr<ObserverInterface<api::API const&>> depthObserver)
{
    m_moneyPerClickObserver = moneyPerClickObserver;
    m_depthObserver = depthObserver;
}
