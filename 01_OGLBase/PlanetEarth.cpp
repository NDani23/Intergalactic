#include "PlanetEarth.h"


PlanetEarth::PlanetEarth(std::vector<std::unique_ptr<Projectile>>* projectiles, Player* player)
{
	m_name = "Planet Earth";

	m_player = player;
	m_projectiles = projectiles;

	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "PlanetEarthBaseVert.vert"},
		{ GL_FRAGMENT_SHADER, "PlanetEarthBaseFrag.frag"}
		});

	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_norm" },
		{ 2, "vs_in_tex" },
		});

	m_program.LinkProgram();

	m_skyBoxProgram.AttachShaders({
		{ GL_VERTEX_SHADER, "PlanetEarthSkyBox.vert"},
		{ GL_FRAGMENT_SHADER, "PlanetEarthSkyBox.frag"}
		});

	m_skyBoxProgram.BindAttribLocations({
		{ 0, "vs_in_pos" },
		});

	m_skyBoxProgram.LinkProgram();

	SetSkyBox("assets/xpos.png", "assets/xneg.png", "assets/ypos.png", "assets/yneg.png", "assets/zpos.png", "assets/zneg.png");
}

void PlanetEarth::LoadMap()
{
	AddEntity(std::make_shared<Entity>("assets/ufo.obj", glm::vec3(500, 200, 1000), "assets/ufo_tex.png", Dimensions{ 50.0f, 17.0f, 50.0f }));
}

void PlanetEarth::DrawEntities(glm::mat4& viewproj, GameState& state)
{
	m_Floor.DrawFloor(viewproj, m_player);
	m_program.Use();
	if (state.play)
	{
		for (std::shared_ptr<Entity>& entity : m_Entities)
		{
			m_program.SetUniform("playerPos", m_player->GetPos());
			entity->DrawMesh(m_program, viewproj);
		}
	}
	m_program.Unuse();
}