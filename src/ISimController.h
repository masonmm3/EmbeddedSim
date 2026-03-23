#ifndef ISIM_CONTROLLER_H
#define ISIM_CONTROLLER_H

#include <cstddef>
#include <functional>

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
};

#endif