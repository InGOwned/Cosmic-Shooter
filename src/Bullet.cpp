#include "Bullet.h"

Bullet::Bullet(float x, float y) {
    shape.setSize(sf::Vector2f(Constants::BULLET_SIZE, Constants::BULLET_SIZE));
    shape.setFillColor(sf::Color::Yellow);
    // Изменено: setPosition теперь принимает sf::Vector2f
    shape.setPosition(sf::Vector2f(x + Constants::PLAYER_SIZE / 2 - Constants::BULLET_SIZE / 2, y));
    velocity.y = -Constants::BULLET_SPEED;
}

void Bullet::update() {
    shape.move(velocity);
}

void Bullet::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Bullet::getBounds() const {
    return shape.getGlobalBounds();
}

bool Bullet::isOutOfScreen() const {
    return shape.getPosition().y + Constants::BULLET_SIZE < 0;
}
