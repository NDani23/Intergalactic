#include "../../headers/weapons/Projectile.h"

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

void Projectile::SetPosition(const glm::vec3& newPos)
{
	m_position = newPos;
};

bool Projectile::operator==(const Projectile& rhs) const
{
	return  this == &rhs;
};

bool Projectile::CheckHit(Entity* entity)
{
	return false;
}