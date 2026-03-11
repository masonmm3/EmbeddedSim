#ifndef ISIM_CONTROLLER_H
#define ISIM_CONTROLLER_H

#include <cstddef>

enum PortState {
	Low = 0,
	High = 1,
};

class ISimController {
public:
    virtual ~ISimController() = default;

    // Use virtual methods that don't depend on the template <N>
    virtual void SetPort(int port, PortState value) = 0;
    virtual PortState GetPort(int port) const = 0;
    
    virtual void Init() = 0;
    virtual void DeInit() = 0;
    virtual void SetPortMode() = 0;
    virtual std::size_t GetPortCount() const = 0;
};

#endif