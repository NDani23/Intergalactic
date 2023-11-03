#pragma once

#include "Enemy.h"

class SaR : public Enemy
{
private:
	static std::unique_ptr<Mesh> m_static_mesh;
	static Texture2D sar_static_tex;
public:
	SaR();
	SaR(glm::vec3, Player*, std::vector<std::unique_ptr<Projectile>>*, Map*);

	void Shoot() override;
	void DrawMesh(ProgramObject&, glm::mat4&) override;
	std::unique_ptr<Mesh>& GetMesh() override;

private:
	HitBox UpdateDimensions() override;
	static int FirstInit();
};
