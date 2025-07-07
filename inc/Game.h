#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Constants.h"

class Player;
class Bullet;
class Enemy;

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
    unsigned int score = 0;
    unsigned int enemySpawnTimer = 0;
    sf::Font font;
    std::unique_ptr<sf::Text> scoreText;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
};

