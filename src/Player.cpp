#include "Player.h"
#include <iostream>

Player::Player() 
    : texture(),  // Инициализируем текстуру
      sprite(texture)  // Инициализируем спрайт с текстурой
 {
    if (!texture.loadFromFile("E:/Repositories/sfml_sample_3_0/assets/images/player.png")) {
        std::cerr << "Failed to load player texture" << std::endl;
    }
    sprite.setTexture(texture, true);
    
    // Получаем размеры через getSize()
    sf::Vector2f localSize = sf::Vector2f(
        sprite.getLocalBounds().size.x,
        sprite.getLocalBounds().size.y
    );
    
    sprite.setScale(sf::Vector2f(
        Constants::PLAYER_SIZE / localSize.x,
        Constants::PLAYER_SIZE / localSize.y
    ));
    
    // Получаем глобальные размеры
    sf::Vector2f globalSize = sf::Vector2f(
        sprite.getGlobalBounds().size.x,
        sprite.getGlobalBounds().size.y
    );
    
    sprite.setPosition(sf::Vector2f(
        Constants::WINDOW_WIDTH / 2 - globalSize.x / 2,
        Constants::WINDOW_HEIGHT - globalSize.y - 10
    ));
}

void Player::update() {
    sprite.move(velocity);
    
    sf::Vector2f position = sprite.getPosition();
    sf::Vector2f size = sprite.getGlobalBounds().size;
    
    // Проверка границ
    if (position.x < 0) 
        sprite.setPosition(sf::Vector2f(0.f, position.y));
    if (position.x + size.x > Constants::WINDOW_WIDTH) 
        sprite.setPosition(sf::Vector2f(Constants::WINDOW_WIDTH - size.x, position.y));
    if (position.y < 0) 
        sprite.setPosition(sf::Vector2f(position.x, 0.f));
    if (position.y + size.y > Constants::WINDOW_HEIGHT) 
        sprite.setPosition(sf::Vector2f(position.x, Constants::WINDOW_HEIGHT - size.y));
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Player::resetPosition() {
    // Установка начальной позиции
    sf::Vector2f globalSize = sprite.getGlobalBounds().size;
    sprite.setPosition(sf::Vector2f(
        Constants::WINDOW_WIDTH / 2 - globalSize.x / 2,
        Constants::WINDOW_HEIGHT - globalSize.y - 10
    ));
}

void Player::handleInput() {
    velocity.x = 0;
    velocity.y = 0;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        velocity.x = -Constants::PLAYER_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        velocity.x = Constants::PLAYER_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        velocity.y = -Constants::PLAYER_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        velocity.y = Constants::PLAYER_SPEED;
    }
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

sf::Vector2f Player::getSize() const {
    return sprite.getGlobalBounds().size;
}