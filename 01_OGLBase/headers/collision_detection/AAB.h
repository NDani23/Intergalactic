#pragma once

#include "HitBox.h"

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;
};

namespace AAB
{
	bool Collide(std::vector<HitBox>&, std::vector<HitBox>&);
	bool Collide(std::vector<HitBox>&, glm::vec3&);
	glm::vec3 RayIntersection(HitBox&, Ray&);
}

