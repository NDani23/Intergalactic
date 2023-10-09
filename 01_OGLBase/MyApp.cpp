#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "includes/GLUtils.hpp"

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(0, 0, 0), glm::vec3(0, 0, 3), glm::vec3(0, 1, 0));
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

bool CMyApp::Init(bool* quit)
{
	m_quit = quit;

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

void CMyApp::Reset()
{
	m_PlayTime = 0.f;
	m_projectiles.clear();
	m_player.Reset();
	Map1::ResetMap(m_map, m_projectiles, &m_player);
}

void CMyApp::Update()
{
	
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	if (!m_GameState.play)
	{
		glm::vec4 cam_at = glm::vec4(m_camera.GetAt(),1);
		cam_at = glm::rotate(delta_time*0.2f, glm::vec3(0.f, 1.f, 0.f)) * cam_at;
		m_camera.SetView(glm::vec3(0, 0, 0), glm::vec3(cam_at.x, cam_at.y, cam_at.z), glm::vec3(0, 1, 0));
		m_camera.Update(delta_time);
		last_time = SDL_GetTicks();
		return;
	}

	if (m_GameState.pause)
	{
		m_camera.Update(delta_time);
		last_time = SDL_GetTicks();
		return;
	}

	m_cursor_diff_vec = glm::normalize((m_player.GetUpVec() * -1.0f * m_mouseY) + (m_player.GetCrossVec() * -1.0f * m_mouseX) + m_player.GetForwardVec());

	if (!m_GameState.gameover)
	{
		m_PlayTime += delta_time;

		m_player.Move(delta_time, m_cursor_diff_vec);
		m_player.UpdateProjectiles(delta_time);
	}
	UpdateProjectiles(delta_time);
	DetectHit(m_player.GetProjectiles());

	UpdateEntities(delta_time);

	if (m_shooting) m_player.Shoot();

	//camera
	glm::vec3 new_eye = m_player.GetPos() - m_player.GetForwardVec() * 40.f + m_player.GetUpVec() * 5.f;
	glm::vec3 new_at = m_player.GetPos() + m_player.GetForwardVec() * 1000.f;
	glm::vec3 new_up = m_player.GetUpVec();

	if (m_backward_camera)
	{
		new_eye = m_player.GetPos() + m_player.GetForwardVec() * 40.f + m_player.GetUpVec() * 5.f;
		new_at = m_player.GetPos() - m_player.GetForwardVec() * 30.f;
	}

	//camera teszt up
	/*glm::vec3 new_eye = m_player.GetPos() + m_player.GetUpVec() * 50.f;
	glm::vec3 new_at = m_player.GetPos();
	glm::vec3 new_up = m_player.GetForwardVec();*/

	////camera teszt behind
	/*glm::vec3 new_eye = m_player.GetPos() - m_player.GetForwardVec() * 40.f;
	glm::vec3 new_at = m_player.GetPos();
	glm::vec3 new_up = m_player.GetUpVec();*/

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


	if (m_GameState.play)
	{
		m_program.Use();


		if (!m_GameState.gameover)
		{
			//player
			m_player.DrawMesh(m_program, viewProj);

			//player guns
			m_player.GetActiveWeapon1().DrawMesh(m_program, viewProj);

			m_player.GetActiveWeapon2().DrawMesh(m_program, viewProj);
		}

		//world objects
		for (std::shared_ptr<Entity>& entity : m_map.GetEntities())
		{
			entity->DrawMesh(m_program, viewProj);
		}

		glm::vec3 eye_pos = m_camera.GetEye();
		m_program.SetUniform("eye_pos", eye_pos);

		m_program.Unuse();

		m_axesProgram.Use();
		DrawProjectiles(m_player.GetProjectiles());
		//DrawHitBoxes();
		m_axesProgram.Unuse();
	}
	

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

	//ImGui
	RenderUI();
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
	case SDLK_LCTRL:
		m_player.Decelerate(true);
		break;
	case SDLK_v:
		m_backward_camera = true;
		break;
	case SDLK_ESCAPE:
		if(m_GameState.play)
			m_GameState.pause = true;
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
	case SDLK_LCTRL:
		m_player.Decelerate(false);
		break;
	case SDLK_v:
		m_backward_camera = false;
		break;
	}
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	//m_camera.MouseMove(mouse);

	m_mouseX = mouse.x / (float)(m_screenWidth / 2) - 1;
	m_mouseY = mouse.y / (float)(m_screenHeight / 2) - 1;
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
	if (mouse.button == SDL_BUTTON_LEFT)
	{
		m_shooting = true;
	}
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
	if (mouse.button == SDL_BUTTON_LEFT)
	{
		m_shooting = false;
	}
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

void CMyApp::Resize(int _w, int _h)
{
	m_screenWidth = _w;
	m_screenHeight = _h;

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
				//std::cout << "Collision detected!" << std::endl;
				m_player.setHealth(0);
				m_GameState.gameover = true;
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
				if (entity->Hit(proj.GetDamage()))
				{
					auto position = std::find(m_map.GetEntities().begin(), m_map.GetEntities().end(), entity);
					if (position != m_map.GetEntities().end())
						m_map.GetEntities().erase(position);

					m_player.setPoints(m_player.GetPoints() + 10);
				}

				m_player.RemoveProjectile(proj);
				//std::cout << "Hit detected!" << std::endl;
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

			if (m_player.Hit(proj.GetDamage())) 
			{
				m_GameState.gameover = true;
			}
			//std::cout << "Damage!" << std::endl;
		}
	}
}

void CMyApp::DrawProjectiles(std::vector<Projectile>& projectiles)
{
	std::vector<glm::vec3> Points;

	for (Projectile& projectile : projectiles)
	{
		Points.push_back(projectile.GetPos() + (projectile.GetDirection() * 2.0f));
		Points.push_back(projectile.GetPos() - (projectile.GetDirection() * 2.0f));
	}

	for (Projectile& projectile : m_projectiles)
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
	for (int i = m_map.GetEntities().size() - 1; i >= 0; i--)
	{
		if (m_map.GetEntities()[i]->Update(delta))
		{
			auto position = std::find(m_map.GetEntities().begin(), m_map.GetEntities().end(), m_map.GetEntities()[i]);
			if (position != m_map.GetEntities().end())
				m_map.GetEntities().erase(position);
		}
	}

}

void CMyApp::UpdateProjectiles(const float& delta)
{
	for (Projectile& proj : m_projectiles)
	{
		glm::vec3 newPos = proj.GetPos() + proj.GetDirection() * (delta * proj.GetSpeed());
		proj.SetPosition(newPos);

		glm::vec3 dist_vec = proj.GetPos() - m_player.GetPos();
		if (glm::length(dist_vec) > 500.0f)
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


void CMyApp::RenderUI()
{
	//ImGui::ShowDemoWindow();

	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;


	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", nullptr, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode ^ ImGuiDockNodeFlags_NoTabBar ^ ImGuiDockNodeFlags_NoResize ^ ImGuiDockNodeFlags_NoUndocking;

	//ImGui::ShowDemoWindow();

	if (m_GameState.menu)
		RenderMenu();

	if (m_GameState.play)
		RenderPlayWindow();

	if (m_GameState.gameover)
		RenderGameOverWindow();

	if (m_GameState.pause)
		RenderPauseWindow();

}

void CMyApp::RenderPlayWindow()
{

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.f, 10.f));
	if (ImGui::BeginMainMenuBar()) 
	{
		std::stringstream ss;
		ss << std::setw(2) << std::setfill('0') << (int)m_PlayTime/60 << ":" << std::setw(2) << std::setfill('0') << (int)m_PlayTime % 60;
		ImGui::Text(ss.str().c_str());
		
		ImGui::Indent(m_screenWidth - 70.f);
		ImGui::Text("Score: %i", m_player.GetPoints());

		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleVar();

	ImGui::Begin("Viewport");
	ImGui::Indent(m_screenWidth / 3.f);
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor::HSV(0, 255, 235, 255));
	ImGui::ProgressBar(m_player.GetHealth() * 0.01f, ImVec2(m_screenWidth / 3.f, 15.0f));
	ImGui::End();

}

void CMyApp::RenderMenu()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

	ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + m_screenWidth / 3, main_viewport->WorkPos.y + m_screenHeight / 4));
	ImGui::SetNextWindowSize(ImVec2(m_screenWidth / 3, m_screenHeight / 2));

	ImGui::Begin("Menu", nullptr, window_flags);

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 windowPos = ImGui::GetWindowPos();


	ImGui::Indent(windowSize.x/8.f);

	if (ImGui::Button("PLAY", ImVec2(windowSize.x * (3.f / 4), windowSize.y / 10)))
	{
		m_GameState.play = true;
		m_GameState.menu = false;

		Reset();
	}

	if (ImGui::Button("EXIT", ImVec2(windowSize.x * (3.f / 4), windowSize.y / 10)))
	{
		*m_quit = true;
	}
	ImGui::End();
}

void CMyApp::RenderGameOverWindow()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

	ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + m_screenWidth / 3, main_viewport->WorkPos.y + m_screenHeight / 5 * 2));
	ImGui::SetNextWindowSize(ImVec2(m_screenWidth / 3, m_screenHeight / 5));

	ImGui::Begin("Game Over", nullptr, window_flags);

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 windowPos = ImGui::GetWindowPos();


	ImGui::Indent(windowSize.x / 8.f);

	ImGui::Text("Final score: %i", m_player.GetPoints());

	if (ImGui::Button("BACK TO MENU", ImVec2(windowSize.x * (3.f / 4), windowSize.y / 10)))
	{
		m_GameState.gameover = false;
		m_GameState.play = false;
		m_GameState.menu = true;
	}

	ImGui::End();
}

void CMyApp::RenderPauseWindow()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

	ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + m_screenWidth / 3, main_viewport->WorkPos.y + m_screenHeight / 5 * 2));
	ImGui::SetNextWindowSize(ImVec2(m_screenWidth / 3, m_screenHeight / 5));

	ImGui::Begin("Paused", nullptr, window_flags);

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 windowPos = ImGui::GetWindowPos();


	ImGui::Indent(windowSize.x / 8.f);

	if (ImGui::Button("Continue", ImVec2(windowSize.x * (3.f / 4), windowSize.y / 10)))
	{
		m_GameState.pause = false;
	}

	if (ImGui::Button("BACK TO MENU", ImVec2(windowSize.x * (3.f / 4), windowSize.y / 10)))
	{
		m_GameState.play = false;
		m_GameState.pause = false;
		m_GameState.menu = true;
	}

	ImGui::End();
}
