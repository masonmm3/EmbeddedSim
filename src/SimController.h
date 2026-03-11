#ifndef SIM_CONTROLLER_H
#define SIM_CONTROLLER_H

#include <bitset>
#include <iostream>
#include <bitset>
#include <cstddef>
#include "ISimController.h"

template <size_t N>
class SimController : public ISimController {
	protected:
		std::bitset<N> ports;
	public:
		virtual ~SimController() = default;

		void SetPort(int port, PortState value) override{
			if (port >= 0 && static_cast<std::size_t>(port) < N) {
            	ports[port] = (value == PortState::High);
        	}
		}

		void SetPort(int port, int value) {
			SetPort(port, value ? PortState::High : PortState::Low);
		}

		PortState GetPort(int port) const override {
			if (port >= 0 && static_cast<std::size_t>(port) < N) {
	            return ports.test(port) ? PortState::High : PortState::Low;
	        }
	        return PortState::Low;
		}

		std::size_t GetPortCount() const override{
			return N;
		}
		
		virtual void SetPortMode() override = 0;	
};

#endif

