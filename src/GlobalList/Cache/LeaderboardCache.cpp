#include "LeaderboardCache.hpp"

namespace GlobalList::Cache {
    static std::unordered_map<std::string, LeaderboardUser> leaderboardUserCache;

    void saveLeaderboardUser(const std::string& username, const LeaderboardUser& userData) {
        leaderboardUserCache[username] = userData;
        UserCachedEvent(username).send();
    }

    LeaderboardUser* getLeaderboardUser(const std::string& username) {
        auto it = leaderboardUserCache.find(username);
        if (it == leaderboardUserCache.end()) return nullptr;

        auto now = std::time(nullptr);
        if (now - it->second.loadTime > 1800) return nullptr;

        return &it->second;
    }

    size_t leaderboardUserCacheSize() {
        return leaderboardUserCache.size();
    }

    void leaderboardUserCacheClear() {
        leaderboardUserCache.clear();
    }
}