#pragma once

#include "Projectile.h"

class Laser : public Projectile
{
private:
	float m_travelDistance;
public:
	Laser(glm::vec3, glm::vec3);
	Laser(glm::vec3, glm::vec3, int);
	bool Update(const float&) override;
};
