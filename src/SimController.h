#ifndef SIM_CONTROLLER_H
#define SIM_CONTROLLER_H

#include <bitset>
#include <functional>
#include <iostream>
#include <bitset>
#include <cstddef>
#include <map>
#include <vector>
#include "ISimController.h"

template <size_t N>
class SimController : public ISimController {
	protected:
		std::bitset<N> ports;
		std::map<int, std::function<void()>> callbacks;

		/**
		 * @brief runs the registered callbacks for this controller
		 * 
		 */
		void runCallbacks(int port) {
			for (const auto& callback : callbacks) {
				callbacks[port]();
			}
		}

	public:
		virtual ~SimController() = default;

		/**
		 * @brief Set the value of a port on the controller
		 * 
		 * @param port 
		 * @param value 
		 */
		void SetPort(int port, PortState value) override{
			if (port >= 0 && static_cast<std::size_t>(port) < N) {
            	ports[port] = (value == PortState::High);
				runCallbacks(port);
        	}
		}

		/**
		* @brief Set the value of a port on the controller
		* 
		* @param port 
		* @param value
		*/
		void SetPort(int port, int value) {
			SetPort(port, value ? PortState::High : PortState::Low);
		}

		/**
		 * @brief Get the value of a port on the controller
		 * 
		 * @param port 
		 * @return PortState 
		 */
		PortState GetPort(int port) override {
			if (port >= 0 && static_cast<std::size_t>(port) < N) {
	            return ports.test(port) ? PortState::High : PortState::Low;
	        }
			runCallbacks(port);
	        return PortState::Low;
		}

		/**
		 * @brief For Custom Objects to register a callback to be called when a port value changes
		 * 
		 * @param callback 
		 */
		void SetCallback(std::function<void()> callback, int port) override {
			callbacks[port] = callback;
		}

		std::size_t GetPortCount() const override{
			return N;
		}
		
		virtual void SetPortMode() override = 0;
};

#endif

