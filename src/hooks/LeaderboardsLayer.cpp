#include <Geode/modify/LeaderboardsLayer.hpp>
#include "../layers/GlobalListLeaderboards/GlobalListLeaderboards.hpp"

class $modify(MyLeaderboardsLayer, LeaderboardsLayer) {
    bool init(LeaderboardType type, LeaderboardStat stat) {
        if (!LeaderboardsLayer::init(type, stat)) return false;
        
        auto bottomRightMenu = getChildByID("bottom-right-menu");

        auto globalListSpr = CircleButtonSprite::createWithSprite("global-list.png"_spr);
        globalListSpr->getTopNode()->setScale(1.1f);
        globalListSpr->setScale(0.8f);
        auto globalListBtn = CCMenuItemSpriteExtra::create(globalListSpr, this, menu_selector(MyLeaderboardsLayer::onBtn));
        globalListBtn->setID("global-list-button");

        bottomRightMenu->addChild(globalListBtn);
        bottomRightMenu->updateLayout();

        return true;
    }

    void onBtn(CCObject* sender) {
        CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, GlobalListLeaderboards::scene()));
    }
};