#include "Levels.hpp"

namespace GlobalList::API {
    std::string API_URL = "https://api.demonlist.org";

    void getDemonlist() {
        spawn(web::WebRequest().get(fmt::format("{}/level/classic/list", API_URL)),
            [](web::WebResponse value) {
                if (!value.ok()) {
                    log::error("Failed to load GlobalList. Failed code: {}", value.code());
                    Utils::failure(value.code(), "load GlobalList");
                    return;
                }

                GlobalList::Levels::clear();
                auto data = value.json();

                matjson::Value json = data.unwrapOrDefault();
                if (!json.contains("data") || !json["data"].contains("levels") || !json["data"]["levels"].isArray() || json["data"]["levels"].size() == 0) {
                    Utils::failure(value.code(), "load GlobalList");
                    return;
                }

                for (const auto& level : json["data"]["levels"]) {
                    int id = level["id"].asInt().unwrapOrDefault();
                    int levelID = level["ingame_id"].asInt().unwrapOrDefault();
                    std::string name = level["name"].asString().unwrapOrDefault();
                    int placement = level["placement"].asInt().unwrapOrDefault();
                    int length = level["length"].asInt().unwrapOrDefault();
                    GlobalListLevel newLevel = { id, levelID, name, placement, length };

                    if (id == 0) {
                        Utils::failure(value.code(), "load GlobalList");
                        return;
                    }

                    GlobalList::Cache::savePlacement(levelID, placement);
                    GlobalList::Levels::saveLevel(levelID, newLevel);
                }

                PopulateListEvent().send();
            }
        );
    }

    void getLevelPlacement(int levelID) {
        int cachedPlacement = GlobalList::Cache::getPlacement(levelID);
        if (cachedPlacement != -1) {
            PlacementEvent(levelID).send(cachedPlacement);
        }
        else {
            spawn(web::WebRequest().get(fmt::format("{}/level/classic/get?ingame_id={}", API_URL, levelID)),
                [levelID](web::WebResponse value) {
                    int placement = GlobalList::Cache::getPlacement(levelID, false);
                    if (!value.ok()) {
                        if (value.code() == 404 && placement == -1) placement = 0;
                    }
                    else {
                        auto data = value.json();
                        auto json = data.unwrapOrDefault();

                        if (!json.contains("data") || json["data"].size() == 0) {
                            Utils::failure(value.code(), "get level placement");
                            return;
                        }

                        auto levelData = json["data"];

                        placement = levelData["placement"].asInt().unwrapOrDefault();

                        if (placement > 0) GlobalList::Cache::savePlacement(levelID, placement);
                        else placement = -1;
                    }

                    PlacementEvent(levelID).send(placement);
                }
            );
        }
    }
};