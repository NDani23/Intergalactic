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

	m_Entities.clear();
	m_enemySpawnPoints.clear();

	std::shared_ptr<Entity> base = std::make_shared<Entity>("assets/desert_base.obj", glm::vec3(0, m_Floor.GetZCoord(0.f, 2000.f) + 10.f, 2000), "assets/desert_base_tex.png");
	base->GetHitboxes().clear();
	base->AddHitBox({ base->GetPos() - glm::vec3(0.f, 40.f, 0.f), {400.0f, 100.0f, 400.0f} });
	base->AddHitBox({ base->GetPos() + glm::vec3(0.f, 25.f, 130.f), {270.0f, 50.0f, 50.0f} });
	base->AddHitBox({ base->GetPos() + glm::vec3(130.f, 25.f, -20.f), {50.0f, 50.0f, 250.0f} });
	base->AddHitBox({ base->GetPos() + glm::vec3(-115.f, 25.f, -20.f), {50.0f, 50.0f, 250.0f} });
	AddEntity(base);

	AddEntity(std::make_shared<Turret>(Turret(base->GetPos() + glm::vec3(-130, 55, -15), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(base->GetPos() + glm::vec3(-87, 55, -130), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(base->GetPos() + glm::vec3(-115, 55, 105), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(base->GetPos() + glm::vec3(0, 55, 142), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(base->GetPos() + glm::vec3(95, 55, -130), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(base->GetPos() + glm::vec3(128, 55, 68), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(base->GetPos() + glm::vec3(140, 55, -47), m_player, m_projectiles)));

	AddEntity(std::make_shared<Entity>("assets/rock.obj", glm::vec3(50, m_Floor.GetZCoord(50.f, 100.f) + 2.f, 100), "assets/rock_tex.png", Dimensions{13.0f, 13.0f, 13.0f}));

	AddEntity(std::make_shared<Entity>("assets/rock.obj", glm::vec3(800, m_Floor.GetZCoord(800.f, 700.f) + 2.f, 700), "assets/rock_tex.png", Dimensions{ 13.0f, 13.0f, 13.0f }));
	AddEntity(std::make_shared<Entity>("assets/rock.obj", glm::vec3(1000, m_Floor.GetZCoord(1000.f, 500.f) + 2.f, 500), "assets/rock_tex.png", Dimensions{ 13.0f, 13.0f, 13.0f }));
	AddEntity(std::make_shared<Entity>("assets/rock.obj", glm::vec3(1200, m_Floor.GetZCoord(1200.f, -300.f) + 2.f, -300), "assets/rock_tex.png", Dimensions{ 13.0f, 13.0f, 13.0f }));
	AddEntity(std::make_shared<Entity>("assets/rock.obj", glm::vec3(-100, m_Floor.GetZCoord(-100.f, -100.f) + 2.f, -100), "assets/rock_tex.png", Dimensions{ 13.0f, 13.0f, 13.0f }));

	m_enemySpawnPoints.emplace_back(std::make_unique<EnemySpawnPoint>(glm::vec3(0, 50.f, 2000), m_player, m_projectiles, this));
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

Floor* PlanetEarth::GetFloor()
{
	return &m_Floor;
}