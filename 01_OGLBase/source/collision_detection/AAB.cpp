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

glm::vec3 AAB::RayIntersection(HitBox& hitbox, Ray& ray)
{

	//glm::vec3 to_enemy = m_position - hitbox.Position;

	float closest_t = FLT_MAX;
	glm::vec3 closest_normal = glm::vec3(0.f, 0.f, 0.f);
	//metszés vizsgálat:
	//hitbox felsõ/alsó lapja
	if (ray.direction.y != 0)
	{
		float y_0 = hitbox.Position.y + hitbox.dimensions.height * 0.5f;
		float t = (y_0 - ray.origin.y) / ray.origin.y;

		if (t > 0)
		{
			glm::vec3 hit_point = ray.origin + t * ray.direction;
			glm::vec3 distance_vec = hit_point - hitbox.Position;

			if (abs(distance_vec.x) < hitbox.dimensions.width * 0.8f
				&& abs(distance_vec.z) < hitbox.dimensions.length * 0.8f
				&& t < closest_t)
			{
				closest_t = t;
				//closest_normal = glm::vec3(0, 1, 0);

				if (abs(distance_vec.x) / hitbox.dimensions.width * 0.8f > abs(distance_vec.z) / hitbox.dimensions.length * 0.8f)
				{
					closest_normal = glm::sign(distance_vec.x) == 0 ?  glm::vec3(0, 0, 1) : glm::sign(distance_vec.x) * glm::vec3(1, 0, 0);
				}
				else
				{
					closest_normal = glm::sign(distance_vec.z) == 0 ? glm::vec3(0, 0, 1) : glm::sign(distance_vec.z) * glm::vec3(0, 0, 1);
				}
				//return closest_normal;
			}
		}

		y_0 = hitbox.Position.y - hitbox.dimensions.height * 0.5f;
		t = (y_0 - ray.origin.y) / ray.direction.y;

		if (t > 0)
		{
			glm::vec3 hit_point = ray.origin + t * ray.direction;
			glm::vec3 distance_vec = hit_point - hitbox.Position;

			if (abs(distance_vec.x) < hitbox.dimensions.width * 0.8f
				&& abs(distance_vec.z) < hitbox.dimensions.length * 0.8f
				&& t < closest_t)
			{
				closest_t = t;
				//closest_normal = glm::vec3(0, -1, 0);
				//return closest_normal;
				if (abs(distance_vec.x) / hitbox.dimensions.width * 0.8f > abs(distance_vec.z) / hitbox.dimensions.length * 0.8f)
				{
					closest_normal = glm::sign(distance_vec.x) == 0 ? glm::vec3(0, 0, 1) : glm::sign(distance_vec.x) * glm::vec3(1, 0, 0);
				}
				else
				{
					closest_normal = glm::sign(distance_vec.z) == 0 ? glm::vec3(0, 0, 1) : glm::sign(distance_vec.z) * glm::vec3(0, 0, 1);
				}
			}

		}
	}

	//hitbox szélsõ lapja
	if (ray.direction.x != 0)
	{
		float x_0 = hitbox.Position.x + hitbox.dimensions.width * 0.5f;
		float t = (x_0 - ray.origin.x) / ray.direction.x;

		if (t > 0)
		{
			glm::vec3 hit_point = ray.origin + t * ray.direction;
			glm::vec3 distance_vec = hit_point - hitbox.Position;

			if (abs(distance_vec.y) < hitbox.dimensions.height * 0.8f
				&& abs(distance_vec.z) < hitbox.dimensions.length * 0.8f
				&& t < closest_t)
			{
				closest_t = t;
				//closest_normal = glm::vec3(1, 0, 0);
				//return closest_normal;
				if (abs(distance_vec.y) / hitbox.dimensions.height * 0.8f > abs(distance_vec.z) / hitbox.dimensions.length * 0.8f)
				{
					closest_normal = glm::sign(distance_vec.y) == 0 ? glm::vec3(0, 0, 1) : glm::sign(distance_vec.y) * glm::vec3(0, 1, 0);
				}
				else
				{
					closest_normal = glm::sign(distance_vec.z) == 0 ? glm::vec3(0, 0, 1) : glm::sign(distance_vec.z) * glm::vec3(0, 0, 1);
				}
			}
		}

		x_0 = hitbox.Position.x - hitbox.dimensions.width * 0.5f;
		t = (x_0 - ray.origin.x) / ray.direction.x;

		if (t > 0)
		{
			glm::vec3 hit_point = ray.origin + t * ray.direction;
			glm::vec3 distance_vec = hit_point - hitbox.Position;

			if (abs(distance_vec.y) < hitbox.dimensions.height * 0.8f
				&& abs(distance_vec.z) < hitbox.dimensions.length * 0.8f
				&& t < closest_t)
			{
				closest_t = t;
				//closest_normal = glm::vec3(-1, 0, 0);
				//return closest_normal;

				if (abs(distance_vec.y) / hitbox.dimensions.height * 0.8f > abs(distance_vec.z) / hitbox.dimensions.length * 0.8f)
				{
					closest_normal = glm::sign(distance_vec.y) == 0 ? glm::vec3(0, 1, 0) : glm::sign(distance_vec.y) * glm::vec3(0, 1, 0);
				}
				else
				{
					closest_normal = glm::sign(distance_vec.z) == 0 ? glm::vec3(0, 1, 0) : glm::sign(distance_vec.z) * glm::vec3(0, 0, 1);
				}
			}
		}
	}

	//metszés vizsgálat:
	//hitbox hosszanti lapja
	if (ray.direction.z != 0)
	{

		float z_0 = hitbox.Position.z - hitbox.dimensions.length * 0.5f;
		float t = (z_0 - ray.origin.z) / ray.direction.z;

		if (t > 0)
		{
			glm::vec3 hit_point = ray.origin + t * ray.direction;
			glm::vec3 distance_vec = hit_point - hitbox.Position;

			if (abs(distance_vec.x) < hitbox.dimensions.width * 0.8f
				&& abs(distance_vec.y) < hitbox.dimensions.height * 0.8f
				&& t < closest_t)
			{
				closest_t = t;
				//closest_normal = glm::vec3(0, 0, -1);
				//return closest_normal;

				if (abs(distance_vec.y) / hitbox.dimensions.height * 0.8f > abs(distance_vec.y) / hitbox.dimensions.width * 0.8f)
				{
					closest_normal = glm::sign(distance_vec.y) == 0 ? glm::vec3(0, 1, 0) : glm::sign(distance_vec.y) * glm::vec3(0, 1, 0);
				}
				else
				{
					closest_normal = glm::sign(distance_vec.x) == 0 ? glm::vec3(0, 1, 0) : glm::sign(distance_vec.x) * glm::vec3(1, 0, 0);
				}
			}
		}

		z_0 = hitbox.Position.z + hitbox.dimensions.length * 0.5f;
		t = (z_0 - ray.origin.z) / ray.direction.z;

		if (t > 0)
		{
			glm::vec3 hit_point = ray.origin + t * ray.direction;
			glm::vec3 distance_vec = hit_point - hitbox.Position;

			if (abs(distance_vec.x) < hitbox.dimensions.width * 0.8f
				&& abs(distance_vec.y) < hitbox.dimensions.height * 0.8f
				&& t < closest_t)
			{
				closest_t = t;
				//closest_normal = glm::vec3(0, 0, 1);
				//return closest_normal;
				if (abs(distance_vec.y) / hitbox.dimensions.height * 0.8f > abs(distance_vec.y) / hitbox.dimensions.width * 0.8f)
				{
					closest_normal = glm::sign(distance_vec.y) == 0 ? glm::vec3(0, 1, 0) : glm::sign(distance_vec.y) * glm::vec3(0, 1, 0);
				}
				else
				{
					closest_normal = glm::sign(distance_vec.x) == 0 ? glm::vec3(0, 1, 0) : glm::sign(distance_vec.x) * glm::vec3(1, 0, 0);
				}
			}
		}

	}

	return closest_normal;
}