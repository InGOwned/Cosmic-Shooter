#include "Player.h"
#include <iostream>

Player::Player() {
    shape.setSize(sf::Vector2f(Constants::PLAYER_SIZE, Constants::PLAYER_SIZE));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(Constants::WINDOW_WIDTH / 2 - Constants::PLAYER_SIZE / 2, 
                      Constants::WINDOW_HEIGHT - Constants::PLAYER_SIZE - 10);
}

void Player::update() {
    shape.move(velocity);
    
    // Keep player within window bounds
    if (shape.getPosition().x < 0) shape.setPosition(0, shape.getPosition().y);
    if (shape.getPosition().x > Constants::WINDOW_WIDTH - Constants::PLAYER_SIZE) 
        shape.setPosition(Constants::WINDOW_WIDTH - Constants::PLAYER_SIZE, shape.getPosition().y);
    if (shape.getPosition().y < 0) shape.setPosition(shape.getPosition().x, 0);
    if (shape.getPosition().y > Constants::WINDOW_HEIGHT - Constants::PLAYER_SIZE) 
        shape.setPosition(shape.getPosition().x, Constants::WINDOW_HEIGHT - Constants::PLAYER_SIZE);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Player::handleInput() {
    velocity.x = 0;
    velocity.y = 0;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocity.x = -Constants::PLAYER_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocity.x = Constants::PLAYER_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        velocity.y = -Constants::PLAYER_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        velocity.y = Constants::PLAYER_SPEED;
    }
}

sf::FloatRect Player::getBounds() const {
    return shape.getGlobalBounds();
}