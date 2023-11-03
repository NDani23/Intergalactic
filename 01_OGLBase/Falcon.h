#pragma once

#include "Enemy.h"

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
	std::unique_ptr<Mesh>& GetMesh() override;

private:
	HitBox UpdateDimensions() override;
	static int FirstInit();
};
