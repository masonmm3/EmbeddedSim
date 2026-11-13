#ifndef ISIM_CONTROLLER_H
#define ISIM_CONTROLLER_H

#include <cstddef>
#include <functional>

#include "SimEvent.h"
#include "EventDispatcher.h"

enum PortState {
	Low = 0,
	High = 1,
};

class ISimController {
    public:
        virtual ~ISimController() = default;

        virtual void SetPort(std::size_t port, PortState value) = 0;
        virtual PortState GetPort(std::size_t port) = 0;
        
        virtual void SetPortMode() = 0;
        virtual void SetCallback(std::size_t port, std::function<void()> callback) = 0;
        virtual std::size_t GetPortCount() const = 0;
        virtual long long GetDeltaTimeNs() const = 0;

        virtual void AttachInterrupt(std::size_t port, std::function<void()> isr, InterruptMode mode) = 0;
        virtual void DetachInterrupt(std::size_t port) = 0;
        virtual EventDispatcher& GetEventDispatcher() = 0;
};

#endif