#pragma once

#include "Enemy.h"
#include <math.h>

class Falcon : public Enemy
{
private:
	static std::unique_ptr<Mesh> m_static_mesh;
	static Texture2D m_static_tex;
public:
	Falcon();
	Falcon(glm::vec3, Player*, std::vector<std::unique_ptr<Projectile>>*, Map*);

	void Shoot() override;
	void DrawMesh(ProgramObject&, glm::mat4&) override;

private:
	HitBox UpdateDimensions() override;
	static int FirstInit();
};
