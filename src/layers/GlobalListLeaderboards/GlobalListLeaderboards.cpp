#include "GlobalListLeaderboards.hpp"

GlobalListLeaderboards* GlobalListLeaderboards::create() {
	auto ret = new GlobalListLeaderboards();
	if (ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

CCScene* GlobalListLeaderboards::scene() {
	auto ret = CCScene::create();
	AppDelegate::get()->m_runningScene = ret;
	ret->addChild(GlobalListLeaderboards::create());
	return ret;
}

bool GlobalListLeaderboards::init() {
    if (!CCLayer::init()) return false;
    setID("GlobalListLeaderboards");

    auto winSize = CCDirector::get()->getWinSize();

	auto leaderboardBG = CCSprite::create("player-leaderboard-bg.png"_spr);
	leaderboardBG->setAnchorPoint({ 0.5f, 0.5f });
	leaderboardBG->setScale(winSize.height / leaderboardBG->getTextureRect().size.height);
	if (winSize.width > leaderboardBG->getContentWidth() * leaderboardBG->getScale()) {
		leaderboardBG->setScale(winSize.width / leaderboardBG->getTextureRect().size.width);
	}
	leaderboardBG->setPosition({ winSize.width / 2, winSize.height / 2 });
	leaderboardBG->setZOrder(0);
	leaderboardBG->setID("gdl-backgrownd");
	addChild(leaderboardBG);

    m_leaderboardList = GJListLayer::create(
		nullptr, nullptr,
		{ 0, 0, 0, 180 },
		356.0f, 220.0f, 0);
	m_leaderboardList->setPosition(winSize / 2.0f - m_leaderboardList->getContentSize() / 2.0f - ccp(0, 5));
	m_leaderboardList->setColor({ 194, 114, 62 });
	m_leaderboardList->setID("GJListLayer");
	addChild(m_leaderboardList, 5);

    auto btnsMenu = CCMenu::create();
	btnsMenu->setPosition({ 0.0f, 0.0f });
	btnsMenu->setID("buttons-menu");
	addChild(btnsMenu, 2);

    auto backButton = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_arrow_01_001.png", 1.0f, [this](auto) {
		CCDirector::get()->popSceneWithTransition(0.5f, kPopTransitionFade);
	});
	backButton->setPosition({ 25.0f, winSize.height - 25.0f });
	backButton->setID("back-button");
	btnsMenu->addChild(backButton);

	m_populateListener = LeaderboardsCachedEvent().listen(
		[this](int page) {
			for (int i = 0; i < 10; i++) {
				UserData userData = {
					1234, "Zoink",
					i+1, 101*(10 - i), "RU", "PC", {},
					std::time(nullptr)
				};

				auto cell = LeaderboardCell::create(userData);
				m_leaderboardList->m_listView->m_tableView->m_contentLayer->addChild(cell);
				m_leaderboardList->m_listView->m_tableView->m_contentLayer->updateLayout();
			}
		}
	);

    return true;
}

void GlobalListLeaderboards::keyDown(enumKeyCodes key, double d) {
	switch (key) {
	case KEY_Left:
	case CONTROLLER_Left:
		break;
	case KEY_Right:
	case CONTROLLER_Right:
		break;
	case KEY_Enter:
		break;
	default:
		CCLayer::keyDown(key, d);
		break;
	}
}

void GlobalListLeaderboards::keyBackClicked() {
	CCDirector::get()->popSceneWithTransition(0.5f, kPopTransitionFade);
}