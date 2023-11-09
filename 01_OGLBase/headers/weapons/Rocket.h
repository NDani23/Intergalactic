#pragma once

#include "Projectile.h"

class Rocket : public Projectile
{
private:
	float m_travelDistance;
	Entity* m_target;
public:
	Rocket();
	Rocket(glm::vec3, Entity*);
	bool Update(const float&) override;
	bool CheckHit(Entity*) override;
};