#ifndef SIM_CONTROLLER_H
#define SIM_CONTROLLER_H

#include <bitset>
#include <iostream>
#include <bitset>
#include <cstddef>

enum PortState {
	Low = 0,
	High = 1,
};



template <size_t N>
class SimController {
	protected:
		std::bitset<N> ports;
	public:
		virtual ~SimController() = default;

		void SetPort(int port, PortState value) {
			if (port >= 0 && static_cast<std::size_t>(port) < N) {
            	ports[port] = (value == PortState::High);
        	}
		}
		PortState GetPort(int port) {
			if (port >= 0 && static_cast<std::size_t>(port) < N) {
	            return ports.test(port) ? PortState::High : PortState::Low;
	        }
	        return PortState::Low;
		}
		
		virtual void SetPortMode();
		virtual void Init();
		virtual void DeInit();
	
}

#endif

