#pragma once

#include "Projectile.h"
#include "headers/scenes/ParticleSystem.h"

class Scene;

class Mine : public Projectile
{
private:
	ParticleProps m_explosionProp;
	Scene* m_Scene;
public:
	Mine();
	Mine(glm::vec3&, Scene*);
	bool Update(const float&) override;
	bool CheckHit(Entity*) override;
};