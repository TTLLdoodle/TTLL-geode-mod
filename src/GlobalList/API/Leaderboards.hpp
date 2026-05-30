#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

#include "../Cache/LeaderboardCache.hpp"
#include "../Cache/UserCache.hpp"
#include "../../events/LeaderboardCachedEvent.hpp"
#include "../../Utils.hpp"

namespace GlobalList::API {
    void getPlayerLeaderboard(size_t page = 0);
    void getCountryLeaderboard();
    void getCountryAltLeaderboard();
};