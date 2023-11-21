#pragma once

#include "../Entity.h"
#include "HitBox.h"

namespace AAB
{
	bool Collide(Entity&, Entity&);
	bool Collide(Entity&, glm::vec3&);
}

