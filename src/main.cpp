#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "FloatingButton.hpp"

using namespace geode::prelude;

namespace {
    void spawnFloatingButton(CCLayer* layer) {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto fab = FloatingButton::create();
        fab->setPosition({ winSize.width - 70.f, 90.f });
        layer->addChild(fab, 1000000);
    }
}

class $modify(MobileUIMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        spawnFloatingButton(this);
        return true;
    }
};

class $modify(MobileUIPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        spawnFloatingButton(this);
        return true;
    }
};
