#include "src/ISimController.h"
#include "src/Objects/Switch.hpp"
#include "src/controllers/Esp32.hpp"
#include "src/Objects/LED.hpp"
#include "src/Objects/Switch.hpp"


#include <thread>
#include <chrono>
#include <cstdio>


int main(void) {

	ESP32 esp;

	LED led1;

	led1.SetConnectedPort((ISimController*)&esp, 10);

	Switch switch1;

	switch1.SetConnectedPort((ISimController*)&esp, 1, 2);

	LED led2;

	led2.SetConnectedPort((ISimController*)&esp, 8);

	bool state = 0;

	esp.SetPort(1, 1);

	while (true) {

		//Testing section
		printf("LED1 state: %s\n", led1.GetInternalPortState() == PortState::High ? "ON" : "OFF");
		printf("Button Pushed: %s\n", switch1.GetInternalPortState() == PortState::High ? "YES" : "NO");
		printf("Port 2 State: %s\n", esp.GetPort(2) == PortState::High ? "HIGH" : "LOW");
		printf("LED2 state: %s\n", led2.GetInternalPortState() == PortState::High ? "ON" : "OFF");

		printf("\n");
		//Testing section end

		state = !state;

		esp.SetPort(10, state);
		
		if (state) {
			switch1.SwapSwitch();
		}

		esp.SetPort(8, esp.GetPort(2));

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}