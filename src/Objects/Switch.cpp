#include "Switch.hpp"
#include <iostream>

void Switch::SetConnectedPort(ISimController* pController, int HighId, int LowId) {
    if (!pController || HighId < 0 || LowId < 0 || static_cast<std::size_t>(HighId) >= pController->GetPortCount() || static_cast<std::size_t>(LowId) >= pController->GetPortCount()) {
        std::cerr << "Invalid connection attempt for Switch on ports " << HighId << " or " << LowId << "\n";
        return;
    }

    this->HighId = HighId;
    this->LowId = LowId;
    ports[HighId] = pController;
    ports[LowId] = pController;

    auto callback = [this]() { this->UpdateSwitch(); };
    pController->SetCallback(HighId, callback);
    pController->SetCallback(LowId, callback);
}

PortState Switch::GetInternalPortState() const {
   return closed ? PortState::High : PortState::Low;
} 

void Switch::SwapSwitch() {
    closed = !closed;
    UpdateSwitch();
}

void Switch::EnableSwitch() {
    closed = true;
    UpdateSwitch();
}

void Switch::DisableSwitch() {
    closed = false;
    UpdateSwitch();
}

void Switch::UpdateSwitch() {
    if (isUpdating) return;
    isUpdating = true;

    if (shortHigh && !closed)  {
        ports[HighId]->SetPort(HighId, PortState::Low);
        shortHigh = false;
    }
    if (shortLow && !closed)  
    {
        ports[LowId]->SetPort(LowId, PortState::Low);
        shortLow = false;
    }

    if (ports[LowId]->GetPort(LowId) == PortState::High && ports[HighId]->GetPort(HighId) == PortState::Low && closed) {
        ports[HighId]->SetPort(HighId, PortState::High);
        shortHigh = true;
    } else if (ports[LowId]->GetPort(LowId) == PortState::Low && ports[HighId]->GetPort(HighId) == PortState::High && closed) {
        ports[LowId]->SetPort(LowId, PortState::High);
        shortLow = true;
    }

    isUpdating = false;
}


