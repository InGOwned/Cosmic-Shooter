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
    //   player(),
    //   backgroundTexture(),
    gameState(State::MainMenu),
    backgroundSprite(backgroundTexture),
    shootSound(shootBuffer),
    deathSound(deathBuffer),
    hitSound(hitBuffer)
{
    window.setFramerateLimit(60);
    
    // Загрузка шрифта
    if (!font.openFromFile("../../../sfml_sample_3_0/assets/fonts/arialmt.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    } else {
        scoreText = std::make_unique<sf::Text>(font);
        scoreText->setCharacterSize(24);
        scoreText->setFillColor(sf::Color::White);
        scoreText->setPosition(sf::Vector2f(10.f, 10.f));
    }

    // Загрузка текстуры фона
    if (!backgroundTexture.loadFromFile("../../../sfml_sample_3_0/assets/images/background2.jpg")) {
        std::cerr << "Failed to load background texture" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture, true);
    backgroundSprite.setScale(sf::Vector2f(
        static_cast<float>(Constants::WINDOW_WIDTH) / backgroundTexture.getSize().x,
        static_cast<float>(Constants::WINDOW_HEIGHT) / backgroundTexture.getSize().y
    ));

    if (!shootBuffer.loadFromFile("../../../sfml_sample_3_0/assets/audio/shoot.wav")) {
        std::cerr << "Failed to load shoot sound" << std::endl;
    }
    shootSound.setBuffer(shootBuffer);

    if (!deathBuffer.loadFromFile("../../../sfml_sample_3_0/assets/audio/death.wav")) {
        std::cerr << "Failed to load death sound" << std::endl;
    }
    deathSound.setBuffer(deathBuffer);

    if (!hitBuffer.loadFromFile("../../../sfml_sample_3_0/assets/audio/hit.wav")) {
        std::cerr << "Failed to load hit sound" << std::endl;
    }
    hitSound.setBuffer(hitBuffer);

    gameOverText = std::make_unique<sf::Text>(font);
    gameOverText->setCharacterSize(40);
    gameOverText->setFillColor(sf::Color::Red);
    updateGameOverText();
    
    // Инициализация меню
    initMenu();
}

void Game::initMenu() {
    titleText = std::make_unique<sf::Text>(font);
    titleText->setFont(font);
    titleText->setString("Cosmic Shooter");
    titleText->setCharacterSize(60);
    titleText->setFillColor(sf::Color::Cyan);
    titleText->setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = titleText->getLocalBounds();
    titleText->setOrigin(sf::Vector2f(titleBounds.size.x/2, titleBounds.size.y/2));
    titleText->setPosition(sf::Vector2f(Constants::WINDOW_WIDTH/2, 100));

    playButton = std::make_unique<sf::Text>(font);
    playButton->setFont(font);
    playButton->setString("Play");
    playButton->setCharacterSize(40);
    playButton->setFillColor(sf::Color::White);
    sf::FloatRect playBounds = playButton->getLocalBounds();
    playButton->setOrigin(sf::Vector2f(playBounds.size.x/2, playBounds.size.y/2));
    playButton->setPosition(sf::Vector2f(Constants::WINDOW_WIDTH/2, 250));

    exitButton = std::make_unique<sf::Text>(font);
    exitButton->setFont(font);
    exitButton->setString("Exit");
    exitButton->setCharacterSize(40);
    exitButton->setFillColor(sf::Color::White);
    sf::FloatRect exitBounds = exitButton->getLocalBounds();
    exitButton->setOrigin(sf::Vector2f(exitBounds.size.x/2, exitBounds.size.y/2));
    exitButton->setPosition(sf::Vector2f(Constants::WINDOW_WIDTH/2, 350));
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        
        if (gameState == State::Playing) {
            update();
        }
        
        render();
    }
}

void Game::resetGame() {
    gameState = State::Playing;
    score = 0;
    enemySpawnTimer = 0;
    enemies.clear();
    bullets.clear();
    player.resetPosition();
    updateGameOverText();
}

void Game::updateGameOverText() {
    if (!gameOverText) return;
    
    gameOverText->setString("GAME OVER\n\nScore: " + std::to_string(score) + 
                            "\nPress SPACE to restart\nPress ESC to Menu");
    
    sf::FloatRect textRect = gameOverText->getLocalBounds();
    gameOverText->setOrigin(sf::Vector2f(textRect.size.x / 2, textRect.size.y / 2));
    gameOverText->setPosition(sf::Vector2f(Constants::WINDOW_WIDTH / 2.0f, Constants::WINDOW_HEIGHT / 2.0f));
}

void Game::processEvents() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        
        // Обработка событий в главном меню
        if (gameState == State::MainMenu) {
        if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
            sf::Vector2f mousePos(static_cast<float>(mouseMoved->position.x), static_cast<float>(mouseMoved->position.y));
            // Используем ->
            playButton->setFillColor(playButton->getGlobalBounds().contains(mousePos) ? sf::Color::Green : sf::Color::White);
            exitButton->setFillColor(exitButton->getGlobalBounds().contains(mousePos) ? sf::Color::Red : sf::Color::White);
        }
        
        if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseButton->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos(static_cast<float>(mouseButton->position.x), static_cast<float>(mouseButton->position.y));
                // Используем ->
                if (playButton->getGlobalBounds().contains(mousePos)) {
                    resetGame();
                } else if (exitButton->getGlobalBounds().contains(mousePos)) {
                    window.close();
                }
            }
        }
    }
        
        // Обработка клавиши Space
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Space) {
                if (gameState == State::Playing) {
                    sf::Vector2f position = player.getPosition();
                    bullets.push_back(std::make_unique<Bullet>(position.x, position.y));
                    shootSound.play();
                } else if (gameState == State::GameOver) {
                    resetGame();
                }
            }
        }
        
        // Обработка клавиши Escape
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                if (gameState == State::Playing || gameState == State::GameOver) {
                    gameState = State::MainMenu;
                }
            }
        }
    }
    
    if (gameState == State::Playing) {
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
    
     switch (gameState) {
        case State::MainMenu:
            window.draw(*titleText);
            window.draw(*playButton);
            window.draw(*exitButton);
            break;
            
        case State::Playing:
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
            break;
            
        case State::GameOver:
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
            if (gameOverText) {
                window.draw(*gameOverText);
            }
            break;
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
                hitSound.play();
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
            gameState = State::GameOver;
            updateGameOverText();
            return;
        }
    }
}
