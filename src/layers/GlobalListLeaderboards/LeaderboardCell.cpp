#include "LeaderboardCell.hpp"

LeaderboardCell* LeaderboardCell::create(const UserData& userCache) {
	auto ret = new LeaderboardCell();
	if (ret && ret->init(userCache)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool LeaderboardCell::init(const UserData& userCache) {
    if (!CCNode::init()) return false;



    return true;
}