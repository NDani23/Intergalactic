#include "../headers/DeepSpace.h"

DeepSpace::DeepSpace(std::vector<std::unique_ptr<Projectile>>* projectiles, Player* player)
{
	m_name = "Deep Space";
	m_player = player;
	m_projectiles = projectiles;

	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "shaders/myVert.vert"},
		{ GL_FRAGMENT_SHADER, "shaders/myFrag.frag"}
		});

	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_norm" },
		{ 2, "vs_in_tex" },
		});

	m_program.LinkProgram();

	m_skyBoxProgram.AttachShaders({
		{ GL_VERTEX_SHADER, "shaders/skybox.vert"},
		{ GL_FRAGMENT_SHADER, "shaders/skybox.frag"}
		});

	m_skyBoxProgram.BindAttribLocations({
		{ 0, "vs_in_pos" },
		});

	m_skyBoxProgram.LinkProgram();

	SetSkyBox("assets/DeepSpace/right.png",
			  "assets/DeepSpace/left.png",
			  "assets/DeepSpace/top.png",
		      "assets/DeepSpace/bottom.png",
		      "assets/DeepSpace/front.png",
		      "assets/DeepSpace/back.png");
}

void DeepSpace::LoadMap()
{
	m_Entities.clear();
	m_enemySpawnPoints.clear();

	AddEntity(std::make_shared<Entity>("assets/DeepSpace/ufo.obj", glm::vec3(1000, 200, 1000), "assets/DeepSpace/ufo_tex.png", Dimensions{ 50.0f, 17.0f, 50.0f }));

	AddEntity(std::make_shared<Entity>("assets/DeepSpace/satelite.obj", glm::vec3(-200, 1200, 700), "assets/DeepSpace/satelite_tex.png", Dimensions{ 20.0f, 17.0f, 20.0f }));

	AddEntity(std::make_shared<Turret>(Turret(glm::vec3(-70, 50, 2000), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(glm::vec3(-100, 0, 1950), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(glm::vec3(-90, -10, 1900), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(glm::vec3(50, 10, 1950), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(glm::vec3(80, -50, 2000), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(glm::vec3(100, 20, 1850), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(glm::vec3(50, 30, 2050), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(glm::vec3(-60, -50, 2010), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(glm::vec3(50, 200, 1970), m_player, m_projectiles)));

	std::shared_ptr<Entity> gate = std::make_shared<Entity>("assets/DeepSpace/gate.obj", glm::vec3(0, -500, 1200), "assets/DeepSpace/gate_tex.png");
	gate->GetHitboxes().clear();
	gate->AddHitBox({ gate->GetPos() + glm::vec3(20, 0, 0), {10.0f, 50.0f, 10.0f} });
	gate->AddHitBox({ gate->GetPos() + glm::vec3(-20, 0, 0), {10.0f, 50.0f, 10.0f} });
	gate->AddHitBox({ gate->GetPos() + glm::vec3(0, 20, 0), {50.0f, 10.0f, 10.0f} });
	gate->AddHitBox({ gate->GetPos() + glm::vec3(0, -20, 0), {50.0f, 10.0f, 10.0f} });
	AddEntity(gate);

	std::shared_ptr<Entity> station = std::make_shared<Entity>("assets/DeepSpace/station.obj", glm::vec3(0, 0, 2000.f), "assets/DeepSpace/Station_tex.png");
	station->GetHitboxes().clear();
	station->AddHitBox({ station->GetPos() + glm::vec3(0.f, 20.f, 0.f), {40.0f, 400.0f, 40.0f} });
	station->AddHitBox({ station->GetPos() + glm::vec3(0.f, 130.f, 0.f), {260.0f, 40.0f, 260.0f} });
	station->AddHitBox({ station->GetPos() + glm::vec3(0.f, -70.f, 0.f), {140.0f, 10.0f, 140.0f} });
	AddEntity(station);

	CreateMeteorField();

	m_enemySpawnPoints.emplace_back(std::make_unique<EnemySpawnPoint>(glm::vec3(0, 30, 1900), m_player, m_projectiles, this));
}

void DeepSpace::CreateMeteorField()
{
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-800, 300, 1000), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-820, 300, 1100), glm::rotate(1.0f, glm::vec3(1, 0, 1)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-910, 310, 1150), glm::rotate(2.3f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(1.2f, 1.2f, 1.2f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 32.4f, 32.4f, 32.5f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-900, 400, 1200), glm::rotate(1.3f, glm::vec3(0, 0, 1)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-750, 250, 800), glm::rotate(1.0f, glm::vec3(0, 1, 1)) * glm::scale(glm::vec3(0.8f, 0.8f, 0.8f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 21.6f, 21.6f, 21.6f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-700, 280, 1000), glm::rotate(0.6f, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2.0f, 2.0f, 2.0f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 54.0f, 54.0f, 54.0f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-800, 500, 1050), glm::scale(glm::vec3(1.5f, 1.5f, 1.5f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 40.5f, 40.5f, 40.5f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-1000, 500, 750), glm::rotate(-1.0f, glm::vec3(0, 1, 1)) * glm::scale(glm::vec3(0.8f, 0.8f, 0.8f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 21.6f, 21.6f, 21.6f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-950, 400, 800), glm::rotate(-1.6f, glm::vec3(0, 0, 1)) * glm::scale(glm::vec3(1.3f, 1.3f, 1.3f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 35.1f, 35.1f, 35.1f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-900, 350, 900), glm::rotate(0.3f, glm::vec3(1, 1, 0)) * glm::scale(glm::vec3(0.8f, 0.8f, 0.8f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 21.6f, 21.6f, 21.6f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-850, 325, 930), glm::rotate(0.8f, glm::vec3(0, 1, 0)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-820, 400, 950), glm::rotate(2.3f, glm::vec3(1, 0, 1)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-950, 280, 910), glm::rotate(1.5f, glm::vec3(1, 1, 0)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-800, 200, 1000), glm::rotate(0.3f, glm::vec3(0, 1, 1)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-780, 340, 980), glm::rotate(1.2f, glm::vec3(0, 0, 1)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-980, 470, 800), glm::rotate(-0.5f, glm::vec3(1, 1, 1)) * glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 13.5f, 13.5f, 13.5f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-950, 450, 780), glm::rotate(-1.3f, glm::vec3(0, 1, 1)) * glm::scale(glm::vec3(0.6f, 0.6f, 0.6f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 16.2f, 16.2f, 16.2f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-900, 470, 750), glm::rotate(0.8f, glm::vec3(1, 1, 1)) * glm::scale(glm::vec3(1.7f, 1.7f, 1.7f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 45.9f, 45.9f, 45.9f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-860, 420, 830), glm::rotate(-0.5f, glm::vec3(0, 0, 1)) * glm::scale(glm::vec3(0.8f, 0.8f, 0.8f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 21.6f, 21.6f, 21.6f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-830, 390, 870), glm::rotate(2.2f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(2.0f, 2.0f, 2.0f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 54.0f, 54.0f, 54.0f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-788, 335, 930), glm::rotate(1.3f, glm::vec3(0, 1, 1)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-700, 300, 1100), glm::rotate(0.5f, glm::vec3(1, 1, 1)) * glm::scale(glm::vec3(1.3f, 1.3f, 1.3f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 35.1f, 35.1f, 35.1f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-750, 280, 1000), glm::rotate(-1.3f, glm::vec3(0, 1, 1)) * glm::scale(glm::vec3(0.3f, 0.3f, 0.3f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 8.1f, 8.1f, 8.1f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-800, 350, 1150), glm::rotate(0.8f, glm::vec3(1, 1, 1)) * glm::scale(glm::vec3(0.4f, 0.4f, 0.4f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 10.8f, 10.8f, 10.8f }));


	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(830, 1500, 870), glm::rotate(2.2f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(5.0f, 5.0f, 5.0f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 130.0f, 130.0f, 130.0f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(300, -700, -3000), glm::rotate(2.2f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(5.0f, 5.0f, 5.0f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 130.0f, 130.0f, 130.0f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(-2000, -2000, 300), glm::rotate(2.2f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(5.0f, 5.0f, 5.0f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 130.0f, 130.0f, 130.0f }));
	AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(4000, 3000, 5000), glm::rotate(2.2f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(5.0f, 5.0f, 5.0f)), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 130.0f, 130.0f, 130.0f }));
	
}

void DeepSpace::DrawEntities(glm::mat4& viewproj, GameState& state)
{
	if (state.play)
	{
		for (std::shared_ptr<Entity>& entity : m_Entities)
		{
			entity->DrawMesh(m_program, viewproj);
		}
	}
}