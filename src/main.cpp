#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/SceneManager.hpp>
#include "FloatingButton.hpp"

using namespace geode::prelude;

class $modify(MobileUIMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        static bool s_created = false;
        if (!s_created) {
            s_created = true;

            auto winSize = CCDirector::sharedDirector()->getWinSize();
            auto fab = FloatingButton::create();
            fab->setPosition({ winSize.width - 70.f, 90.f });

            this->addChild(fab, 1000000);
            SceneManager::get()->keepAcrossScenes(fab);
        }

        return true;
    }
};
