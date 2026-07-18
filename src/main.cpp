#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/Notification.hpp>

using namespace geode::prelude;

class MobileUIPopup : public Popup {
protected:
    bool init() {
        if (!Popup::init(280.f, 220.f)) return false;

        this->setTitle("Mobile UI Example");

        auto menu = CCMenu::create();
        menu->setContentSize({ 220.f, 130.f });
        menu->setAnchorPoint({ 0.5f, 0.5f });
        menu->setLayout(
            ColumnLayout::create()
                ->setGap(12.f)
                ->setAutoScale(true)
        );

        auto label = CCLabelBMFont::create("Large, touch-friendly controls", "bigFont.fnt");
        label->setScale(0.35f);
        menu->addChild(label);

        auto buttonSprite = ButtonSprite::create("Tap Me");
        buttonSprite->setScale(1.15f);
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

class $modify(MobileUIMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID("bottom-menu");
        if (menu) {
            auto spr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
            spr->setScale(0.9f);

            auto btn = CCMenuItemSpriteExtra::create(
                spr,
                this,
                menu_selector(MobileUIMenuLayer::onMobileUIExample)
            );
            btn->setID("mobile-ui-example-button"_spr);

            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }

    void onMobileUIExample(CCObject*) {
        MobileUIPopup::create()->show();
    }
};
