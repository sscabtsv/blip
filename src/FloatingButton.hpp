#pragma once
#include <Geode/Geode.hpp>

class DraggablePanel;

class FloatingButton : public cocos2d::CCLayer {
protected:
    cocos2d::CCSprite* m_bg = nullptr;
    DraggablePanel* m_panel = nullptr;
    bool m_dragging = false;
    float m_dragDistance = 0.f;
    cocos2d::CCPoint m_lastTouchPos;

    bool init();
    void togglePanel();

public:
    static FloatingButton* create();

    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
};
