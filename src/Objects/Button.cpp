#include "Button.hpp"

void Button::SetConnectedPort(ISimController* pController, int HighId, int LowId) {
    ports[HighId] = pController;
    ports[LowId] = pController;
    this->HighId = HighId;
    this->LowId = LowId;

    if (HighId < 0 || HighId >= pController->GetPortCount() || LowId < 0 || LowId >= pController->GetPortCount()) {
        std::cerr << "Invalid port ID for Button connection.\n";
        return;
    }

    pController->SetCallback(std::bind(&Button::UpdateButton, this), HighId);
    pController->SetCallback(std::bind(&Button::UpdateButton, this), LowId);
}

PortState Button::GetInternalPortState(bool HighSide) {
    if (HighSide) {
        bool state = pushed ? HighSide || LowSide : HighSide;
        return state ? PortState::High : PortState::Low;
    } else if (LowSide) {
        bool state = pushed ? HighSide || LowSide : HighSide;
        return state ? PortState::High : PortState::Low;
    }
} 

void Button::PushButton() {
    pushed = true;
    if (HighSide && !LowSide) {
       LowSide = HighSide; 
       shortLow = true;
       ports[LowId]->SetPort(LowId, PortState::High);
    }
    
    if (LowSide && !HighSide) {
        HighSide = LowSide;
        shortHigh = true;
        ports[HighId]->SetPort(HighId, PortState::High);
    }
}

void Button::UpdateButton() {

    HighSide = ports[HighId]->GetPort(HighId);
    LowSide = ports[LowId]->GetPort(LowId);

    if (shortHigh && !pushed) {
        HighSide = false;
        shortHigh = false;
        ports[HighId]->SetPort(HighId, PortState::Low);
    }

    if (shortLow && !pushed) {
        LowSide = false;
        shortLow = false;
        ports[LowId]->SetPort(LowId, PortState::Low);
    }
}

