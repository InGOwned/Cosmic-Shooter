#include "Game.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include <cstdlib> // Для rand()

Game::Game()
    : window(sf::VideoMode(sf::Vector2u(static_cast<unsigned int>(Constants::WINDOW_WIDTH), 
                                        static_cast<unsigned int>(Constants::WINDOW_HEIGHT))), 
             std::string(Constants::WINDOW_TITLE)),
      player(),
      backgroundTexture(),
      backgroundSprite(backgroundTexture),
      shootSound(shootBuffer),
      deathSound(deathBuffer)
      
{
    window.setFramerateLimit(60);
    
    // Загрузка шрифта
    if (!font.openFromFile("E:/Repositories/sfml_sample_3_0/assets/fonts/arialmt.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    } else {
        scoreText = std::make_unique<sf::Text>(font);
        scoreText->setCharacterSize(24);
        scoreText->setFillColor(sf::Color::White);
        scoreText->setPosition(sf::Vector2f(10.f, 10.f));
    }

    // Загрузка текстуры фона
    if (!backgroundTexture.loadFromFile("E:/Repositories/sfml_sample_3_0/assets/images/background2.jpg")) {
        std::cerr << "Failed to load background texture" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture, true);
    // Масштабирование фона под размер окна
    backgroundSprite.setScale(sf::Vector2f(
        static_cast<float>(Constants::WINDOW_WIDTH) / backgroundTexture.getSize().x,
        static_cast<float>(Constants::WINDOW_HEIGHT) / backgroundTexture.getSize().y
    ));

    if (!shootBuffer.loadFromFile("E:/Repositories/sfml_sample_3_0/assets/audio/shoot.wav")) {
        std::cerr << "Failed to load shoot sound" << std::endl;
    }
    shootSound.setBuffer(shootBuffer);

    if (!deathBuffer.loadFromFile("E:/Repositories/sfml_sample_3_0/assets/audio/death.wav")) {
        std::cerr << "Failed to load death sound" << std::endl;
    }
    deathSound.setBuffer(deathBuffer);

    // Инициализация текста Game Over
    gameOverText = std::make_unique<sf::Text>(font);
    gameOverText->setString("GAME OVER\nPress SPACE to restart");
    gameOverText->setCharacterSize(40);
    gameOverText->setFillColor(sf::Color::Red);
    gameOverText->setPosition(sf::Vector2f(
        Constants::WINDOW_WIDTH / 2 - gameOverText->getLocalBounds().size.x / 2,
        Constants::WINDOW_HEIGHT / 2 - gameOverText->getLocalBounds().size.y / 2
    ));
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        
        if (!gameOver) {
            update();
        }
        
        render();
    }
}

void Game::resetGame() {
    gameOver = false;
    score = 0;
    enemySpawnTimer = 0;
    enemies.clear();
    bullets.clear();
    
    // Сброс позиции игрока
    player.resetPosition();
}

void Game::processEvents() {
    // Создаем событие с помощью placement new
    alignas(sf::Event) unsigned char eventBuffer[sizeof(sf::Event)];
    sf::Event& event = *reinterpret_cast<sf::Event*>(eventBuffer);
    
    while (auto event = window.pollEvent()) {
        // Обработка закрытия окна
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        
        // Обработка нажатия клавиши Space
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Space) {
                if (!gameOver) {
                    // Выстрел
                    sf::Vector2f position = player.getPosition();
                    bullets.push_back(std::make_unique<Bullet>(position.x, position.y));
                    shootSound.play();
                } else {
                    // Рестарт игры
                    resetGame();
                }
            }
        }
    }
    
    if (!gameOver) {
        player.handleInput();
    }
}

void Game::update() {
    if (gameOver) return;

    player.update();
    
    if (enemySpawnTimer >= Constants::ENEMY_SPAWN_INTERVAL && 
        enemies.size() < Constants::MAX_ENEMIES) {
        spawnEnemy();
        enemySpawnTimer = 0;
    }
    enemySpawnTimer++;
    
    for (auto& enemy : enemies) {
        enemy->update();
    }
    
    // Удаление врагов за пределами экрана
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const auto& e) { return e->isOutOfScreen(); }), 
        enemies.end());
    
    for (auto& bullet : bullets) {
        bullet->update();
    }
    
    // Удаление неактивных пуль
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const auto& b) { return !b->isActive; }), 
        bullets.end());
    
    checkCollisions();
    if (scoreText) {
        scoreText->setString("Score: " + std::to_string(score));
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    window.draw(backgroundSprite);
    
    if (!gameOver) {
        player.draw(window);
        for (auto& enemy : enemies) {
            enemy->draw(window);
        }
        for (auto& bullet : bullets) {
            bullet->draw(window);
        }
        if (scoreText) {
            window.draw(*scoreText);
        }
    } else {
        if (gameOverText) {
            window.draw(*gameOverText);
        }
    }
    
    window.display();
}

void Game::spawnEnemy() {
    float x = static_cast<float>(std::rand() % (Constants::WINDOW_WIDTH - Constants::ENEMY_SIZE));
    enemies.push_back(std::make_unique<Enemy>(x, -static_cast<float>(Constants::ENEMY_SIZE)));
}

bool rectsIntersect(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
    return rect1.findIntersection(rect2).has_value();
}

void Game::checkCollisions() {
    // Проверка столкновений пуль с врагами
    for (auto& bullet : bullets) {
        for (auto& enemy : enemies) {
            if (rectsIntersect(bullet->getBounds(), enemy->getBounds())) {
                bullet->isActive = false;
                enemy->isActive = false;
                score += 10;
            }
        }
    }
    
    // Удаление неактивных врагов
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const auto& e) { return !e->isActive; }), 
        enemies.end());
    
    // Удаление неактивных пуль
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const auto& b) { return !b->isActive; }), 
        bullets.end());
    
    // Проверка столкновений игрока с врагами
    for (auto& enemy : enemies) {
        if (rectsIntersect(player.getBounds(), enemy->getBounds())) {
            deathSound.play();
            gameOver = true;
            return; // Выходим сразу после обнаружения столкновения
        }
    }
}
