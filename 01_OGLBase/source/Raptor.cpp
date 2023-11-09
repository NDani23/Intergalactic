#include "../headers/Raptor.h"
#include "../headers/scenes/Map.h"

std::unique_ptr<Mesh> Raptor::m_static_mesh;
Texture2D Raptor::m_static_tex;

int Raptor::FirstInit()
{
	Raptor::m_static_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Enemies/raptor.obj"));
	Raptor::m_static_mesh->initBuffers();
	Raptor::m_static_tex = Texture2D("assets/Enemies/raptor_tex.png");

	return 1;
}

Raptor::Raptor()
{
	static int onFirstCall = FirstInit();
	m_collider.setVertices(m_static_mesh->GetVertices());
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_forward_vec = glm::vec3(1.0f, 0.0f, 0.0f);
	m_shootDir = m_forward_vec;
	m_up_vec = glm::vec3(0.0f, 1.0f, 0.0f);

	SetTransforms(glm::inverse(glm::lookAt(m_position, m_position + m_forward_vec, glm::vec3(0.0f, 1.0f, 0.0f))));

	HitBox hitbox = { m_position, {10.0f, 3.0f, 11.0f} };
	m_hitboxes.emplace_back(hitbox);

	m_health = 200;
	m_speed = 200;
	m_damage = 20;
	m_mobility = 13;
	m_shootAngle = 0.3f;
	m_shootDistance = 300.f;

	m_coolDownTime = 0.20f;
	m_lastShootTime = std::chrono::system_clock::now();
}

Raptor::Raptor(glm::vec3 pos, Player* target, std::vector<std::unique_ptr<Projectile>>* projectiles, Map* map)
{
	static int onFirstCall = FirstInit();
	m_collider.setVertices(m_static_mesh->GetVertices());
	m_position = pos;
	m_target = target;
	m_projectiles = projectiles;
	m_Map = map;

	m_forward_vec = glm::normalize(m_target->GetPos() - m_position);
	m_shootDir = m_forward_vec;
	m_up_vec = glm::vec3(0.0f, 1.0f, 0.0f);

	SetTransforms(glm::inverse(glm::lookAt(m_position, m_position + m_forward_vec, glm::vec3(0.0f, 1.0f, 0.0f))));

	HitBox hitbox = { m_position, {10.0f, 3.0f, 11.0f} };
	m_hitboxes.emplace_back(hitbox);

	m_health = 200;
	m_speed = 180;
	m_damage = 20;
	m_mobility = 13;
	m_shootAngle = 0.3f;
	m_shootDistance = 300.f;

	m_mesh = nullptr;

	m_coolDownTime = 0.2f;
	m_lastShootTime = std::chrono::system_clock::now();
}


void Raptor::Shoot()
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastShootTime;

	if (elapsed_seconds.count() >= m_coolDownTime)
	{
		m_projectiles->emplace_back(std::make_unique<Laser>(m_position + m_forward_vec * 10.f, m_shootDir, m_damage));

		m_lastShootTime = std::chrono::system_clock::now();
	}
}

void Raptor::DrawMesh(ProgramObject& program, glm::mat4& viewProj)
{
	program.SetTexture("texImage", 0, Raptor::m_static_tex);
	program.SetUniform("MVP", viewProj * m_transforms);
	program.SetUniform("world", m_transforms);
	program.SetUniform("worldIT", glm::inverse(glm::transpose(m_transforms)));

	m_static_mesh->draw();
}

HitBox Raptor::UpdateDimensions()
{
	HitBox newHitBox = { m_position, {10.0f, 3.0f, 11.0f} };
	glm::vec3 cross_vec = glm::normalize(glm::cross(m_forward_vec, m_up_vec));

	newHitBox.dimensions.height = 3.0f + ((abs(m_up_vec.y) - 1) * (10.0f - 3.0f)) / -1;
	newHitBox.dimensions.height = std::max(2.5 + ((abs(m_forward_vec.y) - 0) * (11.0f - 3.0f)) / 1, (double)newHitBox.dimensions.height);

	newHitBox.dimensions.width = 10.0f + ((abs(cross_vec.x) - 1) * (3.0f - 10.0f)) / -1;
	newHitBox.dimensions.width = std::max(2.5 + ((abs(m_forward_vec.x)) * (11.0f - 3.0f)) / 1, (double)newHitBox.dimensions.width);

	newHitBox.dimensions.length = 3.0f + ((abs(m_forward_vec.z)) * (11.0f - 3.0f)) / 1;
	newHitBox.dimensions.length = std::max(3.0f + ((abs(cross_vec.z)) * (10.0f - 3.0)) / 1, (double)newHitBox.dimensions.length);

	return newHitBox;
}

std::unique_ptr<Mesh>& Raptor::GetMesh()
{
	return m_static_mesh;
}