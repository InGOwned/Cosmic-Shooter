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
      backgroundSprite(backgroundTexture) // Инициализируем спрайт с текстурой
      
{
    window.setFramerateLimit(60);
    
    // Загрузка шрифта
    if (!font.openFromFile("E:/Repositories/sfml_sample_3_0/assets/fonts/arial.cff")) {
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
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Space) {
                sf::Vector2f position = player.getPosition();
                bullets.push_back(std::make_unique<Bullet>(position.x, position.y));
            }
        }
    }
    
    player.handleInput();
}

void Game::update() {
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
    window.draw(backgroundSprite); // Отрисовка фона
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
            std::cout << "Game Over! Final Score: " << score << std::endl;
            window.close();
        }
    }
}

