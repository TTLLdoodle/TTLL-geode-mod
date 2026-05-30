#pragma once

#include <Geode/Geode.hpp>

#include "../../GlobalList/Cache/UserCache.hpp"

class LeaderboardCell : public CCNode {
public:
    static LeaderboardCell* create(const UserData& userCache);
protected:
    bool init(const UserData& userCache);
};