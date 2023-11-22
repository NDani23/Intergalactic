#pragma once

#include "HitBox.h"

namespace AAB
{
	bool Collide(std::vector<HitBox>&, std::vector<HitBox>&);
	bool Collide(std::vector<HitBox>&, glm::vec3&);
}

