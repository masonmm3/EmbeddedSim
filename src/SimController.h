#ifndef SIM_CONTROLLER_H
#define SIM_CONTROLLER_H

#include <bitset>
#include <functional>
#include <iostream>
#include <bitset>
#include <cstddef>
#include <vector>
#include "ISimController.h"

template <size_t N>
class SimController : public ISimController {
	protected:
		std::bitset<N> ports;
		std::vector<std::function<void()>> callbacks;
	public:
		virtual ~SimController() = default;

		void SetPort(int port, PortState value) override{
			if (port >= 0 && static_cast<std::size_t>(port) < N) {
            	ports[port] = (value == PortState::High);
        	}
		}

		void SetPort(int port, int value) {
			SetPort(port, value ? PortState::High : PortState::Low);
			runCallbacks();
		}

		PortState GetPort(int port) override {
			if (port >= 0 && static_cast<std::size_t>(port) < N) {
	            return ports.test(port) ? PortState::High : PortState::Low;
	        }
			runCallbacks();
	        return PortState::Low;
		}

		void runCallbacks() {
			for (const auto& callback : callbacks) {
				callback();
			}
		}

		void SetCallback(std::function<void()> callback) {
			callbacks.push_back(callback);
		}

		std::size_t GetPortCount() const override{
			return N;
		}
		
		virtual void SetPortMode() override = 0;
	
};

#endif

