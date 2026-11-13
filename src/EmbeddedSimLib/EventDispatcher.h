#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include "SimEvent.h"
#include <functional>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <atomic>

class EventDispatcher {
public:
    using EventHandler = std::function<void(const SimEvent&)>;

    void Subscribe(EventSource source, std::size_t id, EventHandler handler) {
        std::lock_guard<std::mutex> lock(mtx);
        handlers[MakeKey(source, id)].push_back(std::move(handler));
        activeHandlerCount.fetch_add(1, std::memory_order_relaxed);
    }

    void Unsubscribe(EventSource source, std::size_t id) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = handlers.find(MakeKey(source, id));
        if (it != handlers.end()) {
            activeHandlerCount.fetch_sub(it->second.size(), std::memory_order_relaxed);
            handlers.erase(it);
        }
    }

    void Emit(const SimEvent& event) {
        if (activeHandlerCount.load(std::memory_order_relaxed) == 0) {
            return;
        }

        std::vector<EventHandler> toCall;
        {
            std::lock_guard<std::mutex> lock(mtx);
            auto it = handlers.find(MakeKey(event.source, event.id));
            if (it != handlers.end()) {
                toCall = it->second;
            }
        }

        for (const auto& handler : toCall) {
            if (handler) {
                handler(event);
            }
        }
    }

    void Clear() {
        std::lock_guard<std::mutex> lock(mtx);
        handlers.clear();
        activeHandlerCount.store(0, std::memory_order_relaxed);
    }

private:
    static uint64_t MakeKey(EventSource source, std::size_t id) {
        return (static_cast<uint64_t>(source) << 32) | (static_cast<uint64_t>(id) & 0xFFFFFFFFULL);
    }

    std::mutex mtx;
    std::atomic<std::size_t> activeHandlerCount{0};
    std::unordered_map<uint64_t, std::vector<EventHandler>> handlers;
};

#endif
