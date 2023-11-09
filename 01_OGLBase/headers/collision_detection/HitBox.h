#pragma once

#include <glm/glm.hpp>

#include "../../includes/ProgramObject.h"
#include "../../includes/TextureObject.h"
#include "Dimensions.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

struct HitBox {
	glm::vec3 Position;
	Dimensions dimensions;

	void Draw(ProgramObject&, glm::mat4&);
};