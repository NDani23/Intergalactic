#include "../headers/GJK.h"


bool GJK::Collide(MeshCollider& objA, MeshCollider& objB)
{
	glm::vec3 support = Support(objA, objB, glm::vec3(1.f, 0.f, 0.f));

	Simplex points;
	points.push_front(support);

	glm::vec3 direction = -support;

	while (true)
	{
		support = Support(objA, objB, direction);

		if (glm::dot(support, direction) <= 0) return false;

		points.push_front(support);

		if (NextSimplex(points, direction)) return true;

	}

	return false;
}

bool GJK::NextSimplex(Simplex& points, glm::vec3& dir)
{
	switch (points.size())
	{
	case 2:
		return Line(points, dir);
	case 3:
		return Triangle(points, dir);
	case 4:
		return Tetrahedron(points, dir);
	}

	return false;
}

bool GJK::Line(Simplex& points, glm::vec3& dir)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];

	glm::vec3 ab = b - a;
	glm::vec3 ao = -a;

	if (SameDirection(ab, ao))
	{
		dir = glm::cross(glm::cross(ab, ao), ab);
	}
	else 
	{
		points = { a };
		dir = ao;
	}

	return false;
}

bool GJK::Triangle(Simplex& points, glm::vec3& dir)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];
	glm::vec3 c = points[2];

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ao = -a;

	glm::vec3 abc(glm::cross(ab, ac));

	if (SameDirection(glm::cross(abc, ac), ao))
	{
		if (SameDirection(ac, ao))
		{
			points = { a, c };
			dir = glm::cross(glm::cross(ac, ao), ac);
		}
		else
		{
			return Line(points = { a, b }, dir);
		}
	}
	else
	{
		if (SameDirection(glm::cross(ab, abc), ao))
		{
			return Line(points = { a, b }, dir);
		}
		else
		{
			if (SameDirection(abc, ao))
			{
				dir = abc;
			}
			else
			{
				points = { a, c, b };
				dir = -abc;
			}
		}
	}

	return false;
}

bool GJK::Tetrahedron(Simplex& points, glm::vec3& dir)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];
	glm::vec3 c = points[2];
	glm::vec3 d = points[3];

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ad = d - a;
	glm::vec3 ao = -a;

	glm::vec3 abc = glm::cross(ab, ac);
	glm::vec3 acd = glm::cross(ac, ad);
	glm::vec3 adb = glm::cross(ad, ab);

	if (SameDirection(abc, ao))
	{
		return Triangle(points = { a, b, c }, dir);
	}

	if (SameDirection(acd, ao))
	{
		return Triangle(points = { a, c, d }, dir);
	}

	if (SameDirection(adb, ao))
	{
		return Triangle(points = { a, d, b }, dir);
	}

	return true;
}

glm::vec3 GJK::Support(MeshCollider& colliderA, MeshCollider& colliderB, glm::vec3 dir)
{
	return colliderA.FindFurthestPoint(dir) 
		- colliderB.FindFurthestPoint(-dir);
}

bool GJK::SameDirection(glm::vec3& dir, glm::vec3& ao)
{
	return dot(dir, ao) > 0;
}