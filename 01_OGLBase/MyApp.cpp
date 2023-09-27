#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>
#include "includes/GLUtils.hpp"

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(0, 7, -30), glm::vec3(0, 3, 0), glm::vec3(0, 1, 0));
}

CMyApp::~CMyApp(void)
{
}

void CMyApp::InitSkyBox()
{
	m_SkyboxPos.BufferData(
		std::vector<glm::vec3>{

		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, -1),

		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, 1),
	}
	);


	m_SkyboxIndices.BufferData(
		std::vector<int>{
			// hátsó lap
			0, 1, 2,
			2, 3, 0,
			// elülső lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// alsó
			1, 0, 4,
			1, 4, 5,
			// felső
			3, 2, 6,
			3, 6, 7,
	}
	);

	m_SkyboxVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_SkyboxPos },
		}, m_SkyboxIndices
	);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	m_skyboxTexture = m_map.GetSkyBox();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CMyApp::InitShaders()
{
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

	m_programSkybox.Init(
		{
			{ GL_VERTEX_SHADER, "skybox.vert" },
			{ GL_FRAGMENT_SHADER, "skybox.frag" }
		},
		{
			{ 0, "vs_in_pos" },
		}
	);

	m_axesProgram.Init({
		{GL_VERTEX_SHADER, "axes.vert"},
		{GL_FRAGMENT_SHADER, "axes.frag"}
		});
}

bool CMyApp::Init()
{
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glLineWidth(4.0f);

	Map1::InitMap(m_map, m_projectiles, &m_player);
	InitShaders();
	InitSkyBox();

	m_camera.SetProj(glm::radians(60.0f), 640.0f / 480.0f, 0.01f, 10000.0f);

	return true;
}

void CMyApp::Clean()
{
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;
	
	//std::cout << delta_time << std::endl;
	
	m_player.Move(delta_time);
	m_player.UpdateProjectiles(delta_time);
	UpdateProjectiles(delta_time);
	DetectHit(m_player.GetProjectiles());

	UpdateEntities(delta_time);

	if (m_shooting) m_player.Shoot();

	//camera
	glm::vec3 new_eye = m_player.GetPos() - m_player.GetForwardVec() * 40.f + m_player.GetUpVec() * 5.f;
	glm::vec3 new_at = m_player.GetPos() + m_player.GetForwardVec() * 5.f;
	glm::vec3 new_up = m_player.GetUpVec();

	if (m_backward_camera)
	{
		new_eye = m_player.GetPos() + m_player.GetForwardVec() * 40.f + m_player.GetUpVec() * 5.f;
		new_at = m_player.GetPos() - m_player.GetForwardVec() * 5.f;
	}

	//camera teszt up
	/*glm::vec3 new_eye = m_player.GetPos() + m_player.GetUpVec() * 50.f;
	glm::vec3 new_at = m_player.GetPos();
	glm::vec3 new_up = m_player.GetForwardVec();*/

	////camera teszt behind
	//glm::vec3 new_eye = m_player.GetPos() - m_player.GetForwardVec() * 40.f;
	//glm::vec3 new_at = m_player.GetPos();
	//glm::vec3 new_up = m_player.GetUpVec();

	//camera teszt side
	/*glm::vec3 new_eye = m_player.GetPos() - m_player.GetCrossVec() * 40.f;
	glm::vec3 new_at = m_player.GetPos();
	glm::vec3 new_up = m_player.GetUpVec();*/

	m_camera.SetView(new_eye, new_at, new_up);

	m_camera.Update(delta_time);

	DetectCollisions();

	last_time = SDL_GetTicks();
}

void CMyApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewProj = m_camera.GetViewProj();

	float t = SDL_GetTicks() / 20;

	m_program.Use();

	//player
	m_player.DrawMesh(m_program, viewProj);

	//player guns
	m_player.GetActiveWeapon1().DrawMesh(m_program, viewProj);

	m_player.GetActiveWeapon2().DrawMesh(m_program, viewProj);

	//world objects
	for (std::shared_ptr<Entity>& entity : m_map.GetEntities())
	{
		entity->DrawMesh(m_program, viewProj);
	}

	m_program.Unuse();

	//eye_pos for illumination
	glm::vec3 eye_pos = m_camera.GetEye();
	m_program.SetUniform("eye_pos", eye_pos);

	m_axesProgram.Use();
	DrawProjectiles(m_player.GetProjectiles());
	//DrawHitBoxes();
	m_axesProgram.Unuse();

	// skybox
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	glDepthFunc(GL_LEQUAL);

	m_SkyboxVao.Bind();
	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", viewProj * glm::translate( m_camera.GetEye()) );
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glUniform1i(m_programSkybox.GetLocation("skyboxTexture"), 0);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	m_programSkybox.Unuse();

	glDepthFunc(prevDepthFnc);

	//ImGui Testwindow
	ImGui::ShowTestWindow();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym)
	{
	case SDLK_w:
		m_player.setPitchDir(vertical::down);
		break;
	case SDLK_s:
		m_player.setPitchDir(vertical::up);
		break;
	case SDLK_a:
		m_player.setRollDir(horizontal::left);
		break;
	case SDLK_d:
		m_player.setRollDir(horizontal::right);
		break;
	case SDLK_SPACE:
		m_shooting = true;
		break;
	case SDLK_v:
		m_backward_camera = true;
		break;
	}
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym)
	{
	case SDLK_w:
		m_player.setPitchDir(vertical::none);
		break;
	case SDLK_s:
		m_player.setPitchDir(vertical::none);
		break;
	case SDLK_a:
		m_player.setRollDir(horizontal::none);
		break;
	case SDLK_d:
		m_player.setRollDir(horizontal::none);
		break;
	case SDLK_SPACE:
		m_shooting = false;
		break;
	case SDLK_v:
		m_backward_camera = false;
		break;
	}
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}

void CMyApp::DetectCollisions()
{
	glm::vec3 player_pos = m_player.GetPos();
	Dimensions player_dims = m_player.GetHitboxes()[0].dimensions;

	for (std::shared_ptr<Entity>& entity : m_map.GetEntities())
	{

		for (HitBox& hitbox : entity->GetHitboxes())
		{
			glm::vec3 distance_vec = player_pos - hitbox.Position;

			Dimensions hitbox_dims = hitbox.dimensions;

			if (abs(distance_vec.x) < std::max(player_dims.width / 2, hitbox_dims.width / 2)
				&& abs(distance_vec.y) < std::max(player_dims.height / 2, hitbox_dims.height / 2)
				&& abs(distance_vec.z) < std::max(player_dims.length / 2, hitbox_dims.length / 2))
			{
				//Collision response
				std::cout << "Collision detected!" << std::endl;
			}
		}
		
	}
}

void CMyApp::DetectHit(std::vector<Projectile>& projectiles)
{
	for (Projectile& proj : projectiles)
	{
		glm::vec3 endPoint = proj.GetPos() + (proj.GetDirection() * 2.0f);
		for (std::shared_ptr<Entity>& entity : m_map.GetEntities())
		{
			glm::vec3 distance_vec = entity->GetPos() - endPoint;
			Dimensions hitbox_dims = entity->GetHitboxes()[0].dimensions;

			if (abs(distance_vec.x) < hitbox_dims.width / 2
				&& abs(distance_vec.y) < hitbox_dims.height / 2
				&& abs(distance_vec.z) < hitbox_dims.length / 2)
			{
				// hit response
				m_player.RemoveProjectile(proj);
				std::cout << "Hit detected!" << std::endl;
				break;
			}
		}
	}

	for (Projectile& proj : m_projectiles)
	{
		glm::vec3 endPoint = proj.GetPos() + (proj.GetDirection() * 2.0f);

		glm::vec3 distance_vec = m_player.GetPos() - endPoint;
		Dimensions hitbox_dims = m_player.GetHitboxes()[0].dimensions;

		if (abs(distance_vec.x) < hitbox_dims.width / 2
			&& abs(distance_vec.y) < hitbox_dims.height / 2
			&& abs(distance_vec.z) < hitbox_dims.length / 2)
		{
			// hit response
			auto position = std::find(m_projectiles.begin(), m_projectiles.end(), proj);
			if (position != m_projectiles.end())
				m_projectiles.erase(position);

			std::cout << "Damage!" << std::endl;
		}
	}
}

void CMyApp::DrawProjectiles(const std::vector<Projectile>& projectiles)
{
	std::vector<glm::vec3> Points;

	for (Projectile projectile : projectiles)
	{
		Points.push_back(projectile.GetPos() + (projectile.GetDirection() * 2.0f));
		Points.push_back(projectile.GetPos() - (projectile.GetDirection() * 2.0f));
	}

	for (Projectile projectile : m_projectiles)
	{
		Points.push_back(projectile.GetPos() + (projectile.GetDirection() * 2.0f));
		Points.push_back(projectile.GetPos() - (projectile.GetDirection() * 2.0f));
	}

	m_axesProgram.SetUniform("mvp", m_camera.GetViewProj());
	m_axesProgram.SetUniform("points", Points);
	glDrawArrays(GL_LINES, 0, (GLsizei)Points.size());
}

void CMyApp::UpdateEntities(const float& delta)
{
	for (std::shared_ptr<Entity>& entity : m_map.GetEntities())
	{
		entity->Update(delta);
	}
}

void CMyApp::UpdateProjectiles(const float& delta)
{
	for (Projectile& proj : m_projectiles)
	{
		glm::vec3 newPos = proj.GetPos() + proj.GetDirection() * (delta * proj.GetSpeed());
		proj.SetPosition(newPos);

		glm::vec3 dist_vec = proj.GetPos() - m_player.GetPos();
		if (glm::length(dist_vec) > 2000.0f)
		{
			auto position = std::find(m_projectiles.begin(), m_projectiles.end(), proj);
			if (position != m_projectiles.end())
				m_projectiles.erase(position);
		}
	}
}

void CMyApp::DrawHitBoxes()
{
	std::vector<glm::vec3> Points;

	for (std::shared_ptr<Entity>& entity : m_map.GetEntities())
	{
		for (HitBox& hitbox : entity->GetHitboxes())
		{
			glm::vec3 leftDB = hitbox.Position;
			leftDB.x -= hitbox.dimensions.width / 2;
			leftDB.y -= hitbox.dimensions.height / 2;
			leftDB.z -= hitbox.dimensions.length / 2;

			glm::vec3 rightDB = hitbox.Position;
			rightDB.x += hitbox.dimensions.width / 2;
			rightDB.y -= hitbox.dimensions.height / 2;
			rightDB.z -= hitbox.dimensions.length / 2;

			glm::vec3 rightUB = hitbox.Position;
			rightUB.x += hitbox.dimensions.width / 2;
			rightUB.y += hitbox.dimensions.height / 2;
			rightUB.z -= hitbox.dimensions.length / 2;

			glm::vec3 leftUB = hitbox.Position;
			leftUB.x -= hitbox.dimensions.width / 2;
			leftUB.y += hitbox.dimensions.height / 2;
			leftUB.z -= hitbox.dimensions.length / 2;


			glm::vec3 leftDF = hitbox.Position;
			leftDF.x -= hitbox.dimensions.width / 2;
			leftDF.y -= hitbox.dimensions.height / 2;
			leftDF.z += hitbox.dimensions.length / 2;

			glm::vec3 rightDF = hitbox.Position;
			rightDF.x += hitbox.dimensions.width / 2;
			rightDF.y -= hitbox.dimensions.height / 2;
			rightDF.z += hitbox.dimensions.length / 2;

			glm::vec3 rightUF = hitbox.Position;
			rightUF.x += hitbox.dimensions.width / 2;
			rightUF.y += hitbox.dimensions.height / 2;
			rightUF.z += hitbox.dimensions.length / 2;

			glm::vec3 leftUF = hitbox.Position;
			leftUF.x -= hitbox.dimensions.width / 2;
			leftUF.y += hitbox.dimensions.height / 2;
			leftUF.z += hitbox.dimensions.length / 2;
			//---------------------------------------------------------------------------------------------------------------------------------------------------------------

			Points.push_back(leftDB);
			Points.push_back(rightDB);

			Points.push_back(rightDB);
			Points.push_back(rightUB);

			Points.push_back(rightUB);
			Points.push_back(leftUB);

			Points.push_back(leftUB);
			Points.push_back(leftDB);

			Points.push_back(leftDB);
			Points.push_back(leftDF);

			Points.push_back(leftUB);
			Points.push_back(leftUF);

			Points.push_back(leftUF);
			Points.push_back(leftDF);

			Points.push_back(leftUF);
			Points.push_back(rightUF);

			Points.push_back(rightUF);
			Points.push_back(rightDF);

			Points.push_back(rightDF);
			Points.push_back(leftDF);

			Points.push_back(rightDF);
			Points.push_back(rightDB);

			Points.push_back(rightUF);
			Points.push_back(rightUB);

			m_axesProgram.SetUniform("mvp", m_camera.GetViewProj());
			m_axesProgram.SetUniform("points", Points);
			glDrawArrays(GL_LINES, 0, (GLsizei)Points.size());

			Points.clear();
		}
	}


	glm::vec3 leftDB = m_player.GetHitboxes()[0].Position;
	leftDB.x -= m_player.GetHitboxes()[0].dimensions.width / 2;
	leftDB.y -= m_player.GetHitboxes()[0].dimensions.height / 2;
	leftDB.z -= m_player.GetHitboxes()[0].dimensions.length / 2;

	glm::vec3 rightDB = m_player.GetHitboxes()[0].Position;
	rightDB.x += m_player.GetHitboxes()[0].dimensions.width / 2;
	rightDB.y -= m_player.GetHitboxes()[0].dimensions.height / 2;
	rightDB.z -= m_player.GetHitboxes()[0].dimensions.length / 2;

	glm::vec3 rightUB = m_player.GetHitboxes()[0].Position;
	rightUB.x += m_player.GetHitboxes()[0].dimensions.width / 2;
	rightUB.y += m_player.GetHitboxes()[0].dimensions.height / 2;
	rightUB.z -= m_player.GetHitboxes()[0].dimensions.length / 2;

	glm::vec3 leftUB = m_player.GetHitboxes()[0].Position;
	leftUB.x -= m_player.GetHitboxes()[0].dimensions.width / 2;
	leftUB.y += m_player.GetHitboxes()[0].dimensions.height / 2;
	leftUB.z -= m_player.GetHitboxes()[0].dimensions.length / 2;


	glm::vec3 leftDF = m_player.GetHitboxes()[0].Position;
	leftDF.x -= m_player.GetHitboxes()[0].dimensions.width / 2;
	leftDF.y -= m_player.GetHitboxes()[0].dimensions.height / 2;
	leftDF.z += m_player.GetHitboxes()[0].dimensions.length / 2;

	glm::vec3 rightDF = m_player.GetHitboxes()[0].Position;
	rightDF.x += m_player.GetHitboxes()[0].dimensions.width / 2;
	rightDF.y -= m_player.GetHitboxes()[0].dimensions.height / 2;
	rightDF.z += m_player.GetHitboxes()[0].dimensions.length / 2;

	glm::vec3 rightUF = m_player.GetHitboxes()[0].Position;
	rightUF.x += m_player.GetHitboxes()[0].dimensions.width / 2;
	rightUF.y += m_player.GetHitboxes()[0].dimensions.height / 2;
	rightUF.z += m_player.GetHitboxes()[0].dimensions.length / 2;

	glm::vec3 leftUF = m_player.GetHitboxes()[0].Position;
	leftUF.x -= m_player.GetHitboxes()[0].dimensions.width / 2;
	leftUF.y += m_player.GetHitboxes()[0].dimensions.height / 2;
	leftUF.z += m_player.GetHitboxes()[0].dimensions.length / 2;
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------

	Points.push_back(leftDB);
	Points.push_back(rightDB);

	Points.push_back(rightDB);
	Points.push_back(rightUB);

	Points.push_back(rightUB);
	Points.push_back(leftUB);

	Points.push_back(leftUB);
	Points.push_back(leftDB);

	Points.push_back(leftDB);
	Points.push_back(leftDF);

	Points.push_back(leftUB);
	Points.push_back(leftUF);

	Points.push_back(leftUF);
	Points.push_back(leftDF);

	Points.push_back(leftUF);
	Points.push_back(rightUF);

	Points.push_back(rightUF);
	Points.push_back(rightDF);

	Points.push_back(rightDF);
	Points.push_back(leftDF);

	Points.push_back(rightDF);
	Points.push_back(rightDB);

	Points.push_back(rightUF);
	Points.push_back(rightUB);

	m_axesProgram.SetUniform("mvp", m_camera.GetViewProj());
	m_axesProgram.SetUniform("points", Points);
	glDrawArrays(GL_LINES, 0, (GLsizei)Points.size());

	Points.clear();
}
