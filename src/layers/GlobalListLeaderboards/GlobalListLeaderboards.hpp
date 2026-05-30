#pragma once

#include "../../GlobalList/API/Levels.hpp"
#include "../../GlobalList/Filters.hpp"
#include "../../GlobalList/Levels.hpp"

#include "../../events/LeaderboardCachedEvent.hpp"

#include "LeaderboardCell.hpp"

using namespace geode::prelude;

class GlobalListLeaderboards : public cocos2d::CCLayer {
public:
	static GlobalListLeaderboards* create();
	static cocos2d::CCScene* scene();

	void keyDown(cocos2d::enumKeyCodes, double d) override;
	void keyBackClicked() override;

protected:
	ListenerHandle m_populateListener;
    GJListLayer* m_leaderboardList;
    std::vector<TabButton*> m_tabButtons;

	bool init();
};