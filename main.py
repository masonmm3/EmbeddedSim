import EmbeddedSimLib as EmbeddedSim
import time

esp = EmbeddedSim.ESP32()

led1 = EmbeddedSim.LED()
switch1 = EmbeddedSim.Switch()
led2 = EmbeddedSim.LED()
reader = EmbeddedSim.PWMReader()
motor = EmbeddedSim.Motor()

def setup_simulation():
    led1.set_connected_port(esp, 10)
    
    switch1.set_connected_port(esp, 1, 2)
    
    led2.set_connected_port(esp, 8)
    
    esp.set_port(1, EmbeddedSim.PortState.High)
    
    reader.set_connected_port(esp, 5)
    motor.set_connected_port(esp, 6)

def run_loop():
    setup_simulation()
    state = False

    print("Starting Embedded Simulation Logic...")
    
    try:
        while True:
            print(f"LED1 Active:    {led1.get_internal_port_state() == EmbeddedSim.PortState.High}")
            print(f"Switch Pushed:  {switch1.get_internal_port_state() == EmbeddedSim.PortState.High}")
            print(f"Pin 2 State:    {esp.get_port(2)}")
            print(f"PWM Duty Cycle: {reader.get_pwm_cycle():.2f}")
            print(f"Motor Speed:    {motor.speed:.2f}")
            print(f"Loop Delta:     {esp.get_delta_time_ns()} ns")
            print("")

            state = not state
            esp.set_port(10, EmbeddedSim.PortState.High if state else EmbeddedSim.PortState.Low)
            
            if state:
                switch1.swap_switch()

            esp.set_port(8, esp.get_port(2))

            esp.set_port_pwm(5, 90)
            esp.set_port_pwm(6, 90)

            time.sleep(1)

    except KeyboardInterrupt:
        print("Simulation terminated.")

if __name__ == "__main__":
    run_loop()