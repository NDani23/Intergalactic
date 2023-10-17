#pragma once

#include "Projectile.h"

class Mine : public Projectile
{
public:
	Mine();
	Mine(glm::vec3&);
	bool Update(const float&) override;
	bool CheckHit(Entity*) override;
};