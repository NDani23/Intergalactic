#pragma once

#include <glm/glm.hpp>

#include "includes/ProgramObject.h"
#include "includes/TextureObject.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

struct Dimensions {
	float width = 0.f;
	float height = 0.f;
	float length = 0.f;
};

struct HitBox {

	glm::vec3 Position;
	Dimensions dimensions;

	void Draw(ProgramObject&, glm::mat4&);
};