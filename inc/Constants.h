#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace Constants {
    // Используем constexpr для compile-time констант
    static constexpr int WINDOW_WIDTH = 1200;
    static constexpr int WINDOW_HEIGHT = 800;
    
    // Для строк используем constexpr с std::string_view (C++17)
    static constexpr std::string_view WINDOW_TITLE = "Cosmic Shooter";

    static constexpr float PLAYER_SPEED = 5.0f;
    static constexpr float ENEMY_SPEED = 2.0f;
    static constexpr float BULLET_SPEED = 7.0f;

    static constexpr int PLAYER_SIZE = 60;
    static constexpr int ENEMY_SIZE = 40;
    static constexpr int BULLET_SIZE = 10;

    static constexpr int ENEMY_SPAWN_INTERVAL = 60; // frames
    static constexpr int MAX_ENEMIES = 10;
}

#endif // CONSTANTS_H