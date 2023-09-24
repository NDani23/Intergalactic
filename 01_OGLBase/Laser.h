#pragma once

#include "Projectile.h"
#include "Updatable.h"

class Laser : public Projectile
{
public:
	Laser(glm::vec3, glm::vec3);
};
