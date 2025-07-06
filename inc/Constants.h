#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants {
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    const std::string WINDOW_TITLE = "Cosmic Shooter";

    const float PLAYER_SPEED = 5.0f;
    const float ENEMY_SPEED = 2.0f;
    const float BULLET_SPEED = 7.0f;

    const int PLAYER_SIZE = 50;
    const int ENEMY_SIZE = 40;
    const int BULLET_SIZE = 10;

    const int ENEMY_SPAWN_INTERVAL = 60; // frames
    const int MAX_ENEMIES = 10;
}

#endif // CONSTANTS_H