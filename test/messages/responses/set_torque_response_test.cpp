/**
 * \file set_torque_response_test.cpp
 * \mainpage
 *    Test parsing of set torque response
 * \author
 *    Tobit Flatscher (github.com/2b-t)
*/

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "myactuator_rmd_driver/messages/responses/set_torque_response.hpp"
#include "myactuator_rmd_driver/feedback.hpp"


namespace myactuator_rmd_driver {
  namespace test {

    TEST(SetTorqueResponseTest, parsingPositiveValues) {
      myactuator_rmd_driver::SetTorqueResponse const response {{0xA1, 0x32, 0x64, 0x00, 0xF4, 0x01, 0x2D, 0x00}};
      myactuator_rmd_driver::Feedback const feedback {response.getFeedback()};
      auto const temperature {feedback.getTemperature()};
      EXPECT_NEAR(temperature, 50.0f, 0.1f);
      auto const current {feedback.getTorqueCurrent()};
      EXPECT_NEAR(current, 1.0f, 0.1f);
      auto const speed {feedback.getShaftSpeed()};
      EXPECT_NEAR(speed, 500.0f, 0.1f);
      auto const angle {feedback.getShaftAngle()};
      EXPECT_NEAR(angle, 45.0f, 0.1f);
    }

    TEST(SetTorqueResponseTest, parsingNegativeValues) {
      myactuator_rmd_driver::SetTorqueResponse const response {{0xA1, 0x32, 0x9C, 0xFF, 0x0C, 0xFE, 0xD3, 0xFF}};
      myactuator_rmd_driver::Feedback const feedback {response.getFeedback()};
      auto const temperature {feedback.getTemperature()};
      EXPECT_NEAR(temperature, 50.0f, 0.1f);
      auto const current {feedback.getTorqueCurrent()};
      EXPECT_NEAR(current, -1.0f, 0.1f);
      auto const speed {feedback.getShaftSpeed()};
      EXPECT_NEAR(speed, -500.0f, 0.1f);
      auto const angle {feedback.getShaftAngle()};
      EXPECT_NEAR(angle, -45.0f, 0.1f);
    }

  }
}