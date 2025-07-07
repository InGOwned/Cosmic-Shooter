#include "Player.h"
#include <iostream>

Player::Player() {
    shape.setSize(sf::Vector2f(Constants::PLAYER_SIZE, Constants::PLAYER_SIZE));
    shape.setFillColor(sf::Color::Green);
    // Изменено: setPosition теперь принимает sf::Vector2f
    shape.setPosition(sf::Vector2f(Constants::WINDOW_WIDTH / 2 - Constants::PLAYER_SIZE / 2, 
                                   Constants::WINDOW_HEIGHT - Constants::PLAYER_SIZE - 10));
}

void Player::update() {
    shape.move(velocity);
    
    // Keep player within window bounds
    // Изменено: setPosition теперь принимает sf::Vector2f
    if (shape.getPosition().x < 0) shape.setPosition(sf::Vector2f(0.f, shape.getPosition().y));
    if (shape.getPosition().x > Constants::WINDOW_WIDTH - Constants::PLAYER_SIZE) 
        shape.setPosition(sf::Vector2f(static_cast<float>(Constants::WINDOW_WIDTH - Constants::PLAYER_SIZE), shape.getPosition().y));
    if (shape.getPosition().y < 0) shape.setPosition(sf::Vector2f(shape.getPosition().x, 0.f));
    if (shape.getPosition().y > Constants::WINDOW_HEIGHT - Constants::PLAYER_SIZE) 
        shape.setPosition(sf::Vector2f(shape.getPosition().x, static_cast<float>(Constants::WINDOW_HEIGHT - Constants::PLAYER_SIZE)));
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Player::handleInput() {
    velocity.x = 0;
    velocity.y = 0;
    
    // Изменено: sf::Keyboard::Left теперь sf::Keyboard::Key::Left и т.д.
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
    return shape.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const {
    return shape.getPosition();
}

sf::Vector2f Player::getSize() const {
    return shape.getSize();
}