#include "Enemy.h"
#include "Map.h"

Enemy::Enemy()
{
	m_target = nullptr;
	m_projectiles = nullptr;
	m_Map = nullptr;

	m_mesh = nullptr;
}

HitBox Enemy::UpdateDimensions()
{
	return HitBox();
}

bool Enemy::Hit(int damage)
{
	m_health -= damage;
	return m_health <= 0;
}

void Enemy::SetTexture()
{
	m_texture.FromFile("assets/enemy_tex.png");
}

bool Enemy::IsTargetable()
{
	return true;
}