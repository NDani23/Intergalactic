#include "Projectile.h"

float Projectile::GetSpeed()
{
	return m_speed;
}

glm::vec3 Projectile::GetDirection()
{
	return m_direction;
}

int Projectile::GetDamage() 
{
	return m_damage;
}