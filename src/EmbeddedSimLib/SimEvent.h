#ifndef SIM_EVENT_H
#define SIM_EVENT_H

#include <cstdint>
#include <cstddef>

enum class EventSource : uint8_t {
    GPIO = 0,
};

enum class InterruptMode {
    Rising,
    Falling,
    Change,
    Low,
    High,
};

struct SimEvent {
    EventSource source;
    std::size_t id;
    uint32_t data1;
    uint32_t data2;
};

#endif
