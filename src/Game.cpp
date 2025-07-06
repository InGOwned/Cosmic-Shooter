#include "Game.h"
#include <iostream>

Game::Game() : window(sf::VideoMode(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT), Constants::WINDOW_TITLE) {
    window.setFramerateLimit(60);
    
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }
    
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
            bullets.emplace_back(std::make_unique<Bullet>(
                player.getBounds().left, 
                player.getBounds().top
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
        [](const std::unique_ptr<Bullet>& b) { return b->isOutOfScreen(); }), 
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
    enemies.emplace_back(std::make_unique<Enemy>(x, -Constants::ENEMY_SIZE));
}

void Game::checkCollisions() {
    // Bullet-Enemy collisions
    for (auto& bullet : bullets) {
        for (auto& enemy : enemies) {
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