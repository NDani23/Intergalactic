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

	bool Update(const float& delta) override;
	void Shoot() override;
	void DrawMesh(ProgramObject&, glm::mat4&) override;

private:
	HitBox UpdateDimensions() override;
	static int FirstInit();

	void CalcBaseDir(glm::vec3&) override;
	bool CalcAvoidObjectsVec(glm::vec3&) override;
	bool CalcAvoidFloorVec(glm::vec3&) override;
	void RegulateTurnDegree(glm::vec3&) override;
};
