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

	
	Map1::InitMap(m_map);
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
	
	m_player.Move(delta_time);

	//camera
	glm::vec3 new_eye = m_player.GetPos() - m_player.GetForwardVec() * 40.f + m_player.GetUpVec() * 5.f;
	glm::vec3 new_at = m_player.GetPos() + m_player.GetForwardVec() * 5.f;
	glm::vec3 new_up =	m_player.GetUpVec();

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

	m_program.Use();

	//player
	glm::mat4 playerWorld = m_player.GetWorldTransform();
	m_program.SetTexture("texImage", 0, m_player.GetTexture());
	m_program.SetUniform("MVP", viewProj * playerWorld);
	m_program.SetUniform("world", playerWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(playerWorld)));
	m_player.GetMesh()->draw();

	//world objects
	for (Entity& entity : m_map.GetEntities())
	{
		glm::mat4 entityWorld = entity.GetWorldTransform();
		m_program.SetTexture("texImage", 0, entity.GetTexture());
		m_program.SetUniform("MVP", viewProj * entityWorld);
		m_program.SetUniform("world", entityWorld);
		m_program.SetUniform("worldIT", glm::inverse(glm::transpose(entityWorld)));

		entity.GetMesh()->draw();
	}

	m_program.Unuse();

	//eye_pos for illumination
	glm::vec3 eye_pos = m_camera.GetEye();
	m_program.SetUniform("eye_pos", eye_pos);

	m_axesProgram.Use();
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

	for (Entity& entity : m_map.GetEntities())
	{
		glm::vec3 distance_vec = player_pos - entity.GetPos();

		Dimensions entity_dims = entity.GetHitboxes()[0].dimensions;

		if (abs(distance_vec.x) < std::max(player_dims.width / 2, entity_dims.width / 2)
		&& abs(distance_vec.y) < std::max(player_dims.height / 2, entity_dims.height / 2)
		&& abs(distance_vec.z) < std::max(player_dims.length / 2, entity_dims.length / 2))
		{
			//Collision response
			std::cout << "Collision detected!" << std::endl;
		}
		
	}
}

void CMyApp::DrawHitBoxes()
{
	//std::vector<glm::vec3> Points;

	//for (Entity& entity : m_map.GetEntities())
	//{

	//	glm::vec3 leftDB = entity.GetPos();
	//	leftDB.x -= entity.GetWidth() / 2;
	//	leftDB.y -= entity.GetHeight() / 2;
	//	leftDB.z -= entity.GetLength() / 2;

	//	glm::vec3 rightDB = entity.GetPos();
	//	rightDB.x += entity.GetWidth() / 2;
	//	rightDB.y -= entity.GetHeight() / 2;
	//	rightDB.z -= entity.GetLength() / 2;

	//	glm::vec3 rightUB = entity.GetPos();
	//	rightUB.x += entity.GetWidth() / 2;
	//	rightUB.y += entity.GetHeight() / 2;
	//	rightUB.z -= entity.GetLength() / 2;

	//	glm::vec3 leftUB = entity.GetPos();
	//	leftUB.x -= entity.GetWidth() / 2;
	//	leftUB.y += entity.GetHeight() / 2;
	//	leftUB.z -= entity.GetLength() / 2;


	//	glm::vec3 leftDF = entity.GetPos();
	//	leftDF.x -= entity.GetWidth() / 2;
	//	leftDF.y -= entity.GetHeight() / 2;
	//	leftDF.z += entity.GetLength() / 2;

	//	glm::vec3 rightDF = entity.GetPos();
	//	rightDF.x += entity.GetWidth() / 2;
	//	rightDF.y -= entity.GetHeight() / 2;
	//	rightDF.z += entity.GetLength() / 2;

	//	glm::vec3 rightUF = entity.GetPos();
	//	rightUF.x += entity.GetWidth() / 2;
	//	rightUF.y += entity.GetHeight() / 2;
	//	rightUF.z += entity.GetLength() / 2;

	//	glm::vec3 leftUF = entity.GetPos();
	//	leftUF.x -= entity.GetWidth() / 2;
	//	leftUF.y += entity.GetHeight() / 2;
	//	leftUF.z += entity.GetLength() / 2;
	//	//---------------------------------------------------------------------------------------------------------------------------------------------------------------

	//	Points.push_back(leftDB);
	//	Points.push_back(rightDB);

	//	Points.push_back(rightDB);
	//	Points.push_back(rightUB);

	//	Points.push_back(rightUB);
	//	Points.push_back(leftUB);

	//	Points.push_back(leftUB);
	//	Points.push_back(leftDB);

	//	Points.push_back(leftDB);
	//	Points.push_back(leftDF);

	//	Points.push_back(leftUB);
	//	Points.push_back(leftUF);

	//	Points.push_back(leftUF);
	//	Points.push_back(leftDF);

	//	Points.push_back(leftUF);
	//	Points.push_back(rightUF);

	//	Points.push_back(rightUF);
	//	Points.push_back(rightDF);

	//	Points.push_back(rightDF);
	//	Points.push_back(leftDF);

	//	Points.push_back(rightDF);
	//	Points.push_back(rightDB);

	//	Points.push_back(rightUF);
	//	Points.push_back(rightUB);

	//	m_axesProgram.SetUniform("mvp", m_camera.GetViewProj());
	//	m_axesProgram.SetUniform("points", Points);
	//	glDrawArrays(GL_LINES, 0, (GLsizei)Points.size());

	//	Points.clear();
	//}


	//glm::vec3 leftDB = m_player.GetPos();
	//leftDB.x -= m_player.GetWidth() / 2;
	//leftDB.y -= m_player.GetHeight() / 2;
	//leftDB.z -= m_player.GetLength() / 2;

	//glm::vec3 rightDB = m_player.GetPos();
	//rightDB.x += m_player.GetWidth() / 2;
	//rightDB.y -= m_player.GetHeight() / 2;
	//rightDB.z -= m_player.GetLength() / 2;

	//glm::vec3 rightUB = m_player.GetPos();
	//rightUB.x += m_player.GetWidth() / 2;
	//rightUB.y += m_player.GetHeight() / 2;
	//rightUB.z -= m_player.GetLength() / 2;

	//glm::vec3 leftUB = m_player.GetPos();
	//leftUB.x -= m_player.GetWidth() / 2;
	//leftUB.y += m_player.GetHeight() / 2;
	//leftUB.z -= m_player.GetLength() / 2;


	//glm::vec3 leftDF = m_player.GetPos();
	//leftDF.x -= m_player.GetWidth() / 2;
	//leftDF.y -= m_player.GetHeight() / 2;
	//leftDF.z += m_player.GetLength() / 2;

	//glm::vec3 rightDF = m_player.GetPos();
	//rightDF.x += m_player.GetWidth() / 2;
	//rightDF.y -= m_player.GetHeight() / 2;
	//rightDF.z += m_player.GetLength() / 2;

	//glm::vec3 rightUF = m_player.GetPos();
	//rightUF.x += m_player.GetWidth() / 2;
	//rightUF.y += m_player.GetHeight() / 2;
	//rightUF.z += m_player.GetLength() / 2;

	//glm::vec3 leftUF = m_player.GetPos();
	//leftUF.x -= m_player.GetWidth() / 2;
	//leftUF.y += m_player.GetHeight() / 2;
	//leftUF.z += m_player.GetLength() / 2;
	////---------------------------------------------------------------------------------------------------------------------------------------------------------------

	//Points.push_back(leftDB);
	//Points.push_back(rightDB);

	//Points.push_back(rightDB);
	//Points.push_back(rightUB);

	//Points.push_back(rightUB);
	//Points.push_back(leftUB);

	//Points.push_back(leftUB);
	//Points.push_back(leftDB);

	//Points.push_back(leftDB);
	//Points.push_back(leftDF);

	//Points.push_back(leftUB);
	//Points.push_back(leftUF);

	//Points.push_back(leftUF);
	//Points.push_back(leftDF);

	//Points.push_back(leftUF);
	//Points.push_back(rightUF);

	//Points.push_back(rightUF);
	//Points.push_back(rightDF);

	//Points.push_back(rightDF);
	//Points.push_back(leftDF);

	//Points.push_back(rightDF);
	//Points.push_back(rightDB);

	//Points.push_back(rightUF);
	//Points.push_back(rightUB);

	//m_axesProgram.SetUniform("mvp", m_camera.GetViewProj());
	//m_axesProgram.SetUniform("points", Points);
	//glDrawArrays(GL_LINES, 0, (GLsizei)Points.size());

	//Points.clear();
}
