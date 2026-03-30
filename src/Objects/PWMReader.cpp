#include "PWMReader.hpp"
#include <iostream>

void PWMReader::SetConnectedPort(ISimController* pController, int id) {
    if (!pController || id < 0 || static_cast<std::size_t>(id) >= pController->GetPortCount()) {
        std::cerr << "Invalid connection attempt for PWMReader on port " << id << "\n";
        return;
    }

    this->portId = id;
    ports[id] = pController;
    
    if (id < 0 || id >= pController->GetPortCount()) {
        std::cerr << "Invalid port ID " << id << " for PWMReader connection.\n";
        return;
    }

    controller = pController;
    
    pController->SetCallback(id, std::bind(&PWMReader::UpdatePWM, this));

    UpdatePWM();
}

void PWMReader::UpdatePWM() {
    ISimController* port = ports[portId];
    if (port) {
        PortState state = port->GetPort(portId);

        if (state == PortState::High && !lastState) {
            calculateDutyCycle();

            onDuration = 0;
            offDuration = 0;
        }

        float dt  = controller->GetDeltaTimeNs();
        if (state == PortState::High) {
            onDuration += dt;
        } else {
            offDuration += dt;
        }

        lastState = (state == PortState::High);
    }
}

void PWMReader::calculateDutyCycle() {
    float totalCycle = onDuration + offDuration;
    if (totalCycle > 1e-6f) {
        currentCycle = onDuration / totalCycle;
    } else {
        currentCycle = 0;
    }
}

float PWMReader::GetPWMCycle() {
    return currentCycle;
}