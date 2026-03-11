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

        virtual void SetPort(int port, PortState value) = 0;
        virtual PortState GetPort(int port) = 0;
        
        virtual void SetPortMode() = 0;
        virtual void SetCallback(std::function<void()> callback, int port) = 0;
        virtual std::size_t GetPortCount() const = 0;
};

#endif