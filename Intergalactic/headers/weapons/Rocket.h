#pragma once

#include "Projectile.h"
#include "headers/scenes/TailFire.h"

class Rocket : public Projectile
{
private:
	float m_travelDistance;
	Entity* m_target;
	ParticleSystem* m_particleSystem;
	TailFire m_TailFire;
public:
	Rocket();
	Rocket(glm::vec3, Entity*, ParticleSystem*);
	bool Update(const float&) override;
	bool CheckHit(Entity*) override;
};