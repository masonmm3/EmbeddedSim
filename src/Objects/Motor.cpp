#include "Motor.hpp"
#include <iostream>

void Motor::SetConnectedPort(ISimController* pController, int id) {
    if (!pController || id < 0 || static_cast<std::size_t>(id) >= pController->GetPortCount()) {
        std::cerr << "Invalid connection attempt for Motor on port " << id << "\n";
        return;
    }

    this->portId = id;
    ports[id] = pController;
    
    if (id < 0 || id >= pController->GetPortCount()) {
        std::cerr << "Invalid port ID " << id << " for Motor connection.\n";
        return;
    }
    
    pController->SetCallback(id, std::bind(&Motor::UpdateMotor, this));

    UpdateMotor();
}

void Motor::UpdateMotor() {
    ISimController* port = ports[portId];
    if (port) {
        PortState state = port->GetPort(portId);
        speed += (torque * (((state == PortState::High) ? 1.0f : 0.0f)) - (torque * speed)) * (port->GetDeltaTimeNs()/1000000000.0f);
        if (speed > 1.0f) speed = 1.0f;
        if (speed < 0.0f) speed = 0.0f;
    }
}

PortState Motor::GetInternalPortState() const {
    return PortState::Low;
}
