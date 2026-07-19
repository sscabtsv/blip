#pragma once
#include <Geode/Geode.hpp>
#include "Theme.hpp"

using namespace geode::prelude;

namespace ui {
    inline CCScale9Sprite* createPanelSprite(cocos2d::ccColor3B color, float w, float h) {
        auto s9 = CCScale9Sprite::createWithSpriteFrameName("panel.png"_spr);
        s9->setContentSize({ w, h });
        s9->setCapInsets(theme::panelCapInsets());
        s9->setColor(color);
        return s9;
    }

    inline cocos2d::CCMenuItemSpriteExtra* createButton(
        std::string const& text,
        cocos2d::ccColor3B bgColor,
        float width,
        float height,
        cocos2d::CCObject* target,
        cocos2d::SEL_MenuHandler selector
    ) {
        auto bg = createPanelSprite(bgColor, width, height);

        auto label = cocos2d::CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
        label->setScale(0.42f);
        label->setPosition({ width / 2.f, height / 2.f + 1.f });
        label->setColor(theme::kTextPrimary);
        bg->addChild(label);

        return cocos2d::CCMenuItemSpriteExtra::create(bg, target, selector);
    }

    // Briefly flashes a background sprite towards white and back, giving
    // click feedback beyond the built-in press/release scale animation.
    inline void flashFeedback(cocos2d::CCNodeRGBA* sprite) {
        if (!sprite) return;
        sprite->stopActionByTag(9001);
        auto flash = cocos2d::CCSequence::create(
            cocos2d::CCTintTo::create(theme::kAnimFast * 0.6f, 255, 255, 255),
            cocos2d::CCTintTo::create(theme::kAnimFast * 0.9f, sprite->getColor().r, sprite->getColor().g, sprite->getColor().b),
            nullptr
        );
        flash->setTag(9001);
        sprite->runAction(flash);
    }
}
