#pragma once

#include "Weapon.h"
#include "Laser.h"
#include "Map.h"

#include <algorithm>

class Enemy : public Weapon
{
private:
	static std::unique_ptr<Mesh> m_static_mesh;
	static Texture2D m_static_tex;
	int m_health;
	float m_speed;
	glm::vec3 m_up_vec;

	Entity* m_target;
	std::vector<std::unique_ptr<Projectile>>* m_projectiles;
	Map* m_Map;

public:
	Enemy();
	Enemy(glm::vec3, Entity*, std::vector<std::unique_ptr<Projectile>>*, Map*);
	bool Update(const float& delta) override;
	void Shoot();
	bool Hit(int) override;
	void SetTexture();
	bool IsTargetable() override;
	void DrawMesh(ProgramObject&, glm::mat4&) override;

private:
	HitBox UpdateDimensions();
	static int FirstInit();

};
