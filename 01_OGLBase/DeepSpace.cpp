#include "DeepSpace.h"


DeepSpace::DeepSpace(std::vector<std::unique_ptr<Projectile>>* projectiles, Player* player)
{
	m_name = "Deep Space";
	m_player = player;
	m_projectiles = projectiles;

	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
		});

	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_norm" },
		{ 2, "vs_in_tex" },
		});

	m_program.LinkProgram();

	m_skyBoxProgram.AttachShaders({
		{ GL_VERTEX_SHADER, "skybox.vert"},
		{ GL_FRAGMENT_SHADER, "skybox.frag"}
		});

	m_skyBoxProgram.BindAttribLocations({
		{ 0, "vs_in_pos" },
		});

	m_skyBoxProgram.LinkProgram();

	SetSkyBox("assets/right.png", "assets/left.png", "assets/top.png", "assets/bottom.png", "assets/front.png", "assets/back.png");
}

void DeepSpace::LoadMap()
{
	AddEntity(std::make_shared<Entity>("assets/ufo.obj", glm::vec3(500, 200, 1000), "assets/ufo_tex.png", Dimensions{ 50.0f, 17.0f, 50.0f }));

	AddEntity(std::make_shared<Turret>(Turret(glm::vec3(0, 50, 2000), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(glm::vec3(-100, 0, 1950), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(glm::vec3(-50, -10, 1900), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(glm::vec3(50, 10, 1950), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(glm::vec3(25, -50, 2000), m_player, m_projectiles)));
	AddEntity(std::make_shared<Turret>(Turret(glm::vec3(100, 20, 1850), m_player, m_projectiles)));

	AddEntity(std::make_shared<Mothership>(Mothership(glm::vec3(0, 0, 2000), m_player, m_projectiles, GetEntitiesPtr())));

	std::shared_ptr<Entity> gate = std::make_shared<Entity>("assets/gate.obj", glm::vec3(0, -500, 1000), "assets/gate_tex.png");
	gate->GetHitboxes().clear();
	gate->AddHitBox({ gate->GetPos() + glm::vec3(20, 0, 0), {10.0f, 50.0f, 10.0f} });
	gate->AddHitBox({ gate->GetPos() + glm::vec3(-20, 0, 0), {10.0f, 50.0f, 10.0f} });
	gate->AddHitBox({ gate->GetPos() + glm::vec3(0, 20, 0), {50.0f, 10.0f, 10.0f} });
	gate->AddHitBox({ gate->GetPos() + glm::vec3(0, -20, 0), {50.0f, 10.0f, 10.0f} });
	AddEntity(gate);

	CreateMeteorField();
}

void DeepSpace::CreateMeteorField()
{
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-800, 300, 1000), "assets/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-820, 300, 1100), glm::rotate(1.0f, glm::vec3(1, 0, 1)), "assets/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-910, 310, 1150), glm::rotate(2.3f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(1.2f, 1.2f, 1.2f)), "assets/meteor_tex.jpg", Dimensions{ 32.4f, 32.4f, 32.5f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-900, 400, 1200), glm::rotate(1.3f, glm::vec3(0, 0, 1)), "assets/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-750, 250, 800), glm::rotate(1.0f, glm::vec3(0, 1, 1)) * glm::scale(glm::vec3(0.8f, 0.8f, 0.8f)), "assets/meteor_tex.jpg", Dimensions{ 21.6f, 21.6f, 21.6f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-700, 280, 1000), glm::rotate(0.6f, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2.0f, 2.0f, 2.0f)), "assets/meteor_tex.jpg", Dimensions{ 54.0f, 54.0f, 54.0f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-800, 500, 1050), glm::scale(glm::vec3(1.5f, 1.5f, 1.5f)), "assets/meteor_tex.jpg", Dimensions{ 40.5f, 40.5f, 40.5f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-1000, 500, 750), glm::rotate(-1.0f, glm::vec3(0, 1, 1)) * glm::scale(glm::vec3(0.8f, 0.8f, 0.8f)), "assets/meteor_tex.jpg", Dimensions{ 21.6f, 21.6f, 21.6f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-950, 400, 800), glm::rotate(-1.6f, glm::vec3(0, 0, 1)) * glm::scale(glm::vec3(1.3f, 1.3f, 1.3f)), "assets/meteor_tex.jpg", Dimensions{ 35.1f, 35.1f, 35.1f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-900, 350, 900), glm::rotate(0.3f, glm::vec3(1, 1, 0)) * glm::scale(glm::vec3(0.8f, 0.8f, 0.8f)), "assets/meteor_tex.jpg", Dimensions{ 21.6f, 21.6f, 21.6f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-850, 325, 930), glm::rotate(0.8f, glm::vec3(0, 1, 0)), "assets/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-820, 400, 950), glm::rotate(2.3f, glm::vec3(1, 0, 1)), "assets/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-950, 280, 910), glm::rotate(1.5f, glm::vec3(1, 1, 0)), "assets/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-800, 200, 1000), glm::rotate(0.3f, glm::vec3(0, 1, 1)), "assets/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-780, 340, 980), glm::rotate(1.2f, glm::vec3(0, 0, 1)), "assets/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-980, 470, 800), glm::rotate(-0.5f, glm::vec3(1, 1, 1)) * glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)), "assets/meteor_tex.jpg", Dimensions{ 13.5f, 13.5f, 13.5f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-950, 450, 780), glm::rotate(-1.3f, glm::vec3(0, 1, 1)) * glm::scale(glm::vec3(0.6f, 0.6f, 0.6f)), "assets/meteor_tex.jpg", Dimensions{ 16.2f, 16.2f, 16.2f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-900, 470, 750), glm::rotate(0.8f, glm::vec3(1, 1, 1)) * glm::scale(glm::vec3(1.7f, 1.7f, 1.7f)), "assets/meteor_tex.jpg", Dimensions{ 45.9f, 45.9f, 45.9f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-860, 420, 830), glm::rotate(-0.5f, glm::vec3(0, 0, 1)) * glm::scale(glm::vec3(0.8f, 0.8f, 0.8f)), "assets/meteor_tex.jpg", Dimensions{ 21.6f, 21.6f, 21.6f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-830, 390, 870), glm::rotate(2.2f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(2.0f, 2.0f, 2.0f)), "assets/meteor_tex.jpg", Dimensions{ 54.0f, 54.0f, 54.0f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-788, 335, 930), glm::rotate(1.3f, glm::vec3(0, 1, 1)), "assets/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-700, 300, 1100), glm::rotate(0.5f, glm::vec3(1, 1, 1)) * glm::scale(glm::vec3(1.3f, 1.3f, 1.3f)), "assets/meteor_tex.jpg", Dimensions{ 35.1f, 35.1f, 35.1f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-750, 280, 1000), glm::rotate(-1.3f, glm::vec3(0, 1, 1)) * glm::scale(glm::vec3(0.3f, 0.3f, 0.3f)), "assets/meteor_tex.jpg", Dimensions{ 8.1f, 8.1f, 8.1f }));
	AddEntity(std::make_shared<Entity>("assets/meteor.obj", glm::vec3(-800, 350, 1150), glm::rotate(0.8f, glm::vec3(1, 1, 1)) * glm::scale(glm::vec3(0.4f, 0.4f, 0.4f)), "assets/meteor_tex.jpg", Dimensions{ 10.8f, 10.8f, 10.8f }));
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