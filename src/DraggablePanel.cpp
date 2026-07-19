#include "DraggablePanel.hpp"
#include "PremiumToggle.hpp"
#include "PremiumButton.hpp"
#include "Theme.hpp"
#include <Geode/ui/Notification.hpp>

using namespace geode::prelude;

namespace {
    constexpr float PANEL_W = 280.f;
    constexpr float PANEL_H = 360.f;
    constexpr float HEADER_H = 46.f;
    constexpr float TABS_H = 40.f;
    constexpr float CONTENT_PAD = 16.f;
    constexpr float CONTENT_W = PANEL_W - CONTENT_PAD * 2.f;
    constexpr float CONTENT_H = PANEL_H - HEADER_H - TABS_H - CONTENT_PAD;

    class PanelHeader : public CCLayer {
    protected:
        CCNode* m_dragTarget = nullptr;
        CCPoint m_lastTouchPos;
        bool m_dragging = false;

        bool init(CCNode* dragTarget, CCSize size) {
            if (!CCLayer::init()) return false;
            m_dragTarget = dragTarget;
            this->setContentSize(size);
            this->setAnchorPoint({ 0.f, 0.f });
            this->ignoreAnchorPointForPosition(true);
            this->setTouchEnabled(true);
            this->setTouchMode(kCCTouchesOneByOne);
            return true;
        }

    public:
        static PanelHeader* create(CCNode* dragTarget, CCSize size) {
            auto ret = new PanelHeader();
            if (ret->init(dragTarget, size)) {
                ret->autorelease();
                return ret;
            }
            delete ret;
            return nullptr;
        }

        bool ccTouchBegan(CCTouch* touch, CCEvent*) override {
            auto loc = this->convertTouchToNodeSpace(touch);
            CCRect box(0.f, 0.f, this->getContentSize().width, this->getContentSize().height);
            if (!box.containsPoint(loc)) return false;
            m_dragging = true;
            m_lastTouchPos = touch->getLocation();
            return true;
        }

        void ccTouchMoved(CCTouch* touch, CCEvent*) override {
            if (!m_dragging || !m_dragTarget) return;
            auto cur = touch->getLocation();
            auto delta = cur - m_lastTouchPos;
            m_dragTarget->setPosition(m_dragTarget->getPosition() + delta);
            m_lastTouchPos = cur;
        }

        void ccTouchEnded(CCTouch*, CCEvent*) override {
            m_dragging = false;
        }

        void ccTouchCancelled(CCTouch*, CCEvent*) override {
            m_dragging = false;
        }
    };
}

bool DraggablePanel::init() {
    if (!CCLayer::init()) return false;

    this->setContentSize({ PANEL_W, PANEL_H });
    this->setAnchorPoint({ 0.f, 0.f });
    this->ignoreAnchorPointForPosition(true);

    auto bg = ui::createPanelSprite(theme::kSurface, PANEL_W, PANEL_H);
    bg->setPosition({ PANEL_W / 2.f, PANEL_H / 2.f });
    this->addChild(bg);

    auto header = PanelHeader::create(this, { PANEL_W, HEADER_H });
    header->setPosition({ 0.f, PANEL_H - HEADER_H });
    this->addChild(header, 10);

    auto headerBg = ui::createPanelSprite(theme::kSurfaceAlt, PANEL_W, HEADER_H);
    headerBg->setPosition({ PANEL_W / 2.f, HEADER_H / 2.f });
    header->addChild(headerBg);

    auto title = CCLabelBMFont::create("Mod Menu", "bigFont.fnt");
    title->setScale(0.5f);
    title->setAnchorPoint({ 0.f, 0.5f });
    title->setPosition({ CONTENT_PAD, HEADER_H / 2.f });
    title->setColor(theme::kTextPrimary);
    header->addChild(title);

    auto closeMenu = CCMenu::create();
    closeMenu->setPosition({ 0.f, 0.f });
    auto closeSprite = CCSprite::createWithSpriteFrameName("icon-close.png"_spr);
    closeSprite->setScale(0.55f);
    closeSprite->setColor(theme::kTextSecondary);
    auto closeBtn = CCMenuItemSpriteExtra::create(closeSprite, this, menu_selector(DraggablePanel::onClose));
    closeBtn->setPosition({ PANEL_W - CONTENT_PAD, HEADER_H / 2.f });
    closeMenu->addChild(closeBtn);
    header->addChild(closeMenu, 5);

    float tabsY = PANEL_H - HEADER_H - TABS_H;
    auto tabsMenu = CCMenu::create();
    tabsMenu->setPosition({ 0.f, tabsY });
    this->addChild(tabsMenu, 10);

    const char* tabNames[3] = { "General", "Display", "About" };
    float tabW = PANEL_W / 3.f;

    m_tabIndicator = ui::createPanelSprite(theme::kAccent, tabW - 24.f, 3.f);

    for (int i = 0; i < 3; i++) {
        auto label = CCLabelBMFont::create(tabNames[i], "bigFont.fnt");
        label->setScale(0.38f);
        label->setColor(i == 0 ? theme::kTextPrimary : theme::kTextSecondary);
        m_tabLabels[i] = label;

        cocos2d::SEL_MenuHandler selector = i == 0
            ? menu_selector(DraggablePanel::onTab0)
            : i == 1 ? menu_selector(DraggablePanel::onTab1) : menu_selector(DraggablePanel::onTab2);

        auto item = CCMenuItemSpriteExtra::create(label, this, selector);
        item->setPosition({ tabW * i + tabW / 2.f, TABS_H / 2.f });
        tabsMenu->addChild(item);

        m_tabIndicatorPositions[i] = CCPoint(tabW * i + tabW / 2.f, 3.f);
    }

    m_tabIndicator->setPosition(m_tabIndicatorPositions[0]);
    tabsMenu->addChild(m_tabIndicator, -1);

    auto contentRoot = CCNode::create();
    contentRoot->setPosition({ CONTENT_PAD, CONTENT_PAD });
    contentRoot->setAnchorPoint({ 0.f, 0.f });
    this->addChild(contentRoot);

    m_generalTab = buildGeneralTab(CONTENT_W, CONTENT_H);
    m_displayTab = buildDisplayTab(CONTENT_W, CONTENT_H);
    m_aboutTab = buildAboutTab(CONTENT_W, CONTENT_H);

    contentRoot->addChild(m_generalTab);
    contentRoot->addChild(m_displayTab);
    contentRoot->addChild(m_aboutTab);

    m_displayTab->setVisible(false);
    m_aboutTab->setVisible(false);
    m_activeTab = m_generalTab;

    this->setScale(0.85f);
    this->setVisible(false);

    return true;
}

CCNode* DraggablePanel::buildGeneralTab(float w, float h) {
    auto root = CCNode::create();
    root->setContentSize({ w, h });
    root->setAnchorPoint({ 0.f, 0.f });

    auto desc = CCLabelBMFont::create(
        "Welcome to the redesigned menu. Everything here is built from scratch with custom art and animation.",
        "chatFont.fnt", w, kCCTextAlignmentLeft
    );
    desc->setScale(0.42f);
    desc->setAnchorPoint({ 0.f, 1.f });
    desc->setPosition({ 0.f, h });
    desc->setColor(theme::kTextSecondary);
    root->addChild(desc);

    float rowY = h - 100.f;

    auto toggleLabel = CCLabelBMFont::create("Enable Feature", "bigFont.fnt");
    toggleLabel->setScale(0.4f);
    toggleLabel->setAnchorPoint({ 0.f, 0.5f });
    toggleLabel->setColor(theme::kTextPrimary);
    toggleLabel->setPosition({ 0.f, rowY });
    root->addChild(toggleLabel);

    auto toggle = PremiumToggle::create(true, [](bool on) {
        Notification::create(on ? "Feature enabled" : "Feature disabled", NotificationIcon::Success, 1.f)->show();
    });
    toggle->setPosition({ w - 78.f, rowY - 21.f });
    root->addChild(toggle);

    auto btnMenu = CCMenu::create();
    btnMenu->setPosition({ 0.f, 0.f });
    auto btn = ui::createButton("Trigger Action", theme::kAccent, w, 44.f, this, menu_selector(DraggablePanel::onAction));
    btn->setPosition({ w / 2.f, 30.f });
    btnMenu->addChild(btn);
    root->addChild(btnMenu);

    return root;
}

CCNode* DraggablePanel::buildDisplayTab(float w, float h) {
    auto root = CCNode::create();
    root->setContentSize({ w, h });
    root->setAnchorPoint({ 0.f, 0.f });

    struct Row { const char* label; bool startOn; };
    Row rows[2] = {
        { "Show FPS Counter", false },
        { "Reduce Motion", false }
    };

    float rowY = h - 30.f;
    for (auto& row : rows) {
        auto label = CCLabelBMFont::create(row.label, "bigFont.fnt");
        label->setScale(0.4f);
        label->setAnchorPoint({ 0.f, 0.5f });
        label->setColor(theme::kTextPrimary);
        label->setPosition({ 0.f, rowY });
        root->addChild(label);

        auto toggle = PremiumToggle::create(row.startOn, nullptr);
        toggle->setPosition({ w - 78.f, rowY - 21.f });
        root->addChild(toggle);

        rowY -= 64.f;
    }

    auto note = CCLabelBMFont::create(
        "These settings are placeholders for this example.",
        "chatFont.fnt", w, kCCTextAlignmentLeft
    );
    note->setScale(0.36f);
    note->setAnchorPoint({ 0.f, 0.f });
    note->setColor(theme::kTextSecondary);
    note->setPosition({ 0.f, 8.f });
    root->addChild(note);

    return root;
}

CCNode* DraggablePanel::buildAboutTab(float w, float h) {
    auto root = CCNode::create();
    root->setContentSize({ w, h });
    root->setAnchorPoint({ 0.f, 0.f });

    auto name = CCLabelBMFont::create("Mobile UI Example", "bigFont.fnt");
    name->setScale(0.5f);
    name->setAnchorPoint({ 0.f, 1.f });
    name->setPosition({ 0.f, h });
    name->setColor(theme::kTextPrimary);
    root->addChild(name);

    auto version = CCLabelBMFont::create("v2.0.0", "chatFont.fnt");
    version->setScale(0.4f);
    version->setAnchorPoint({ 0.f, 1.f });
    version->setPosition({ 0.f, h - 24.f });
    version->setColor(theme::kAccent);
    root->addChild(version);

    auto desc = CCLabelBMFont::create(
        "A ground-up custom UI redesign built entirely with Geode: a persistent floating button, a draggable window, animated toggles, and premium-styled buttons.",
        "chatFont.fnt", w, kCCTextAlignmentLeft
    );
    desc->setScale(0.4f);
    desc->setAnchorPoint({ 0.f, 1.f });
    desc->setPosition({ 0.f, h - 48.f });
    desc->setColor(theme::kTextSecondary);
    root->addChild(desc);

    return root;
}

void DraggablePanel::switchTab(int index) {
    CCNode* target = m_generalTab;
    if (index == 1) target = m_displayTab;
    if (index == 2) target = m_aboutTab;

    if (target == m_activeTab) return;

    m_activeTab->setVisible(false);
    target->setVisible(true);
    target->setScale(0.94f);
    target->stopAllActions();
    target->runAction(CCEaseBackOut::create(CCScaleTo::create(theme::kAnimMed, 1.f)));
    m_activeTab = target;

    m_tabIndicator->stopAllActions();
    m_tabIndicator->runAction(CCEaseSineInOut::create(CCMoveTo::create(theme::kAnimMed, m_tabIndicatorPositions[index])));

    for (int i = 0; i < 3; i++) {
        if (m_tabLabels[i]) {
            m_tabLabels[i]->setColor(i == index ? theme::kTextPrimary : theme::kTextSecondary);
        }
    }
}

void DraggablePanel::onTab0(CCObject*) { switchTab(0); }
void DraggablePanel::onTab1(CCObject*) { switchTab(1); }
void DraggablePanel::onTab2(CCObject*) { switchTab(2); }

void DraggablePanel::onClose(CCObject*) {
    popOut();
}

void DraggablePanel::onAction(CCObject* sender) {
    if (auto item = static_cast<CCMenuItemSpriteExtra*>(sender)) {
        ui::flashFeedback(static_cast<CCNodeRGBA*>(item->getNormalImage()));
    }
    Notification::create("Action triggered!", NotificationIcon::Success, 1.f)->show();
}

void DraggablePanel::popIn() {
    this->setVisible(true);
    this->stopAllActions();
    this->setScale(0.85f);
    this->runAction(CCEaseBackOut::create(CCScaleTo::create(theme::kAnimSlow, 1.f)));
}

void DraggablePanel::popOut() {
    this->stopAllActions();
    auto scaleDown = CCScaleTo::create(theme::kAnimMed, 0.85f);
    auto hide = CCCallFunc::create(this, callfunc_selector(DraggablePanel::onPopOutFinished));
    this->runAction(CCSequence::create(scaleDown, hide, nullptr));
}

void DraggablePanel::onPopOutFinished() {
    this->setVisible(false);
}

DraggablePanel* DraggablePanel::create() {
    auto ret = new DraggablePanel();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
