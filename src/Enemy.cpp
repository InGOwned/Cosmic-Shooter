#include "Enemy.h"

Enemy::Enemy(float x, float y) {
    shape.setSize(sf::Vector2f(Constants::ENEMY_SIZE, Constants::ENEMY_SIZE));
    shape.setFillColor(sf::Color::Red);
    // Изменено: setPosition теперь принимает sf::Vector2f
    shape.setPosition(sf::Vector2f(x, y));
    velocity.y = Constants::ENEMY_SPEED;
}

void Enemy::update() {
    shape.move(velocity);
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Enemy::getBounds() const {
    return shape.getGlobalBounds();
}

bool Enemy::isOutOfScreen() const {
    return shape.getPosition().y > Constants::WINDOW_HEIGHT;
}
