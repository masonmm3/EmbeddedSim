#include "src/EmbeddedSim.h"


#include <thread>
#include <chrono>
#include <cstdio>

using namespace std;

ESP32 esp;
LED led1;
Switch switch1;
LED led2;
PWMReader reader;
Motor motor; 

void SetupObjects();
void RunTests();

int main(void) {

	SetupObjects();
	bool state = 0;

	while (true) {

		//Testing section
		RunTests();
		//Testing section end

		state = !state;

		esp.SetPort(10, state);
		
		if (state) {
			switch1.SwapSwitch();
		}

		esp.SetPortPWM(5, 90);
		esp.SetPortPWM(6, 90);

		this_thread::sleep_for(chrono::seconds(1));
	}

	return 0;
}

void RunTests() {
	printf("LED1 state: %s\n", led1.GetInternalPortState() == PORT_HIGH ? "ON" : "OFF");
		printf("Button Pushed: %s\n", switch1.GetInternalPortState() == PORT_HIGH ? "YES" : "NO");
		printf("Port 2 State: %s\n", esp.GetPort(2) == PORT_HIGH ? "HIGH" : "LOW");
		printf("LED2 state: %s\n", led2.GetInternalPortState() == PORT_HIGH ? "ON" : "OFF");
		printf("PWM cycle: %f\n", reader.GetPWMCycle());
		printf("Delta Time: %lld nano seconds\n", esp.GetDeltaTimeNs());
		printf("Motor Speed %f\n", motor.GetSpeed());

		printf("\n");
}

void SetupObjects() {
	led1.SetConnectedPort(&esp, 10);

	switch1.SetConnectedPort(&esp, 1, 2);

	led2.SetConnectedPort(&esp, 8);

	esp.AttachInterrupt(2, []() {
		esp.SetPort(8, esp.GetPort(2));
	}, InterruptMode::Change);

	esp.SetPort(1, 1);

	reader.SetConnectedPort(&esp, 5);

	motor.SetConnectedPort(&esp, 6);
}