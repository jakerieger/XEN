//
// Created by conta on 2/18/2024.
//

#pragma once

#include <stdint.h>

struct FKeyEvent {
    uint32_t KeyCode;
    uint32_t Action;
    uint32_t Modifiers;
};

struct FMouseEvent {
    uint32_t Button;
};

struct FScrollEvent {};