#include <pybind11/pybind11.h>
#include "EmbeddedSim.h"
#include "EmbeddedSimLib/SimController.h"

namespace py = pybind11;

void BindTypes(pybind11::module_& m) {
    py::enum_<PortState>(m, "PortState")
        .value("Low", PortState::Low)
        .value("High", PortState::High)
        .export_values();

    py::enum_<InterruptMode>(m, "InterruptMode")
        .value("Rising", InterruptMode::Rising)
        .value("Falling", InterruptMode::Falling)
        .value("Change", InterruptMode::Change)
        .value("Low", InterruptMode::Low)
        .value("High", InterruptMode::High)
        .export_values();
}

void BindESP32(pybind11::module_& m) {
    py::class_<ESP32PWM>(m, "ESP32PWM")
        .def_readwrite("port", &ESP32PWM::port)
        .def_readwrite("dutyCycle", &ESP32PWM::dutyCycle)
        .def_readwrite("lastState", &ESP32PWM::lastState)
        .def_readwrite("counterNs", &ESP32PWM::counterNs)
        .def_readwrite("periodNs", &ESP32PWM::periodNs);

    py::class_<ESP32, SimController<35>>(m, "ESP32")
        .def(py::init<>())
        .def("set_port", static_cast<void (ESP32::*)(std::size_t, PortState)>(&ESP32::SetPort))
        .def("get_port", static_cast<PortState (ESP32::*)(std::size_t)>(&ESP32::GetPort))
        .def("set_port_mode", &ESP32::SetPortMode)
        .def("set_port_pwm", &ESP32::SetPortPWM);
}

class PyISimController : public ISimController {
public:
    using ISimController::ISimController;
    void SetPort(std::size_t port, PortState value) override { PYBIND11_OVERRIDE_PURE(void, ISimController, SetPort, port, value); }
    PortState GetPort(std::size_t port) override { PYBIND11_OVERRIDE_PURE(PortState, ISimController, GetPort, port); }
    void SetPortMode() override { PYBIND11_OVERRIDE_PURE(void, ISimController, SetPortMode); }
    void SetCallback(std::size_t port, std::function<void()> callback) override { PYBIND11_OVERRIDE_PURE(void, ISimController, SetCallback, port, callback); }
    std::size_t GetPortCount() const override { PYBIND11_OVERRIDE_PURE(std::size_t, ISimController, GetPortCount); }
    long long GetDeltaTimeNs() const override { PYBIND11_OVERRIDE_PURE(long long, ISimController, GetDeltaTimeNs); }
    void AttachInterrupt(std::size_t port, std::function<void()> isr, InterruptMode mode) override {
        PYBIND11_OVERRIDE_PURE(void, ISimController, AttachInterrupt, port, isr, mode);
    }
    void DetachInterrupt(std::size_t port) override {
        PYBIND11_OVERRIDE_PURE(void, ISimController, DetachInterrupt, port);
    }
    EventDispatcher& GetEventDispatcher() override {
        PYBIND11_OVERRIDE_PURE(EventDispatcher&, ISimController, GetEventDispatcher);
    }
};

void BindISimController(py::module &m) {
    py::class_<ISimController, PyISimController>(m, "ISimController")
        .def("get_port_count", &ISimController::GetPortCount)
        .def("get_delta_time_ns", &ISimController::GetDeltaTimeNs)
        .def("detach_interrupt", &ISimController::DetachInterrupt, py::arg("port"));
}

template <size_t N>
void DeclareSimController(py::module &m, const std::string &typestr) {
    using Class = SimController<N>;
    
    py::class_<Class, ISimController>(m, typestr.c_str())
        .def("set_port", static_cast<void (Class::*)(std::size_t, PortState)>(&Class::SetPort), 
             "Set port using PortState enum")
        .def("set_port", static_cast<void (Class::*)(std::size_t, std::size_t)>(&Class::SetPort), 
             "Set port using integer/boolean value")
        
        .def("get_port", &Class::GetPort)
        .def("set_callback", &Class::SetCallback)
        .def("get_delta_time_ns", &Class::GetDeltaTimeNs)
        .def("get_port_count", &Class::GetPortCount)
        .def("attach_interrupt", [](Class& self, std::size_t port, py::object isr, InterruptMode mode) {
            self.AttachInterrupt(port, [isr]() {
                py::gil_scoped_acquire acquire;
                isr();
            }, mode);
        }, py::arg("port"), py::arg("isr"), py::arg("mode") = InterruptMode::Change)
        .def("detach_interrupt", &Class::DetachInterrupt, py::arg("port"));
}

void BindSimControllerBase(py::module &m) {
    DeclareSimController<35>(m, "SimController35");
}

class PySimObject : public SimObject {
public:
    using SimObject::SimObject;
    PortState GetInternalPortState(std::size_t id) override {
        PYBIND11_OVERRIDE(PortState, SimObject, GetInternalPortState, id);
    }
};

void BindSimObject(py::module &m) {
    py::class_<SimObject, PySimObject>(m, "SimObject")
        .def(py::init<>())
        // Accept a pointer to the interface
        .def("set_connected_port", &SimObject::SetConnectedPort, 
             py::arg("pController"),
             "Connects a SimController to this object")
        
        .def("get_internal_port_state", &SimObject::GetInternalPortState,
             py::arg("id"),
             "Returns the internal state of a specific port");
}

void BindLed(py::module &m) {
    py::class_<LED, SimObject>(m, "LED")
        .def(py::init<>())
        // Explicitly bind the overloaded/specific SetConnectedPort
        .def("set_connected_port", &LED::SetConnectedPort, 
             py::arg("pController"), py::arg("id"),
             "Connects LED to a specific controller port")
        .def("get_internal_port_state", &LED::GetInternalPortState,
             "Returns the current LED state")
        .def("update_led", &LED::UpdateLED,
             "Triggers an update for the LED logic");
}

void BindMotor(py::module &m) {
    py::class_<Motor, SimObject>(m, "Motor")
        .def(py::init<>())
        .def("set_connected_port", &Motor::SetConnectedPort, 
             py::arg("pController"), py::arg("id"),
             "Connects Motor to a specific controller port")
        .def("get_internal_port_state", &Motor::GetInternalPortState,
             "Returns the current internal port state of the motor")
        .def("update_motor", &Motor::UpdateMotor,
             "Triggers motor simulation logic")
        .def_property_readonly("speed", &Motor::GetSpeed, 
             "Returns the current motor speed");
}

void BindPwmReader(py::module &m) {
    py::class_<PWMReader, SimObject>(m, "PWMReader")
        .def(py::init<>())
        .def("set_connected_port", &PWMReader::SetConnectedPort, 
             py::arg("pController"), py::arg("id"),
             "Connects PWMReader to a specific controller port")
        .def("get_internal_port_state", &PWMReader::GetInternalPortState,
             "Returns the current port state")
        .def("update_pwm", &PWMReader::UpdatePWM,
             "Updates the internal timing logic for PWM calculation")
        .def("get_pwm_cycle", &PWMReader::GetPWMCycle,
             "Returns the calculated duty cycle (0.0 to 1.0)");
}

void BindSwitch(py::module &m) {
    py::class_<Switch, SimObject>(m, "Switch")
        .def(py::init<>())
        .def("set_connected_port", &Switch::SetConnectedPort, 
             py::arg("pController"), py::arg("HighId"), py::arg("LowId"),
             "Connects Switch to high and low controller ports")
        .def("get_internal_port_state", &Switch::GetInternalPortState,
             "Returns current switch state")
        .def("swap_switch", &Switch::SwapSwitch, "Toggles switch position")
        .def("enable_switch", &Switch::EnableSwitch, "Closes the switch")
        .def("disable_switch", &Switch::DisableSwitch, "Opens the switch")
        .def("update_switch", &Switch::UpdateSwitch, "Updates simulation logic");
}

void BindObjects(py::module &m) {
    BindLed(m);
    BindMotor(m);
    BindPwmReader(m);
    BindSwitch(m);
}


PYBIND11_MODULE(EmbeddedSimLib, m, py::mod_gil_not_used()) {
    m.doc() = "EmbeddedSim Plugin bindings for python"; 

    BindTypes(m);
    BindISimController(m);
    BindSimControllerBase(m);
    BindESP32(m);
    BindSimObject(m);
    BindObjects(m);
}