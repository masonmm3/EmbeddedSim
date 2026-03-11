#include "LED.hpp"

void LED::SetConnectedPort(ISimController* pController, int id) {
    ports[id] = pController;
    this->id = id;
    
    if (id < 0 || id >= pController->GetPortCount()) {
        std::cerr << "Invalid port ID " << id << " for LED connection.\n";
        return;
    }
    
    pController->SetCallback(std::bind(&LED::UpdateLED, this));

    enabled = 0;
}

PortState LED::GetInternalPortState() {
    return enabled ? PortState::High : PortState::Low;
} 

void LED::UpdateLED() {
    if (ports[id]) {
        enabled = ports[id]->GetPort(id) == PortState::High;
    }
}