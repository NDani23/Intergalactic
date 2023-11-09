#pragma once

#include "Simplex.h"
#include "MeshCollider.h"

namespace GJK
{
	bool Collide(MeshCollider&, MeshCollider&);
	bool NextSimplex(Simplex&, glm::vec3&);
	bool Line(Simplex&, glm::vec3&);
	bool Triangle(Simplex&, glm::vec3&);
	bool Tetrahedron(Simplex&, glm::vec3&);

	glm::vec3 Support(MeshCollider&, MeshCollider&, glm::vec3);
	bool SameDirection(glm::vec3&, glm::vec3&);
}
