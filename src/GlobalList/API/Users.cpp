#include "Users.hpp"

namespace GlobalList::API {
    extern std::string API_URL;

    void getUserRecords(int userID, std::string username);

    void getUser(std::string username, bool loadRecords) {
        spawn(web::WebRequest().get(fmt::format("{}/leaderboard/user/list?search={}", API_URL, username)),
            [username, loadRecords](web::WebResponse value) {
                if (!value.ok()) {
                    log::error("Failed to get player info. Failed code: {}", value.code());
                    Utils::failure(value.code(), "get player info");
                    return;
                }

                auto data = value.json();

                matjson::Value json = data.unwrapOrDefault();
                if (!json.contains("data") || !json["data"].contains("users") || !json["data"]["users"].isArray() || json["data"]["users"].size() == 0) {
                    Utils::failure(value.code(), "get player info");
                    return;
                }

                for (const auto& user : json["data"]["users"]) {
                    auto lowerUsername = geode::utils::string::toLower(username);
                    auto lowerUnwrapedUsername = geode::utils::string::toLower(user["username"].asString().unwrapOr(""));

                    if (lowerUnwrapedUsername == lowerUsername) {
                        int id = user["id"].asInt().unwrapOr(0);
                        int placement = user["placement"].asInt().unwrapOr(0);
                        double points = user["points"].asDouble().unwrapOr(0.0f);
                        std::string country = user["country"].asString().unwrapOrDefault();
                        std::string badge = user["badge"].asString().unwrapOrDefault();

                        auto userData = UserData{
                            id, user["username"].asString().unwrapOr(""),
                            placement, points, country, badge, {},
                            std::time(nullptr)
                        };
                        GlobalList::Cache::saveUser(username, userData);

                        if (loadRecords) Loader::get()->queueInMainThread([id, username]() { getUserRecords(id, username); });
                    }
                }
            }
        );
    }

    void getUserRecords(int userID, std::string username) {
        spawn(web::WebRequest().get(fmt::format("{}/user/record/list?user_id={}", API_URL, userID)),
            [username](web::WebResponse value) {
                if (!value.ok()) {
                    log::error("Failed to get player records. Failed code: {}", value.code());
                    Utils::failure(value.code(), "get player records");
                    return;
                }

                auto data = value.json();

                matjson::Value json = data.unwrapOrDefault();
                if (!json.contains("data") || !json["data"].contains("records") || !json["data"]["records"].isArray() || json["data"]["records"].size() == 0) {
                    Utils::failure(value.code(), "get player records");
                    return;
                }

                if (auto userData = GlobalList::Cache::getUser(username)) {
                    for (const auto& record : json["data"]["records"]) {
                        std::string name = record["level"]["name"].asString().unwrapOrDefault();
                        int levelID = record["level"]["ingame_id"].asInt().unwrapOrDefault();
                        int placement = record["level"]["placement"].asInt().unwrapOrDefault();

                        if (levelID != 0) userData->records.push_back( {name, levelID, placement} );
                        if (levelID != 0) log::info("{} | {} | {}", name, levelID, placement);

                        UserCachedEvent(username).send();
                    }
                }
            }
        );
    }
};