#include "Switch.hpp"

void Switch::SetConnectedPort(ISimController* pController, int HighId, int LowId) {
    ports[HighId] = pController;
    ports[LowId] = pController;
    this->HighId = HighId;
    this->LowId = LowId;

    if (HighId < 0 || HighId >= pController->GetPortCount() || LowId < 0 || LowId >= pController->GetPortCount()) {
        std::cerr << "Invalid port ID for Button connection.\n";
        return;
    }

    pController->SetCallback(std::bind(&Switch::UpdateSwitch, this), HighId);
    pController->SetCallback(std::bind(&Switch::UpdateSwitch, this), LowId);
}

PortState Switch::GetInternalPortState() {
   return closed ? PortState::High : PortState::Low;
} 

void Switch::SwapSwitch() {
    closed = !closed;
}

void Switch::EnableSwitch() {
    closed = true;
}

void Switch::DisableSwitch() {
    closed = false;
}

void Switch::UpdateSwitch() {
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
}


