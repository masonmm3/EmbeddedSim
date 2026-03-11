#include <cstdio>
#include "src/ISimController.h"
#include "src/controllers/Esp32.hpp"
#include "src/Objects/LED.hpp"
#include <thread>
#include <chrono>


int main(void) {

	ESP32 esp;

	esp.Init();

	LED led1;

	led1.SetConnectedPort((ISimController*)&esp, 10);

	bool state = 0;

	while (true) {
		led1.UpdateLED();

		printf("LED1 state: %s\n", led1.GetInternalPortState(10) == PortState::High ? "ON" : "OFF");

		esp.SetPort(10, state);

		state = !state;
		
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}