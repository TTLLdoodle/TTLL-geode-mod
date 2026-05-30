#include "Leaderboards.hpp"

namespace GlobalList::API {
    extern std::string API_URL;

    void getPlayerLeaderboard(size_t page) {
        spawn(web::WebRequest().get(fmt::format("{}/leaderboard/user/list?limit=50&offset={}", API_URL, 50 * page)),
            [page](web::WebResponse value) {
                if (!value.ok()) {
                    log::error("Failed to load GlobalList leaderboards. Failed code: {}", value.code());
                    Utils::failure(value.code(), "load GlobalList leaderboards");
                    return;
                }
                else {
                    auto data = value.json();

                    matjson::Value json = data.unwrapOrDefault();
                    if (!json.contains("data") || !json["data"].contains("users") || !json["data"]["users"].isArray() || json["data"]["users"].size() == 0) {
                        Utils::failure(value.code(), "load GlobalList leaderboards");
                        return;
                    }

                    for (const auto& user : json["data"]["users"]) {
                        int id = user["id"].asInt().unwrapOrDefault();
                        std::string username = user["username"].asString().unwrapOrDefault();
                        int placement = user["placement"].asInt().unwrapOrDefault();
                        double points = user["points"].asDouble().unwrapOrDefault();
                        std::string country = user["country"].asString().unwrapOrDefault();
                        std::string badge = user["badge"].asString().unwrapOrDefault();

                        UserData userData = {
                            id, user["username"].asString().unwrapOr(""),
                            placement, points, country, badge, {},
                            std::time(nullptr)
                        };
                        GlobalList::Cache::saveUser(username, userData);
                    }

                    LeaderboardsCachedEvent().send(page);
                }
            }
        );
    }

    void getCountryLeaderboard() {

    }

    void getCountryAltLeaderboard() {

    }
};