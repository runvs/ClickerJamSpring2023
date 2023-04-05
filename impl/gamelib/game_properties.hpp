#ifndef GAME_GAMEPROPERTIES_HPP
#define GAME_GAMEPROPERTIES_HPP

#include <color/color.hpp>
#include <color/palette.hpp>
#include <vector.hpp>
#include <string>

class GP {
public:
    GP() = delete;

    static std::string GameName() { return "MyAwesomeGame"; }
    static std::string AuthorName() { return "TODO"; }
    static std::string JamName() { return "TODO"; }
    static std::string JamDate() { return "TODO"; }
    static std::string ExplanationText()
    {
        return "Win the game\n[W,A,S,D] to move \n[Space] to jump\n[M/U] to mute/unmute audio";
    }

    static jt::Vector2f GetWindowSize() { return jt::Vector2f { 1200, 810 }; }
    static float GetZoom() { return 3.0f; }
    static jt::Vector2f GetScreenSize() { return GetWindowSize() * (1.0f / GetZoom()); }

    static jt::Color PaletteBackground() { return GP::getPalette().getColor(5); }
    static jt::Color PaletteFontFront() { return GP::getPalette().getColor(0); }
    static jt::Color PalleteFrontHighlight() { return GP::getPalette().getColor(1); }
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

    static int PurchasedNumberOfMinersPerLine();

    static float PurchaseButtonRevealAtPercentage();
    static float PurchaseButtonCostIncreasePercent();
    static float HudMineShaftLayerHeight();
    static int PurchasedMaxNumberOfLines();
};

#endif
