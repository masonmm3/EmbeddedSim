#ifndef SIM_CONTROLLER_H
#define SIM_CONTROLLER_H

#include <bitset>
#include <functional>
#include <iostream>
#include <bitset>
#include <cstddef>
#include <map>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <vector>
#include "ISimController.h"

template <size_t N>
class SimController : public ISimController {
	protected:
		std::bitset<N> ports;
		std::map<int, std::function<void()>> callbacks;
		
		std::mutex mtx;
		std::thread worker;
    	std::atomic<bool> running{true};

		void workerLoop(int intervalMs) {
			while (running) {
				std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
				std::lock_guard<std::mutex> lock(mtx);
				for (auto const& callback : callbacks) {
					if (callback.second) callback.second();
				}
			}
		}

	public:
		SimController(int intervalMs = 100) {
			worker = std::thread(&SimController::workerLoop, this, intervalMs);
		}

		virtual ~SimController() {
			running = false;
			if (worker.joinable()) {
				worker.join();
			}
		}

		/**
		 * @brief Set the value of a port on the controller
		 * 
		 * @param port 
		 * @param value 
		 */
		void SetPort(int port, PortState value) override{
			if (port >= 0 && static_cast<std::size_t>(port) < N) {
            	ports[port] = (value == PortState::High);
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
	        return PortState::Low;
		}

		/**
		 * @brief For Custom Objects to register a callback to be called when a port value changes
		 * 
		 * @param callback 
		 */
		void SetCallback(std::function<void()> callback, int port) override {
			std::lock_guard<std::mutex> lock(mtx);
			callbacks[port] = callback;
		}

		std::size_t GetPortCount() const override{
			return N;
		}
		
		virtual void SetPortMode() override = 0;
};

#endif

