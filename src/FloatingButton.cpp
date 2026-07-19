#include "FloatingButton.hpp"
#include "DraggablePanel.hpp"
#include "Theme.hpp"

using namespace geode::prelude;

namespace {
    constexpr float FAB_SIZE = 64.f;
}

bool FloatingButton::init() {
    if (!CCLayer::init()) return false;

    this->setContentSize({ FAB_SIZE, FAB_SIZE });
    this->setAnchorPoint({ 0.f, 0.f });
    this->ignoreAnchorPointForPosition(true);

    m_bg = CCSprite::createWithSpriteFrameName("fab.png"_spr);
    m_bg->setScale(FAB_SIZE / 128.f);
    m_bg->setPosition({ FAB_SIZE / 2.f, FAB_SIZE / 2.f });
    m_bg->setColor(theme::kAccent);
    this->addChild(m_bg);

    auto icon = CCSprite::createWithSpriteFrameName("icon-menu.png"_spr);
    icon->setScale(0.62f);
    icon->setPosition({ FAB_SIZE / 2.f, FAB_SIZE / 2.f });
    this->addChild(icon, 1);

    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);

    return true;
}

FloatingButton* FloatingButton::create() {
    auto ret = new FloatingButton();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool FloatingButton::ccTouchBegan(CCTouch* touch, CCEvent*) {
    auto loc = this->convertTouchToNodeSpace(touch);
    CCRect box(0.f, 0.f, this->getContentSize().width, this->getContentSize().height);
    if (!box.containsPoint(loc)) return false;

    m_dragging = true;
    m_dragDistance = 0.f;
    m_lastTouchPos = touch->getLocation();

    this->stopAllActions();
    this->runAction(CCScaleTo::create(0.08f, 1.08f));
    return true;
}

void FloatingButton::ccTouchMoved(CCTouch* touch, CCEvent*) {
    if (!m_dragging) return;
    auto cur = touch->getLocation();
    auto delta = cur - m_lastTouchPos;
    m_dragDistance += ccpLength(delta);
    this->setPosition(this->getPosition() + delta);
    m_lastTouchPos = cur;
}

void FloatingButton::ccTouchEnded(CCTouch*, CCEvent*) {
    m_dragging = false;
    this->stopAllActions();
    this->runAction(CCScaleTo::create(0.1f, 1.f));

    if (m_dragDistance < 6.f) {
        togglePanel();
    }
}

void FloatingButton::ccTouchCancelled(CCTouch*, CCEvent*) {
    m_dragging = false;
    this->stopAllActions();
    this->runAction(CCScaleTo::create(0.1f, 1.f));
}

void FloatingButton::togglePanel() {
    if (!m_panel) {
        m_panel = DraggablePanel::create();
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        m_panel->setPosition({ winSize.width / 2.f - 140.f, winSize.height / 2.f - 180.f });
        if (this->getParent()) {
            this->getParent()->addChild(m_panel, 1000000);
        }
    }

    if (m_panel->isVisible()) {
        m_panel->popOut();
    } else {
        m_panel->popIn();
    }
}
