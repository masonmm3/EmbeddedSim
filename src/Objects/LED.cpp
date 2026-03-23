#include "LED.hpp"
#include <iostream>

void LED::SetConnectedPort(ISimController* pController, int id) {
    if (!pController || id < 0 || static_cast<std::size_t>(id) >= pController->GetPortCount()) {
        std::cerr << "Invalid connection attempt for LED on port " << id << "\n";
        return;
    }

    this->portId = id;
    ports[id] = pController;
    
    if (id < 0 || id >= pController->GetPortCount()) {
        std::cerr << "Invalid port ID " << id << " for LED connection.\n";
        return;
    }
    
    pController->SetCallback(id, std::bind(&LED::UpdateLED, this));

    UpdateLED();
}

PortState LED::GetInternalPortState() const {
    return enabled ? PortState::High : PortState::Low;
} 

void LED::UpdateLED() {
    ISimController* port = ports[portId];
    if (port) {
        enabled = port->GetPort(portId) == PortState::High;
    }
}