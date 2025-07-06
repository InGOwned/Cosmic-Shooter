#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Constants.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();
    void spawnEnemy();
    void checkCollisions();

    sf::RenderWindow window;
    Player player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Bullet>> bullets;
    int enemySpawnTimer = 0;
    int score = 0;
    sf::Font font;
    sf::Text scoreText;
};

#endif // GAME_H