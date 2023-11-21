#include "../../headers/collision_detection/AAB.h"

bool AAB::Collide(Entity& EntityA, Entity& EntityB)
{
	for (HitBox& hitboxA : EntityA.GetHitboxes())
	{
		for (HitBox& hitboxB : EntityB.GetHitboxes())
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

bool AAB::Collide(Entity& Entity, glm::vec3& Point)
{
	glm::vec3 distance_vec = Entity.GetPos() - Point;

	for (HitBox& hitbox : Entity.GetHitboxes())
	{
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