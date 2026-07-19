#pragma once
#include <Geode/Geode.hpp>

class DraggablePanel : public cocos2d::CCLayer {
protected:
    cocos2d::CCNode* m_generalTab = nullptr;
    cocos2d::CCNode* m_displayTab = nullptr;
    cocos2d::CCNode* m_aboutTab = nullptr;
    cocos2d::CCNode* m_activeTab = nullptr;
    cocos2d::CCNode* m_tabIndicator = nullptr;
    cocos2d::CCLabelBMFont* m_tabLabels[3] = { nullptr, nullptr, nullptr };
    cocos2d::CCPoint m_tabIndicatorPositions[3];

    bool init() override;
    cocos2d::CCNode* buildGeneralTab(float w, float h);
    cocos2d::CCNode* buildDisplayTab(float w, float h);
    cocos2d::CCNode* buildAboutTab(float w, float h);

    void switchTab(int index);
    void onTab0(cocos2d::CCObject*);
    void onTab1(cocos2d::CCObject*);
    void onTab2(cocos2d::CCObject*);
    void onClose(cocos2d::CCObject*);
    void onAction(cocos2d::CCObject*);
    void onPopOutFinished();

public:
    static DraggablePanel* create();
    void popIn();
    void popOut();
};
