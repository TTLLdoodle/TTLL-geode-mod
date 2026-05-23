#include "Filters.hpp"

namespace GlobalList::Filters {
    static LevelFilters displayFilters; // ui only
    static LevelFilters levelFilters;

    const LevelFilters& getDisplayFilters() { return displayFilters; }
    const LevelFilters& getLevelFilters() { return levelFilters; }

    void applyFilters() {
        levelFilters = displayFilters;
    }

    void setLengthFilter(LengthFilter type, bool select) {
        int idx = static_cast<int>(type);
        displayFilters.lengthFilter[idx] = select;
    }
    void setDifficultyFilter(DifficultyFilter type, bool select) {
        int idx = static_cast<int>(type);
        displayFilters.diffFilter[idx] = select;
    }

    void setCustomLengthFilter(size_t from, size_t to) {
        if (from > to) return;
        displayFilters.customLengthFilter = {from, to};
    }
    void setCustomDifficultyFilter(size_t from, size_t to) {
        if (from > to) return;
        displayFilters.customDiffFilter = {from, to};
    }

    void setRateFilter(bool rated, bool unrated) {
        displayFilters.rated = rated;
        displayFilters.unrated = unrated;
    }
    
    void setCompleted(bool completed) {
        displayFilters.completed = completed;
    }
    void setByHolder(bool byHolder) {
        displayFilters.byHolder = byHolder;
    }

    void setUsername(const std::string& username) {
        displayFilters.username = username;
    }
    void setHolderName(const std::string& holderName) {
        displayFilters.holder = holderName;
    }

    void clearFilters(bool clearDisplayFilters, bool clearLevelFilters) {
        if (clearDisplayFilters) displayFilters.clear();
        if (clearLevelFilters) levelFilters.clear();
    }
};