//
// Created by conta on 2/18/2024.
//

#pragma once

#include "Types.h"

namespace KeyCode {
    constexpr u32 Space        = 32;
    constexpr u32 Apostrophe   = 39;
    constexpr u32 Comma        = 44;
    constexpr u32 Minus        = 45;
    constexpr u32 Period       = 46;
    constexpr u32 Slash        = 47;
    constexpr u32 Zero         = 48;
    constexpr u32 One          = 49;
    constexpr u32 Two          = 50;
    constexpr u32 Three        = 51;
    constexpr u32 Four         = 52;
    constexpr u32 Five         = 53;
    constexpr u32 Six          = 54;
    constexpr u32 Seven        = 55;
    constexpr u32 Eight        = 56;
    constexpr u32 Nine         = 57;
    constexpr u32 Semicolon    = 59;
    constexpr u32 Equal        = 61;
    constexpr u32 A            = 65;
    constexpr u32 B            = 66;
    constexpr u32 C            = 67;
    constexpr u32 D            = 68;
    constexpr u32 E            = 69;
    constexpr u32 F            = 70;
    constexpr u32 G            = 71;
    constexpr u32 H            = 72;
    constexpr u32 I            = 73;
    constexpr u32 J            = 74;
    constexpr u32 K            = 75;
    constexpr u32 L            = 76;
    constexpr u32 M            = 77;
    constexpr u32 N            = 78;
    constexpr u32 O            = 79;
    constexpr u32 P            = 80;
    constexpr u32 Q            = 81;
    constexpr u32 R            = 82;
    constexpr u32 S            = 83;
    constexpr u32 T            = 84;
    constexpr u32 U            = 85;
    constexpr u32 V            = 86;
    constexpr u32 W            = 87;
    constexpr u32 X            = 88;
    constexpr u32 Y            = 89;
    constexpr u32 Z            = 90;
    constexpr u32 LeftBracket  = 91;
    constexpr u32 Backslash    = 92;
    constexpr u32 RightBracket = 93;
    constexpr u32 GraveAccent  = 96;
    constexpr u32 World1       = 161;
    constexpr u32 World2       = 162;
    constexpr u32 Escape       = 256;
    constexpr u32 Enter        = 257;
    constexpr u32 Tab          = 258;
    constexpr u32 Backspace    = 259;
    constexpr u32 Insert       = 260;
    constexpr u32 Delete       = 261;
    constexpr u32 Right        = 262;
    constexpr u32 Left         = 263;
    constexpr u32 Down         = 264;
    constexpr u32 Up           = 265;
    constexpr u32 PageUp       = 266;
    constexpr u32 PageDown     = 267;
    constexpr u32 Home         = 268;
    constexpr u32 End          = 269;
    constexpr u32 CapsLock     = 280;
    constexpr u32 ScrollLock   = 281;
    constexpr u32 NumLock      = 282;
    constexpr u32 PrintScreen  = 283;
    constexpr u32 Pause        = 284;
    constexpr u32 F1           = 290;
    constexpr u32 F2           = 291;
    constexpr u32 F3           = 292;
    constexpr u32 F4           = 293;
    constexpr u32 F5           = 294;
    constexpr u32 F6           = 295;
    constexpr u32 F7           = 296;
    constexpr u32 F8           = 297;
    constexpr u32 F9           = 298;
    constexpr u32 F10          = 299;
    constexpr u32 F11          = 300;
    constexpr u32 F12          = 301;
    constexpr u32 F13          = 302;
    constexpr u32 F14          = 303;
    constexpr u32 F15          = 304;
    constexpr u32 F16          = 305;
    constexpr u32 F17          = 306;
    constexpr u32 F18          = 307;
    constexpr u32 F19          = 308;
    constexpr u32 F20          = 309;
    constexpr u32 F21          = 310;
    constexpr u32 F22          = 311;
    constexpr u32 F23          = 312;
    constexpr u32 F24          = 313;
    constexpr u32 F25          = 314;
    constexpr u32 KP_0         = 320;
    constexpr u32 KP_1         = 321;
    constexpr u32 KP_2         = 322;
    constexpr u32 KP_3         = 323;
    constexpr u32 KP_4         = 324;
    constexpr u32 KP_5         = 325;
    constexpr u32 KP_6         = 326;
    constexpr u32 KP_7         = 327;
    constexpr u32 KP_8         = 328;
    constexpr u32 KP_9         = 329;
    constexpr u32 KP_DECIMAL   = 330;
    constexpr u32 KP_DIVIDE    = 331;
    constexpr u32 KP_MULTIPLY  = 332;
    constexpr u32 KP_SUBTRACT  = 333;
    constexpr u32 KP_ADD       = 334;
    constexpr u32 KP_ENTER     = 335;
    constexpr u32 KP_EQUAL     = 336;
    constexpr u32 LeftShift    = 340;
    constexpr u32 LeftControl  = 341;
    constexpr u32 LeftAlt      = 342;
    constexpr u32 LeftSuper    = 343;
    constexpr u32 RightShift   = 344;
    constexpr u32 RightControl = 345;
    constexpr u32 RightAlt     = 346;
    constexpr u32 RightSuper   = 347;
    constexpr u32 RightMenu    = 348;
}  // namespace KeyCode

namespace MouseButton {
    constexpr u32 Left   = 0;
    constexpr u32 Right  = 1;
    constexpr u32 Middle = 2;
}  // namespace MouseButton