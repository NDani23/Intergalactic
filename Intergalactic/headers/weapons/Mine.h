#pragma once

#include "Projectile.h"
#include "headers/scenes/ParticleSystem.h"

class Mine : public Projectile
{
private:
	ParticleProps m_explosionProp;
	ParticleSystem* m_particleSystem;
public:
	Mine();
	Mine(glm::vec3&, ParticleSystem*);
	bool Update(const float&) override;
	bool CheckHit(Entity*) override;
};