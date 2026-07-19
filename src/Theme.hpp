#pragma once
#include <Geode/Geode.hpp>

namespace theme {
    constexpr cocos2d::ccColor3B kBackground = { 22, 24, 35 };
    constexpr cocos2d::ccColor3B kSurface = { 33, 36, 52 };
    constexpr cocos2d::ccColor3B kSurfaceAlt = { 42, 46, 66 };
    constexpr cocos2d::ccColor3B kAccent = { 124, 92, 252 };
    constexpr cocos2d::ccColor3B kAccentDim = { 94, 72, 190 };
    constexpr cocos2d::ccColor3B kTextPrimary = { 240, 241, 247 };
    constexpr cocos2d::ccColor3B kTextSecondary = { 150, 156, 178 };
    constexpr cocos2d::ccColor3B kTrackOff = { 58, 62, 82 };
    constexpr cocos2d::ccColor3B kDanger = { 235, 96, 96 };
    constexpr cocos2d::ccColor3B kWhite = { 255, 255, 255 };

    constexpr float kSpacingXS = 4.f;
    constexpr float kSpacingS = 8.f;
    constexpr float kSpacingM = 14.f;
    constexpr float kSpacingL = 20.f;

    // Cap insets for panel.png (128x128 source, 26px corner radius baked in)
    inline cocos2d::CCRect panelCapInsets() {
        return { 38.f, 38.f, 52.f, 52.f };
    }

    constexpr float kAnimFast = 0.12f;
    constexpr float kAnimMed  = 0.22f;
    constexpr float kAnimSlow = 0.32f;
}
