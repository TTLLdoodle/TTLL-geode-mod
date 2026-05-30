#include "Utils.hpp"

namespace Utils {
    void removePlacement(const int levelID, CCNode* gdlLabel, CCNode* gdlIcon, const std::unordered_map<CCNode*, float> origPositions, const bool onLevelCell) {
        GlobalList::Cache::saveLevelWOPlacement(levelID);
        if (gdlLabel) gdlLabel->setVisible(false);
        if (gdlIcon) gdlIcon->setVisible(false);
        for (auto& [node, pos] : origPositions) if (node) onLevelCell ? node->setPositionX(pos) : node->setPositionY(pos);
    }

    void failure(int code, std::string whatsFailed) {
        auto alertLayer = FLAlertLayer::create(
            fmt::format("Load failed ({})", code).c_str(),
            fmt::format("Failed to {}. Please try again.", whatsFailed).c_str(),
            "OK"
        );
        alertLayer->m_scene = CCDirector::get()->getRunningScene();
        alertLayer->show();
    }
};