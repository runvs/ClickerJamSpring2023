#ifndef GAME_GAMEPROPERTIES_HPP
#define GAME_GAMEPROPERTIES_HPP

#include <color/color.hpp>
#include <color/palette.hpp>
#include <vector.hpp>
#include <string>

class GP {
public:
    GP() = delete;

    static std::string GameName() { return "Mines Of Gloria"; }
    static std::string AuthorName() { return "24io, Laguna, Thunraz"; }
    static std::string JamName() { return "ClickerJam Spring"; }
    static std::string JamDate() { return "2023"; }
    static std::string ExplanationText()
    {
        return "Dig Deep in the Mines\n[M/U] to mute/unmute audio";
    }

    static jt::Vector2f GetWindowSize() { return jt::Vector2f { 1200, 810 }; }
    static float GetZoom() { return 3.0f; }
    static jt::Vector2f GetScreenSize() { return GetWindowSize() * (1.0f / GetZoom()); }

    static jt::Color PaletteBackground() { return GP::getPalette().getColor(5); }
    static jt::Color PaletteFontFront() { return GP::getPalette().getColor(0); }
    static jt::Color PaletteFrontHighlight() { return GP::getPalette().getColor(1); }
    static jt::Color PaletteFontShadow() { return GP::getPalette().getColor(6); }
    static jt::Color PaletteFontCredits() { return GP::getPalette().getColor(2); }

    static jt::Palette getPalette();

    static int PhysicVelocityIterations();
    static int PhysicPositionIterations();

    static jt::Vector2u HudButtonSize();
    static jt::Vector2f HudMenuOffset();
    static jt::Vector2f HudMenuSize();
    static jt::Vector2f HudMenuMargin();
    static jt::Vector2f HudMineShaftSize();
    static jt::Vector2f HudMineShaftOffset();
    static jt::Vector2f HudMineShaftActiveLayerOffset();
    static jt::Vector2f HudMineShaftActiveLayerSize();

    static int PurchasedNumberOfObjectsPerLine();

    static float PurchaseButtonRevealAtPercentage();
    static float PurchaseButtonCostIncreasePercent();
    static float HudMineShaftLayerHeight();
    static int PurchasedMaxNumberOfLines();
};

#endif
