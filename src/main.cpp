#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/Notification.hpp>

using namespace geode::prelude;

class MobileUIPopup : public Popup {
protected:
    bool init() {
        if (!Popup::init(300.f, 240.f)) return false;

        this->setTitle("Mobile UI Example");

        CCNode* panel = nullptr;
        if (auto s9 = CCScale9Sprite::create("GJ_square02.png")) {
            s9->setContentSize({ 250.f, 170.f });
            s9->setOpacity(120);
            panel = s9;
        } else {
            panel = CCLayerColor::create({ 0, 0, 0, 90 }, 250.f, 170.f);
        }
        m_mainLayer->addChildAtPosition(panel, Anchor::Center);

        auto menu = CCMenu::create();
        menu->setContentSize({ 230.f, 190.f });
        menu->setLayout(
            ColumnLayout::create()
                ->setGap(10.f)
                ->setAutoScale(true)
        );

        auto accentBar = CCLayerColor::create({ 255, 203, 0, 255 }, 200.f, 3.f);
        menu->addChild(accentBar);

        auto desc = CCLabelBMFont::create(
            "Large, touch-friendly controls that wrap cleanly across multiple lines instead of running off the edge.",
            "chatFont.fnt",
            210.f,
            kCCTextAlignmentCenter
        );
        desc->setScale(0.6f);
        menu->addChild(desc);

        auto buttonSprite = ButtonSprite::create("Tap Me", 100, true, "bigFont.fnt", "GJ_button_04.png", 30.f, 0.9f);
        auto button = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(MobileUIPopup::onTap)
        );
        menu->addChild(button);

        auto toggler = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(MobileUIPopup::onToggle),
            1.0f
        );
        menu->addChild(toggler);

        menu->updateLayout();
        m_mainLayer->addChildAtPosition(menu, Anchor::Center);

        return true;
    }

    void onTap(CCObject*) {
        Notification::create("You tapped the button!", NotificationIcon::Success, 1.f)->show();
    }

    void onToggle(CCObject* sender) {
        auto toggler = static_cast<CCMenuItemToggler*>(sender);
        toggler->toggle(!toggler->isToggled());
    }

public:
    static MobileUIPopup* create() {
        auto ret = new MobileUIPopup();
        if (ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

class $modify(MobileUIPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto spr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
        spr->setScale(0.9f);

        auto btn = CCMenuItemSpriteExtra::create(
            spr,
            this,
            menu_selector(MobileUIPauseLayer::onMobileUIExample)
        );
        btn->setID("mobile-ui-example-button"_spr);

        auto menu = this->getChildByID("right-button-menu");
        if (menu) {
            menu->addChild(btn);
            menu->updateLayout();
        } else {
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            auto fallbackMenu = CCMenu::create();
            fallbackMenu->setID("mobile-ui-example-fallback-menu"_spr);
            fallbackMenu->addChild(btn);
            fallbackMenu->setPosition({ winSize.width - 30.f, winSize.height - 30.f });
            this->addChild(fallbackMenu);
        }
    }

    void onMobileUIExample(CCObject*) {
        MobileUIPopup::create()->show();
    }
};
