/**
 * \file myactuator_rmd.cpp
 * \mainpage
 *    Python bindings for C++ library
 * \author
 *    Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <string>
#include <sstream>
#include <tuple>

#include <pybind11/chrono.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "myactuator_rmd/actuator_state/acceleration_type.hpp"
#include "myactuator_rmd/actuator_state/can_baud_rate.hpp"
#include "myactuator_rmd/actuator_state/control_mode.hpp"
#include "myactuator_rmd/actuator_state/error_code.hpp"
#include "myactuator_rmd/actuator_state/feedback.hpp"
#include "myactuator_rmd/actuator_state/gains.hpp"
#include "myactuator_rmd/actuator_state/motor_status_1.hpp"
#include "myactuator_rmd/actuator_state/motor_status_2.hpp"
#include "myactuator_rmd/actuator_state/motor_status_3.hpp"
#include "myactuator_rmd/can/exceptions.hpp"
#include "myactuator_rmd/can/frame.hpp"
#include "myactuator_rmd/can/node.hpp"
#include "myactuator_rmd/driver/can_driver.hpp"
#include "myactuator_rmd/driver/driver.hpp"
#include "myactuator_rmd/actuator_constants.hpp"
#include "myactuator_rmd/actuator_interface.hpp"
#include "myactuator_rmd/exceptions.hpp"
#include "myactuator_rmd/io.hpp"


namespace myactuator_rmd {
  namespace bindings {

    /**\fn declareActuator
     * \brief
     *    Helper function for declaring the actuator constants for a given actuator
     * 
     * \tparam T
     *    The class containing the actuator constants
     * \param[in] m
     *    Pybind11 module that the actuator constants should be declared into
     * \param[in] class_name
     *    Class name of the corresponding Python bindings
    */
    template<typename T>
    void declareActuator(pybind11::module& m, std::string const& class_name) {
      pybind11::class_<T>(m, class_name.c_str())
        .def_readonly_static("reducer_ratio", &T::reducer_ratio)
        .def_readonly_static("rated_speed", &T::rated_speed)
        .def_readonly_static("rated_current", &T::rated_current)
        .def_readonly_static("rated_power", &T::rated_power)
        .def_readonly_static("rated_torque", &T::rated_torque)
        .def_readonly_static("torque_constant", &T::torque_constant)
        .def_readonly_static("rotor_inertia", &T::rotor_inertia);
      return;
    }

  }
}

PYBIND11_MODULE(myactuator_rmd_py, m) {

  m.doc() = "Python bindings for MyActuator RMD-X actuator series";

  pybind11::class_<myactuator_rmd::Driver>(m, "Driver");
  
  // New bindings for motion mode.  
  pybind11::class_<myactuator_rmd::motion_mode::CanDriver, myactuator_rmd::Driver>(m, "MotionCanDriver")
    .def(pybind11::init<std::string const&>());
  pybind11::class_<myactuator_rmd::motion_mode::ActuatorInterface>(m, "MotionActuatorInterface") 
    .def(pybind11::init<myactuator_rmd::Driver&, std::uint32_t>())
    .def("motionModeControl", &myactuator_rmd::motion_mode::ActuatorInterface::motionModeControl);
  // end of new
  
  pybind11::class_<myactuator_rmd::CanDriver, myactuator_rmd::Driver>(m, "CanDriver")
    .def(pybind11::init<std::string const&>());
  pybind11::class_<myactuator_rmd::ActuatorInterface>(m, "ActuatorInterface")
    .def(pybind11::init<myactuator_rmd::Driver&, std::uint32_t>())
    .def("getAcceleration", &myactuator_rmd::ActuatorInterface::getAcceleration)
    .def("getCanId", &myactuator_rmd::ActuatorInterface::getCanId)
    .def("getControllerGains", &myactuator_rmd::ActuatorInterface::getControllerGains)
    .def("getControlMode", &myactuator_rmd::ActuatorInterface::getControlMode)
    .def("getMotorModel", &myactuator_rmd::ActuatorInterface::getMotorModel)
    .def("getMotorPower", &myactuator_rmd::ActuatorInterface::getMotorPower)
    .def("getMotorStatus1", &myactuator_rmd::ActuatorInterface::getMotorStatus1)
    .def("getMotorStatus2", &myactuator_rmd::ActuatorInterface::getMotorStatus2)
    .def("getMotorStatus3", &myactuator_rmd::ActuatorInterface::getMotorStatus3)
    .def("getMultiTurnAngle", &myactuator_rmd::ActuatorInterface::getMultiTurnAngle)
    .def("getMultiTurnEncoderPosition", &myactuator_rmd::ActuatorInterface::getMultiTurnEncoderPosition)
    .def("getMultiTurnEncoderOriginalPosition", &myactuator_rmd::ActuatorInterface::getMultiTurnEncoderOriginalPosition)
    .def("getMultiTurnEncoderZeroOffset", &myactuator_rmd::ActuatorInterface::getMultiTurnEncoderZeroOffset)
    .def("getRuntime", &myactuator_rmd::ActuatorInterface::getRuntime)
    .def("getSingleTurnAngle", &myactuator_rmd::ActuatorInterface::getSingleTurnAngle)
    .def("getSingleTurnEncoderPosition", &myactuator_rmd::ActuatorInterface::getSingleTurnEncoderPosition)
    .def("getVersionDate", &myactuator_rmd::ActuatorInterface::getVersionDate)
    .def("lockBrake", &myactuator_rmd::ActuatorInterface::lockBrake)
    .def("releaseBrake", &myactuator_rmd::ActuatorInterface::releaseBrake)
    .def("reset", &myactuator_rmd::ActuatorInterface::reset)
    .def("sendCurrentSetpoint", &myactuator_rmd::ActuatorInterface::sendCurrentSetpoint)
    .def("sendPositionAbsoluteSetpoint", &myactuator_rmd::ActuatorInterface::sendPositionAbsoluteSetpoint)
    .def("sendTorqueSetpoint", &myactuator_rmd::ActuatorInterface::sendTorqueSetpoint)
    .def("sendVelocitySetpoint", &myactuator_rmd::ActuatorInterface::sendVelocitySetpoint)
    .def("setAcceleration", &myactuator_rmd::ActuatorInterface::setAcceleration)
    .def("setCanBaudRate", &myactuator_rmd::ActuatorInterface::setCanBaudRate)
    .def("setCanId", &myactuator_rmd::ActuatorInterface::setCanId)
    .def("setControllerGains", &myactuator_rmd::ActuatorInterface::setControllerGains)
    .def("setCurrentPositionAsEncoderZero", &myactuator_rmd::ActuatorInterface::setCurrentPositionAsEncoderZero)
    .def("setEncoderZero", &myactuator_rmd::ActuatorInterface::setEncoderZero)
    .def("setTimeout", &myactuator_rmd::ActuatorInterface::setTimeout)
    .def("shutdownMotor", &myactuator_rmd::ActuatorInterface::shutdownMotor)
    .def("stopMotor", &myactuator_rmd::ActuatorInterface::stopMotor);
  pybind11::register_exception<myactuator_rmd::Exception>(m, "ActuatorException");
  pybind11::register_exception<myactuator_rmd::ProtocolException>(m, "ProtocolException");
  pybind11::register_exception<myactuator_rmd::ValueRangeException>(m, "ValueRangeException");

  auto m_actuator_state = m.def_submodule("actuator_state", "Submodule for actuator state structures");
  pybind11::enum_<myactuator_rmd::AccelerationType>(m_actuator_state, "AccelerationType")
    .value("POSITION_PLANNING_ACCELERATION", myactuator_rmd::AccelerationType::POSITION_PLANNING_ACCELERATION)
    .value("POSITION_PLANNING_DECELERATION", myactuator_rmd::AccelerationType::POSITION_PLANNING_DECELERATION)
    .value("VELOCITY_PLANNING_ACCELERATION", myactuator_rmd::AccelerationType::VELOCITY_PLANNING_ACCELERATION)
    .value("VELOCITY_PLANNING_DECELERATION", myactuator_rmd::AccelerationType::VELOCITY_PLANNING_DECELERATION);
  pybind11::enum_<myactuator_rmd::CanBaudRate>(m_actuator_state, "CanBaudRate")
    .value("KBPS500", myactuator_rmd::CanBaudRate::KBPS500)
    .value("MBPS1", myactuator_rmd::CanBaudRate::MBPS1);
  pybind11::enum_<myactuator_rmd::ControlMode>(m_actuator_state, "ControlMode")
    .value("NONE", myactuator_rmd::ControlMode::NONE)
    .value("CURRENT", myactuator_rmd::ControlMode::CURRENT)
    .value("VELOCITY", myactuator_rmd::ControlMode::VELOCITY)
    .value("POSITION", myactuator_rmd::ControlMode::POSITION);
  pybind11::enum_<myactuator_rmd::ErrorCode>(m_actuator_state, "ErrorCode")
    .value("NO_ERROR", myactuator_rmd::ErrorCode::NO_ERROR)
    .value("MOTOR_STALL", myactuator_rmd::ErrorCode::MOTOR_STALL)
    .value("LOW_VOLTAGE", myactuator_rmd::ErrorCode::LOW_VOLTAGE)
    .value("OVERVOLTAGE", myactuator_rmd::ErrorCode::OVERVOLTAGE)
    .value("OVERCURRENT", myactuator_rmd::ErrorCode::OVERCURRENT)
    .value("POWER_OVERRUN", myactuator_rmd::ErrorCode::POWER_OVERRUN)
    .value("SPEEDING", myactuator_rmd::ErrorCode::SPEEDING)
    .value("UNSPECIFIED_1", myactuator_rmd::ErrorCode::UNSPECIFIED_1)
    .value("UNSPECIFIED_2", myactuator_rmd::ErrorCode::UNSPECIFIED_2)
    .value("UNSPECIFIED_3", myactuator_rmd::ErrorCode::UNSPECIFIED_3)
    .value("OVERTEMPERATURE", myactuator_rmd::ErrorCode::OVERTEMPERATURE)
    .value("ENCODER_CALIBRATION_ERROR", myactuator_rmd::ErrorCode::ENCODER_CALIBRATION_ERROR);
  pybind11::class_<myactuator_rmd::Gains>(m_actuator_state, "Gains")
    .def(pybind11::init<myactuator_rmd::PiGains const&, myactuator_rmd::PiGains const&, myactuator_rmd::PiGains const&>())
    .def(pybind11::init<std::uint8_t const, std::uint8_t const, std::uint8_t const, std::uint8_t const, std::uint8_t const, std::uint8_t const>())
    .def_readwrite("current", &myactuator_rmd::Gains::current)
    .def_readwrite("speed", &myactuator_rmd::Gains::speed)
    .def_readwrite("position", &myactuator_rmd::Gains::position)
    .def("__repr__", [](myactuator_rmd::Gains const& gains) -> std::string { 
      std::ostringstream ss {};
      ss << gains;
      return ss.str();
    });
  pybind11::class_<myactuator_rmd::MotorStatus1>(m_actuator_state, "MotorStatus1")
    .def(pybind11::init<int const, bool const, float const, myactuator_rmd::ErrorCode const>())
    .def_readonly("temperature", &myactuator_rmd::MotorStatus1::temperature)
    .def_readonly("is_brake_released", &myactuator_rmd::MotorStatus1::is_brake_released)
    .def_readonly("voltage", &myactuator_rmd::MotorStatus1::voltage)
    .def_readonly("error_code", &myactuator_rmd::MotorStatus1::error_code)
    .def("__repr__", [](myactuator_rmd::MotorStatus1 const& motor_status) -> std::string { 
      std::ostringstream ss {};
      ss << motor_status;
      return ss.str();
    });
  pybind11::class_<myactuator_rmd::MotorStatus2>(m_actuator_state, "MotorStatus2")
    .def(pybind11::init<int const, float const, float const, float const>())
    .def_readonly("temperature", &myactuator_rmd::MotorStatus2::temperature)
    .def_readonly("current", &myactuator_rmd::MotorStatus2::current)
    .def_readonly("shaft_speed", &myactuator_rmd::MotorStatus2::shaft_speed)
    .def_readonly("shaft_angle", &myactuator_rmd::MotorStatus2::shaft_angle)
    .def("__repr__", [](myactuator_rmd::MotorStatus2 const& motor_status) -> std::string { 
      std::ostringstream ss {};
      ss << motor_status;
      return ss.str();
    });
  pybind11::class_<myactuator_rmd::MotorStatus3>(m_actuator_state, "MotorStatus3")
    .def(pybind11::init<int const, float const, float const, float const>())
    .def_readonly("temperature", &myactuator_rmd::MotorStatus3::temperature)
    .def_readonly("current_phase_a", &myactuator_rmd::MotorStatus3::current_phase_a)
    .def_readonly("current_phase_b", &myactuator_rmd::MotorStatus3::current_phase_b)
    .def_readonly("current_phase_c", &myactuator_rmd::MotorStatus3::current_phase_c)
    .def("__repr__", [](myactuator_rmd::MotorStatus3 const& motor_status) -> std::string { 
      std::ostringstream ss {};
      ss << motor_status;
      return ss.str();
    });
  pybind11::class_<myactuator_rmd::PiGains>(m_actuator_state, "PiGains")
    .def(pybind11::init<std::uint8_t const, std::uint8_t const>())
    .def_readwrite("kp", &myactuator_rmd::PiGains::kp)
    .def_readwrite("ki", &myactuator_rmd::PiGains::ki)
    .def("__repr__", [](myactuator_rmd::PiGains const& pi_gains) -> std::string { 
      std::ostringstream ss {};
      ss << pi_gains;
      return ss.str();
    });

  auto m_can = m.def_submodule("can", "Submodule for basic CAN communication");
  pybind11::class_<myactuator_rmd::can::Frame>(m_can, "Frame")
    .def(pybind11::init<std::uint32_t const, std::array<std::uint8_t,8> const&>())
    .def("getId", &myactuator_rmd::can::Frame::getId)
    .def("getData", &myactuator_rmd::can::Frame::getData);
  pybind11::class_<myactuator_rmd::can::Node>(m_can, "Node")
    .def(pybind11::init<std::string const&>())
    .def("setRecvFilter", &myactuator_rmd::can::Node::setRecvFilter)
    .def("read", &myactuator_rmd::can::Node::read)
    .def("write", pybind11::overload_cast<myactuator_rmd::can::Frame const&>(&myactuator_rmd::can::Node::write));
  pybind11::register_exception<myactuator_rmd::can::SocketException>(m_can, "SocketException");
  pybind11::register_exception<myactuator_rmd::can::Exception>(m_can, "CanException");
  pybind11::register_exception<myactuator_rmd::can::TxTimeoutError>(m_can, "TxTimeoutError");
  pybind11::register_exception<myactuator_rmd::can::LostArbitrationError>(m_can, "LostArbitrationError");
  pybind11::register_exception<myactuator_rmd::can::ControllerProblemError>(m_can, "ControllerProblemError");
  pybind11::register_exception<myactuator_rmd::can::ProtocolViolationError>(m_can, "ProtocolViolationError");
  pybind11::register_exception<myactuator_rmd::can::TransceiverStatusError>(m_can, "TransceiverStatusError");
  pybind11::register_exception<myactuator_rmd::can::NoAcknowledgeError>(m_can, "NoAcknowledgeError");
  pybind11::register_exception<myactuator_rmd::can::BusOffError>(m_can, "BusOffError");
  pybind11::register_exception<myactuator_rmd::can::BusError>(m_can, "BusError");
  pybind11::register_exception<myactuator_rmd::can::ControllerRestartedError>(m_can, "ControllerRestartedError");

  auto m_actuator_constants = m.def_submodule("actuator_constants", "Submodule for actuator constants");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X4V2>(m_actuator_constants,     "X4V2");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X4V3>(m_actuator_constants,     "X4V3");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X4_3>(m_actuator_constants,     "X4_3");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X4_24>(m_actuator_constants,    "X4_24");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X6V2>(m_actuator_constants,     "X6V2");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X6S2V2>(m_actuator_constants,   "X6S2V2");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X6V3>(m_actuator_constants,     "X6V3");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X6_7>(m_actuator_constants,     "X6_7");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X6_8>(m_actuator_constants,     "X6_8");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X6_40>(m_actuator_constants,    "X6_40");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X8V2>(m_actuator_constants,     "X8V2");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X8ProV2>(m_actuator_constants,  "X8ProV2");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X8S2V3>(m_actuator_constants,   "X8S2V3");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X8HV3>(m_actuator_constants,    "X8HV3");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X8ProHV3>(m_actuator_constants, "X8ProHV3");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X8_20>(m_actuator_constants,    "X8_20");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X8_25>(m_actuator_constants,    "X8_25");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X8_60>(m_actuator_constants,    "X8_60");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X8_90>(m_actuator_constants,    "X8_90");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X10V3>(m_actuator_constants,    "X10V3");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X10S2V3>(m_actuator_constants,  "X10S2V3");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X10_40>(m_actuator_constants,   "X10_40");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X10_100>(m_actuator_constants,  "X10_100");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X12_150>(m_actuator_constants,  "X12_150");
  myactuator_rmd::bindings::declareActuator<myactuator_rmd::X15_400>(m_actuator_constants,  "X15_400");

}
