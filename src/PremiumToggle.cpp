#include "PremiumToggle.hpp"
#include "Theme.hpp"

using namespace geode::prelude;

namespace {
    constexpr float kTrackW = 78.f;
    constexpr float kTrackH = 42.f;
    constexpr float kKnobScale = 0.5f;
    constexpr float kKnobY = 21.f;
    constexpr float kKnobOffX = 21.f;
    constexpr float kKnobOnX = 57.f;
}

bool PremiumToggle::init(bool startOn, std::function<void(bool)> callback) {
    if (!CCLayer::init()) return false;

    m_callback = callback;
    m_on = startOn;

    this->setContentSize({ kTrackW, kTrackH });
    this->setAnchorPoint({ 0.f, 0.f });
    this->ignoreAnchorPointForPosition(true);

    m_track = CCSprite::createWithSpriteFrameName("track.png"_spr);
    m_track->setAnchorPoint({ 0.f, 0.f });
    m_track->setPosition({ 0.f, 0.f });
    m_track->setColor(m_on ? theme::kAccent : theme::kTrackOff);
    this->addChild(m_track);

    m_knob = CCSprite::createWithSpriteFrameName("knob.png"_spr);
    m_knob->setScale(kKnobScale);
    m_knob->setPosition({ m_on ? kKnobOnX : kKnobOffX, kKnobY });
    this->addChild(m_knob);

    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);

    return true;
}

PremiumToggle* PremiumToggle::create(bool startOn, std::function<void(bool)> callback) {
    auto ret = new PremiumToggle();
    if (ret->init(startOn, callback)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool PremiumToggle::ccTouchBegan(CCTouch* touch, CCEvent*) {
    auto loc = this->convertTouchToNodeSpace(touch);
    CCRect box(0.f, 0.f, this->getContentSize().width, this->getContentSize().height);
    return box.containsPoint(loc);
}

void PremiumToggle::ccTouchMoved(CCTouch*, CCEvent*) {}

void PremiumToggle::ccTouchEnded(CCTouch*, CCEvent*) {
    setOn(!m_on, true);
    if (m_callback) m_callback(m_on);
}

void PremiumToggle::ccTouchCancelled(CCTouch*, CCEvent*) {}

void PremiumToggle::setOn(bool on, bool animated) {
    m_on = on;
    auto targetColor = m_on ? theme::kAccent : theme::kTrackOff;
    float targetX = m_on ? kKnobOnX : kKnobOffX;

    if (animated) {
        m_track->stopAllActions();
        m_track->runAction(CCTintTo::create(theme::kAnimMed, targetColor.r, targetColor.g, targetColor.b));

        m_knob->stopAllActions();
        auto move = CCEaseSineInOut::create(CCMoveTo::create(theme::kAnimMed, { targetX, kKnobY }));
        auto squish = CCSequence::create(
            CCScaleTo::create(theme::kAnimFast * 0.5f, kKnobScale * 1.18f),
            CCScaleTo::create(theme::kAnimFast * 0.9f, kKnobScale),
            nullptr
        );
        m_knob->runAction(move);
        m_knob->runAction(squish);
    } else {
        m_track->setColor(targetColor);
        m_knob->setPosition({ targetX, kKnobY });
    }
}
