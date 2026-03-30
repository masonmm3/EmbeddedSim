#include "ESP32.hpp"
#include <iostream>

ESP32::ESP32() {
    std::cout << "Initializing ESP32 GPIO pins...\n";
    ports.reset();
    controllerCallbacks.push_back(std::bind(&ESP32::PWMUpdate, this));
}

void ESP32::PWMUpdate() {
    if (pwmHandlers.empty()) return;
    
    for (int i = 0; i < pwmHandlers.size(); i++) {
        ESP32PWM &signal = pwmHandlers[i];
        int onPhase = static_cast<long long>(signal.dutyCycle * signal.periodNs);
        int offPhase = signal.periodNs - onPhase;

        signal.counterNs += deltaTimeNs;

        if (signal.lastState && signal.counterNs >= onPhase) {
            SetPort(signal.port, PortState::Low);
            signal.lastState = false;
            signal.counterNs -= onPhase;
        } else if (!signal.lastState && signal.counterNs >= offPhase) {
            SetPort(signal.port, PortState::High);
            signal.lastState = true;
            signal.counterNs -= offPhase;
        }
    }
}

void ESP32::SetPortPWM(std::size_t port, uint8_t dutyCycle) {
    if (port >= 0 && port < GetPortCount()) {

        if (pwmHandlers[port].dutyCycle == (static_cast<float>(dutyCycle) / 100)) return; // No change, skip

        ESP32PWM signal{port, (static_cast<float>(dutyCycle) / 100), false, 0};
        pwmHandlers[port] = signal;
    }
}

void ESP32::SetPortMode() {
    
}
