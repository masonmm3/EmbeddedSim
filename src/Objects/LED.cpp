#include "LED.hpp"

void LED::SetConnectedPort(ISimController* pController, int id) {
    ports[id] = pController;
    this->id = id;
    
    if (id < 0 || id >= pController->GetPortCount()) {
        std::cerr << "Invalid port ID " << id << " for LED connection.\n";
        return;
    }
    enabled = 0;
}

PortState LED::GetInternalPortState(int id) {
    return enabled ? PortState::High : PortState::Low;
} 

void LED::UpdateLED() {
    if (ports[id]) {
        enabled = ports[id]->GetPort(id) == PortState::High;
    }
}