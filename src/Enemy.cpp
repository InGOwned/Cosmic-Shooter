#include "Enemy.h"
#include <iostream>

Enemy::Enemy(float x, float y)
    : texture(),  // Инициализируем текстуру
      sprite(texture)  // Инициализируем спрайт с текстурой
    {
    if (!texture.loadFromFile("../../../Cosmic-Shooter/assets/images/enemy.png")) {
        std::cerr << "Failed to load enemy texture" << std::endl;
    }
    sprite.setTexture(texture, true);
    sprite.setScale(sf::Vector2f(Constants::ENEMY_SIZE / sprite.getLocalBounds().size.x, Constants::ENEMY_SIZE / sprite.getLocalBounds().size.y));
    sprite.setPosition(sf::Vector2f(x, y));
    velocity.y = Constants::ENEMY_SPEED;
}

void Enemy::update() {
    sprite.move(velocity);
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Enemy::getBounds() const {
    return sprite.getGlobalBounds();
}

bool Enemy::isOutOfScreen() const {
    return sprite.getPosition().y > Constants::WINDOW_HEIGHT;
}


