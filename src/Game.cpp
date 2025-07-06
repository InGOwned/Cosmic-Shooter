#include "Game.h"
#include <iostream>
#include <algorithm> // Для std::remove_if
#include <memory>    // Для std::make_unique

// Конструктор Game
Game::Game() 
    : window(sf::VideoMode(sf::Vector2u(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT)), sf::String(Constants::WINDOW_TITLE))
{
    window.setFramerateLimit(60);
    
    // Загрузка шрифта
    // В SFML 3.0.0 loadFromFile является статическим методом и возвращает sf::Font
    font = sf::Font::loadFromFile("assets/fonts/arial.ttf");
    if (font.getNativeHandle() == 0) { // Проверка на успешную загрузку шрифта
        std::cerr << "Failed to load font" << std::endl;
    }
    
    // Инициализация scoreText с шрифтом
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(sf::Vector2f(10.f, 10.f)); // Используем sf::Vector2f
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event; // sf::Event больше не имеет конструктора по умолчанию, объявляем без инициализации
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        // sf::Keyboard::Space теперь sf::Keyboard::Key::Space
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Space) {
            bullets.emplace_back(std::make_unique<Bullet>(
                player.getBounds().left(), // Используем .left() вместо .left
                player.getBounds().top()   // Используем .top() вместо .top
            ));
        }
    }
    
    player.handleInput();
}

void Game::update() {
    player.update();
    
    // Spawn enemies
    if (enemySpawnTimer >= Constants::ENEMY_SPAWN_INTERVAL && enemies.size() < Constants::MAX_ENEMIES) {
        spawnEnemy();
        enemySpawnTimer = 0;
    }
    enemySpawnTimer++;
    
    // Update enemies
    for (auto& enemy : enemies) {
        enemy->update();
    }
    
    // Remove out of screen enemies
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), 
        [](const std::unique_ptr<Enemy>& e) { return e->isOutOfScreen(); }), 
        enemies.end());
    
    // Update bullets
    for (auto& bullet : bullets) {
        bullet->update();
    }
    
    // Remove out of screen bullets
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), 
        [](const std::unique_ptr<Bullet>& b) { return !b->isActive; }), 
        bullets.end());
    
    checkCollisions();
    
    scoreText.setString("Score: " + std::to_string(score));
}

void Game::render() {
    window.clear(sf::Color::Black);
    
    player.draw(window);
    
    for (auto& enemy : enemies) {
        enemy->draw(window);
    }
    
    for (auto& bullet : bullets) {
        bullet->draw(window);
    }
    
    window.draw(scoreText);
    window.display();
}

void Game::spawnEnemy() {
    float x = static_cast<float>(rand() % (Constants::WINDOW_WIDTH - Constants::ENEMY_SIZE));
    enemies.emplace_back(std::make_unique<Enemy>(x, -static_cast<float>(Constants::ENEMY_SIZE))); // Приведение к float
}

void Game::checkCollisions() {
    // Bullet-Enemy collisions
    for (auto& bullet : bullets) {
        for (auto& enemy : enemies) {
            // intersects теперь принимает const sf::Rect<T>& 
            if (bullet->getBounds().intersects(enemy->getBounds())) {
                bullet->isActive = false;
                enemy->isActive = false;
                score += 10;
            }
        }
    }
    
    // Remove inactive objects
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), 
        [](const std::unique_ptr<Enemy>& e) { return !e->isActive; }), 
        enemies.end());
    
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), 
        [](const std::unique_ptr<Bullet>& b) { return !b->isActive; }), 
        bullets.end());
    
    // Player-Enemy collisions
    for (auto& enemy : enemies) {
        if (player.getBounds().intersects(enemy->getBounds())) {
            // Game over logic can be added here
            std::cout << "Game Over! Final Score: " << score << std::endl;
            window.close();
        }
    }
}
