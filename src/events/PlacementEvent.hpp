#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>

using namespace geode::prelude;

class PlacementEvent : public Event<PlacementEvent, bool(int), int> {
public:
    using Event::Event;
};