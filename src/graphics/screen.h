#pragma once

#include <cstdint>

namespace graphics {
inline constexpr uint8_t SCREEN_WIDTH = 128;
inline constexpr uint8_t SCREEN_HEIGHT = 128;

inline constexpr uint8_t PADDING_TOP = 7;
inline constexpr uint8_t PADDING_BOT = 8;
inline constexpr uint8_t PADDING_LEFT = 1;
inline constexpr uint8_t PADDING_RIGHT = 1;



inline constexpr uint8_t FRAME_X_LO = PADDING_LEFT;
inline constexpr uint8_t FRAME_X_HI = SCREEN_HEIGHT - PADDING_RIGHT;
inline constexpr uint8_t FRAME_W = FRAME_X_HI - FRAME_X_LO;

inline constexpr uint8_t FRAME_Y_LO = PADDING_TOP;
inline constexpr uint8_t FRAME_Y_HI = SCREEN_HEIGHT - PADDING_BOT;
inline constexpr uint8_t FRAME_H = FRAME_Y_HI - FRAME_Y_LO;


inline constexpr int8_t SCENE_X_LO = 0 - (FRAME_W + 1) / 2;
inline constexpr int8_t SCENE_X_HI = 0 + (FRAME_W    ) / 2;
inline constexpr int8_t SCENE_Y_LO = 0 - (FRAME_H + 1) / 2;
inline constexpr int8_t SCENE_Y_HI = 0 + (FRAME_H    ) / 2;

inline constexpr int8_t SCENE_X_TO_FRAME = ((int8_t)FRAME_X_LO) - SCENE_X_LO;
inline constexpr int8_t SCENE_Y_TO_FRAME = ((int8_t)FRAME_Y_LO) - SCENE_Y_LO;


void clearBorder(uint8_t c);
void clearScreen(uint8_t c);

}