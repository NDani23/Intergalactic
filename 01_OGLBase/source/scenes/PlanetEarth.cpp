#include "../../headers/scenes/PlanetEarth.h"

PlanetEarth::PlanetEarth(std::vector<std::unique_ptr<Projectile>>* projectiles, Player* player)
{
	m_name = "Planet Earth";

	m_player = player;
	m_projectiles = projectiles;

	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "shaders/PlanetEarthBaseVert.vert"},
		{ GL_FRAGMENT_SHADER, "shaders/PlanetEarthBaseFrag.frag"}
		});

	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_norm" },
		{ 2, "vs_in_tex" },
		});

	m_program.LinkProgram();

	m_skyBoxProgram.AttachShaders({
		{ GL_VERTEX_SHADER, "shaders/PlanetEarthSkyBox.vert"},
		{ GL_FRAGMENT_SHADER, "shaders/PlanetEarthSkyBox.frag"}
		});

	m_skyBoxProgram.BindAttribLocations({
		{ 0, "vs_in_pos" },
		});

	m_skyBoxProgram.LinkProgram();

	SetSkyBox("assets/PlanetEarth/xpos.png",
			  "assets/PlanetEarth/xneg.png",
		      "assets/PlanetEarth/ypos.png",
		      "assets/PlanetEarth/yneg.png",
			  "assets/PlanetEarth/zpos.png",
			  "assets/PlanetEarth/zneg.png");
}

void PlanetEarth::LoadScene()
{

	m_Entities.clear();
	m_enemySpawnPoints.clear();

	std::shared_ptr<Entity> base = std::make_shared<Entity>("assets/PlanetEarth/desert_base.obj", glm::vec3(0, m_Floor.GetZCoord(0.f, 2000.f) + 10.f, 2000), "assets/PlanetEarth/desert_base_tex.png");
	base->GetHitboxes().clear();
	base->AddHitBox({ base->GetPos() - glm::vec3(0.f, 40.f, 0.f), {400.0f, 100.0f, 400.0f} });
	base->AddHitBox({ base->GetPos() + glm::vec3(0.f, 25.f, 130.f), {270.0f, 50.0f, 50.0f} });
	base->AddHitBox({ base->GetPos() + glm::vec3(130.f, 25.f, -20.f), {50.0f, 50.0f, 250.0f} });
	base->AddHitBox({ base->GetPos() + glm::vec3(-115.f, 25.f, -20.f), {50.0f, 50.0f, 250.0f} });
	AddEntity(base);

	AddEntity(std::make_shared<Entity>("assets/PlanetEarth/bunker_exit.obj", base->GetPos(), "assets/PlanetEarth/bunker_exit_tex.png", Dimensions{100.0f, 70.0f, 100.0f}));

	AddEntity(std::make_shared<Entity>("assets/PlanetEarth/ballon.obj", glm::vec3(-1000.f, 800.f, 800.f), "assets/PlanetEarth/ballon_tex1.png", Dimensions{30.0f, 40.0f, 30.0f}));
	AddEntity(std::make_shared<Entity>("assets/PlanetEarth/ballon.obj", glm::vec3(-950.f, 600.f, 1000.f), "assets/PlanetEarth/ballon_tex1.png", Dimensions{ 30.0f, 40.0f, 30.0f }));
	AddEntity(std::make_shared<Entity>("assets/PlanetEarth/ballon.obj", glm::vec3(-1100.f, 700.f, 1200.f), "assets/PlanetEarth/ballon_tex1.png", Dimensions{ 30.0f, 40.0f, 30.0f }));
	AddEntity(std::make_shared<Entity>("assets/PlanetEarth/ballon.obj", glm::vec3(-820.f, 900.f, 750.f), "assets/PlanetEarth/ballon_tex2.png", Dimensions{ 30.0f, 40.0f, 30.0f }));
	AddEntity(std::make_shared<Entity>("assets/PlanetEarth/ballon.obj", glm::vec3(-950.f, 750.f, 900.f), "assets/PlanetEarth/ballon_tex2.png", Dimensions{ 30.0f, 40.0f, 30.0f }));
	AddEntity(std::make_shared<Entity>("assets/PlanetEarth/ballon.obj", glm::vec3(-800.f, 830.f, 1100.f), "assets/PlanetEarth/ballon_tex2.png", Dimensions{ 30.0f, 40.0f, 30.0f }));
	AddEntity(std::make_shared<Entity>("assets/PlanetEarth/ballon.obj", glm::vec3(-980.f, 610.f, 770.f), "assets/PlanetEarth/ballon_tex2.png", Dimensions{ 30.0f, 40.0f, 30.0f }));
	AddEntity(std::make_shared<Entity>("assets/PlanetEarth/ballon.obj", glm::vec3(-780.f, 550.f, 1130.f), "assets/PlanetEarth/ballon_tex3.png", Dimensions{ 30.0f, 40.0f, 30.0f }));
	AddEntity(std::make_shared<Entity>("assets/PlanetEarth/ballon.obj", glm::vec3(-1200.f, 700.f, 1000.f), "assets/PlanetEarth/ballon_tex3.png", Dimensions{ 30.0f, 40.0f, 30.0f }));
	AddEntity(std::make_shared<Entity>("assets/PlanetEarth/ballon.obj", glm::vec3(-960.f, 910.f, 950.f), "assets/PlanetEarth/ballon_tex3.png", Dimensions{ 30.0f, 40.0f, 30.0f }));

	std::shared_ptr<Entity> pyramid1 = std::make_shared<Entity>("assets/PlanetEarth/pyramid.obj", glm::vec3(2000.f, 120.f, 1600.f), "assets/PlanetEarth/pyramid_tex.png");
	pyramid1->GetHitboxes().clear();
	pyramid1->AddHitBox({ pyramid1->GetPos() + glm::vec3(0.f, -150.f, 0.f), {380.0f, 50.0f, 380.0f} });
	pyramid1->AddHitBox({ pyramid1->GetPos() + glm::vec3(0.f, -100.f, 0.f), {315.0f, 50.0f, 315.0f} });
	pyramid1->AddHitBox({ pyramid1->GetPos() + glm::vec3(0.f, -50.f, 0.f), {270.0f, 50.0f, 270.0f} });
	pyramid1->AddHitBox({ pyramid1->GetPos() + glm::vec3(0.f, 0.f, 0.f), {230.0f, 50.0f, 230.0f} });
	pyramid1->AddHitBox({ pyramid1->GetPos() + glm::vec3(0.f, 50.f, 0.f), {180.0f, 50.0f, 180.0f} });
	pyramid1->AddHitBox({ pyramid1->GetPos() + glm::vec3(0.f, 100.f, 0.f), {135.0f, 50.0f, 135.0f} });
	pyramid1->AddHitBox({ pyramid1->GetPos() + glm::vec3(0.f, 150.f, 0.f), {90.0f, 50.0f, 90.0f} });
	pyramid1->AddHitBox({ pyramid1->GetPos() + glm::vec3(0.f, 185.f, 0.f), {50.0f, 25.0f, 50.0f} });
	AddEntity(pyramid1);

	std::shared_ptr<Entity> pyramid2 = std::make_shared<Entity>("assets/PlanetEarth/pyramid.obj", glm::vec3(2400.f, 90.f, 1300.f), glm::scale(glm::vec3(0.75f, 0.75f, 0.75f)), "assets/PlanetEarth/pyramid_tex.png");
	pyramid2->GetHitboxes().clear();
	pyramid2->AddHitBox({ pyramid2->GetPos() + glm::vec3(0.f, -100.f, 0.f), {280.0f, 40.0f, 280.0f} });
	pyramid2->AddHitBox({ pyramid2->GetPos() + glm::vec3(0.f, -60.f, 0.f), {230.0f, 40.0f, 230.0f} });
	pyramid2->AddHitBox({ pyramid2->GetPos() + glm::vec3(0.f, -20.f, 0.f), {185.0f, 40.0f, 185.0f} });
	pyramid2->AddHitBox({ pyramid2->GetPos() + glm::vec3(0.f, 20.f, 0.f), {150.0f, 40.0f, 150.0f} });
	pyramid2->AddHitBox({ pyramid2->GetPos() + glm::vec3(0.f, 60.f, 0.f), {120.0f, 40.0f, 120.0f} });
	pyramid2->AddHitBox({ pyramid2->GetPos() + glm::vec3(0.f, 100.f, 0.f), {90.0f, 40.0f, 90.0f} });
	pyramid2->AddHitBox({ pyramid2->GetPos() + glm::vec3(0.f, 135.f, 0.f), {50.0f, 30.0f, 50.0f} });
	AddEntity(pyramid2);

	std::shared_ptr<Entity> pyramid3 = std::make_shared<Entity>("assets/PlanetEarth/pyramid.obj", glm::vec3(2680.f, 50.f, 1050.f), glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)), "assets/PlanetEarth/pyramid_tex.png", Dimensions{ 30.0f, 40.0f, 30.0f });
	pyramid3->GetHitboxes().clear();
	pyramid3->AddHitBox({ pyramid3->GetPos() + glm::vec3(0.f, -60.f, 0.f), {180.0f, 40.0f, 180.0f} });
	pyramid3->AddHitBox({ pyramid3->GetPos() + glm::vec3(0.f, -20.f, 0.f), {140.0f, 40.0f, 140.0f} });
	pyramid3->AddHitBox({pyramid3->GetPos() + glm::vec3(0.f, 20.f, 0.f), {100.0f, 40.0f, 100.0f}});
	pyramid3->AddHitBox({pyramid3->GetPos() + glm::vec3(0.f, 55.f, 0.f), {70.0f, 30.0f, 70.0f}});
	pyramid3->AddHitBox({ pyramid3->GetPos() + glm::vec3(0.f, 85.f, 0.f), {35.0f, 30.0f, 35.0f} });
	AddEntity(pyramid3);

	AddEntity(std::make_shared<Turret>(Turret(base->GetPos() + glm::vec3(-130, 55, -15), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(base->GetPos() + glm::vec3(-87, 55, -130), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(base->GetPos() + glm::vec3(-115, 55, 105), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(base->GetPos() + glm::vec3(0, 55, 142), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(base->GetPos() + glm::vec3(95, 55, -130), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(base->GetPos() + glm::vec3(128, 55, 68), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(base->GetPos() + glm::vec3(140, 55, -47), m_player, m_projectiles)));

	AddEntity(std::make_shared<Entity>("assets/PlanetEarth/rock.obj", glm::vec3(50, m_Floor.GetZCoord(50.f, 100.f) + 2.f, 100), "assets/PlanetEarth/rock_tex.png", Dimensions{13.0f, 13.0f, 13.0f}));

	AddEntity(std::make_shared<Entity>("assets/PlanetEarth/rock.obj", glm::vec3(800, m_Floor.GetZCoord(800.f, 700.f) + 2.f, 700), "assets/PlanetEarth/rock_tex.png", Dimensions{ 13.0f, 13.0f, 13.0f }));
	AddEntity(std::make_shared<Entity>("assets/PlanetEarth/rock.obj", glm::vec3(1000, m_Floor.GetZCoord(1000.f, 500.f) + 2.f, 500), "assets/PlanetEarth/rock_tex.png", Dimensions{ 13.0f, 13.0f, 13.0f }));
	AddEntity(std::make_shared<Entity>("assets/PlanetEarth/rock.obj", glm::vec3(1200, m_Floor.GetZCoord(1200.f, -300.f) + 2.f, -300), "assets/PlanetEarth/rock_tex.png", Dimensions{ 13.0f, 13.0f, 13.0f }));
	AddEntity(std::make_shared<Entity>("assets/PlanetEarth/rock.obj", glm::vec3(-100, m_Floor.GetZCoord(-100.f, -100.f) + 2.f, -100), "assets/PlanetEarth/rock_tex.png", Dimensions{ 13.0f, 13.0f, 13.0f }));


	std::shared_ptr<Entity> wrecked_ship = std::make_shared<Entity>("assets/PlanetEarth/Mothership2.obj", glm::vec3(-800, m_Floor.GetZCoord(-800.f, -1500.f) - 15.f, -1000), glm::rotate(0.8f, glm::vec3(-1, 0, 0)), "assets/PlanetEarth/Mothership_tex.png");
	wrecked_ship->GetHitboxes().clear();
	wrecked_ship->AddHitBox({ wrecked_ship->GetPos() + glm::vec3(+15.f, 25.f, +10.f), {320.0f, 45.0f, 95.f} });
	wrecked_ship->AddHitBox({ wrecked_ship->GetPos() + glm::vec3(-230.f, 70.f, +30.f), {200.0f, 120.0f, 160.f} });
	AddEntity(wrecked_ship);

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