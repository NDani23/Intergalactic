#include "../../headers/collision_detection/AAB.h"

bool AAB::Collide(std::vector<HitBox>& hitboxesA, std::vector<HitBox>& hitboxesB)
{
	for (HitBox& hitboxA : hitboxesA)
	{
		for (HitBox& hitboxB : hitboxesB)
		{
			glm::vec3 distance_vec = hitboxA.Position - hitboxB.Position;

			if (abs(distance_vec.x) < std::max(hitboxA.dimensions.width / 2, hitboxB.dimensions.width / 2)
				&& abs(distance_vec.y) < std::max(hitboxA.dimensions.height / 2, hitboxB.dimensions.height / 2)
				&& abs(distance_vec.z) < std::max(hitboxA.dimensions.length / 2, hitboxB.dimensions.length / 2))
			{
				return true;
			}
		}
	}

	return false;
}

bool AAB::Collide(std::vector<HitBox>& hitboxes, glm::vec3& Point)
{

	for (HitBox& hitbox : hitboxes)
	{
		glm::vec3 distance_vec = hitbox.Position - Point;
		Dimensions hitbox_dims = hitbox.dimensions;

		if (abs(distance_vec.x) < hitbox_dims.width / 2
			&& abs(distance_vec.y) < hitbox_dims.height / 2
			&& abs(distance_vec.z) < hitbox_dims.length / 2)
		{
			return true;
		}
	}

	return false;
}