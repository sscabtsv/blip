#pragma once
#include <Geode/Geode.hpp>
#include <functional>

class PremiumToggle : public cocos2d::CCLayer {
protected:
    cocos2d::CCSprite* m_track = nullptr;
    cocos2d::CCSprite* m_knob = nullptr;
    bool m_on = false;
    std::function<void(bool)> m_callback;

    bool init(bool startOn, std::function<void(bool)> callback);

public:
    static PremiumToggle* create(bool startOn, std::function<void(bool)> callback);

    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

    void setOn(bool on, bool animated);
    bool isOn() const { return m_on; }
};
