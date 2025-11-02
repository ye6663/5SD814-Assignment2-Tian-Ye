// event_system.hpp

#pragma once
#include "event_types.hpp"
#include <functional>
#include <unordered_map>
#include <vector>
#include <any>

class EventSystem {
public:
    using EventCallback = std::function<void(const std::any&)>;

    static EventSystem& getInstance() {
        static EventSystem instance;
        return instance;
    }

    // Subscribe
    void subscribe(EventType type, EventCallback callback) {
        m_listeners[type].push_back(callback);
    }

    // Publish
    void publish(EventType type, const std::any& data = {}) {
        auto it = m_listeners.find(type);
        if (it != m_listeners.end()) {
            for (const auto& callback : it->second) {
                callback(data);
            }
        }
    }

    // Clear all listeners
    void clear() {
        m_listeners.clear();
    }

private:
    EventSystem() = default;
    ~EventSystem() = default;

    std::unordered_map<EventType, std::vector<EventCallback>> m_listeners;
};