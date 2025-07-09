#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
    enum class State { Playing, GameOver, MainMenu, Paused }; // Состояния игры
    
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();
    void spawnEnemy();
    void checkCollisions();
    void resetGame();
    void updateGameOverText();
    void initMenu();
    void initPauseMenu();
    void initGameOverMenu();
    void updateGameOverMenu();

    State gameState = State::Playing;

    sf::RenderWindow window;
    Player player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Bullet>> bullets;
    unsigned int score = 0;
    unsigned int enemySpawnTimer = 0;
    sf::Font font;
    std::unique_ptr<sf::Text> scoreText;

    // Текстура фона
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    // Звуки
    sf::SoundBuffer shootBuffer;
    sf::Sound shootSound;
    sf::SoundBuffer deathBuffer;
    sf::Sound deathSound;
    sf::SoundBuffer hitBuffer;
    sf::Sound hitSound;

    // Главное меню
    std::unique_ptr<sf::Text> titleText;
    std::unique_ptr<sf::Text> playButton;
    std::unique_ptr<sf::Text> exitButton;

    // Меню паузы
    std::unique_ptr<sf::Text> pauseTitleText;
    std::unique_ptr<sf::Text> continueButton;
    std::unique_ptr<sf::Text> restartButton;
    std::unique_ptr<sf::Text> exitPauseButton;

    // Game Over Menu
    std::unique_ptr<sf::Text> gameOverTitle;
    std::unique_ptr<sf::Text> gameOverScore;
    std::unique_ptr<sf::RectangleShape> gameOverPanel;
    std::unique_ptr<sf::Text> restartButtonGameOver;
    std::unique_ptr<sf::Text> menuButtonGameOver;

    std::unique_ptr<sf::Text> gameOverText;
    bool gameOver = false;
};

