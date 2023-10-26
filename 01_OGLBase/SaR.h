#pragma once

#include "Enemy.h"

class SaR : public Enemy
{
private:
	static std::unique_ptr<Mesh> m_static_mesh;
	static Texture2D m_static_tex;
public:
	SaR();
	SaR(glm::vec3, Entity*, std::vector<std::unique_ptr<Projectile>>*, Map*);

	bool Update(const float& delta) override;
	void Shoot() override;
	void DrawMesh(ProgramObject&, glm::mat4&) override;

private:
	HitBox UpdateDimensions() override;
	static int FirstInit();

	void CalcBaseDir() override;
	bool CalcAvoidObjectsVec() override;
	bool CalcAvoidFloorVec() override;
};
