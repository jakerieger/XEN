//
// Created by jr on 2/12/24.
//
#include <catch2/catch_test_macros.hpp>
#include "Color.h"

TEST_CASE("Hex to RGBA", "[Conversions]") {
    auto color = FColor(0xFFFF0000);
    REQUIRE(color.Alpha == 1.0);
    REQUIRE(color.Red == 1.0);
    REQUIRE(color.Green == 0.0);
    REQUIRE(color.Blue == 0.0);
}