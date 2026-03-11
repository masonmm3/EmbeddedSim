#include "ESP32.hpp"
#include <iostream>

void ESP32::Init() {
    std::cout << "Initializing ESP32 GPIO pins...\n";
    ports.reset();
}

void ESP32::DeInit() {
    std::cout << "Shutting down ESP32 Controller...\n";
}