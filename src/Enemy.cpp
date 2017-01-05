#include "Enemy.h"

Enemy::Enemy(EnemyType enemy) : Character(0), HP(enemy.hp), Damage(enemy.damage), Recharge(enemy.recharge)
{
}

Enemy::~Enemy()
{
}

void Enemy::_init(b2World * world, const Vector2 & pos, float scale)
{

}

void Enemy::startContact(b2Contact * contact)
{
}

void Enemy::endContact(b2Contact * contact)
{
}

void Enemy::attack(Player * player)
{
}

void Enemy::pathfind()
{
}
