#include "game_properties.hpp"
#include <color/palette_builder.hpp>

namespace {

jt::Palette createPalette()
{
    jt::PaletteBuilder builder;
    return builder
        .addColorsFromGPL(R"(GIMP Palette
#Palette Name: Dreamscape8
#Description: A palette made of low saturation colours to give your art a dreamlike quality.
#Colors: 8
201	204	161	c9cca1
202	160	90	caa05a
174	106	71	ae6a47
139	64	73	8b4049
84	51	68	543344
81	82	98	515262
99	120	125	63787d
142	160	145	8ea091
)")
        .create();
}

} // namespace

jt::Palette GP::getPalette()
{
    static auto const p = createPalette();
    return p;
}

int GP::PhysicVelocityIterations() { return 20; }

int GP::PhysicPositionIterations() { return 20; }
jt::Vector2u GP::HudButtonSize() { return jt::Vector2u { 120, 20 }; }
jt::Vector2f GP::HudMenuOffset() { return jt::Vector2f { GetScreenSize().x / 3.0f * 2.0f, 0.0f }; }
jt::Vector2f GP::HudMenuMargin() { return jt::Vector2f { 7.0f, 5.0f }; }
jt::Vector2f GP::HudMenuSize()
{
    return jt::Vector2f { static_cast<float>(static_cast<int>(GetScreenSize().x / 3.0f)) + 1.0f,
        GP::GetScreenSize().y };
}
jt::Vector2f GP::HudMineShaftOffset()
{
    return jt::Vector2f { GetScreenSize().x / 3.0f * 1.0f, 0.0f };
};
jt::Vector2f GP::HudMineShaftSize()
{
    return jt::Vector2f { static_cast<float>(static_cast<int>(GetScreenSize().x / 3.0f)) + 1.0f,
        GP::GetScreenSize().y };
};

int GP::PurchasedNumberOfMinersPerLine() { return 12; }
float GP::PurchaseButtonRevealAtPercentage() { return 0.85f; }
float GP::PurchaseButtonCostIncreasePercent() { return 1.2f; }
float GP::HudMineShaftLayerHeight() { return 16.0f; }
int GP::PurchasedMaxNumberOfLines() { return 4; }
