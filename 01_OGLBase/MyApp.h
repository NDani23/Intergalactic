#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>
#include <string>
#include <sstream>
#include <iomanip>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "includes/gCamera.h"

#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

// mesh
#include "includes/ObjParser_OGL3.h"

#include "Directions.h"
#include "DeepSpace.h"
#include "InitMap1.h"
#include "Projectile.h"
#include "Player.h"
#include "Turret.h"
#include "GameState.h"
#include "AppUI.h"
#include "Persistence.h"

class CMyApp
{
public:
	CMyApp();
	~CMyApp();

	bool Init(bool*);
	void Clean();

	void Reset();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);
	void DetectCollisions();
	void DetectHit(std::vector<std::unique_ptr<Projectile>>&);
	void DrawHitBoxes();
	void DrawHitBox(HitBox&);
	void DrawProjectiles(std::vector<std::unique_ptr<Projectile>>&);
	void UpdateEntities(const float&);
	void UpdateProjectiles(const float&);
	void GameOver();
	void Exit();

	friend class AppUI;
	friend class Persistence;

protected:
	// shaderekhez szükséges változók
	//ProgramObject		m_program;			// mesh shader
	ProgramObject		m_programSkybox;	// skybox shader
	ProgramObject		m_axesProgram;

	VertexArrayObject	m_SkyboxVao;
	IndexBuffer			m_SkyboxIndices;	
	ArrayBuffer			m_SkyboxPos;		

	gCamera				m_camera;

	TextureCubeMap		m_skyboxTexture;

	GameState m_GameState = {true, false, false, false};

	WeaponDataStorage m_weaponStorage;
	UpgradeDataStorage m_upgradeStorage;

	Persistence m_Persistence;
	AppUI UI;
	Player m_player;
	Map& m_map = DeepSpace(&m_projectiles, &m_player);

	float m_PlayTime = 0;

	bool m_shooting = false;
	bool m_useUpgrade = false;

	bool m_backward_camera = false;
	std::vector<std::unique_ptr<Projectile>> m_projectiles;
	glm::vec3 m_cursor_diff_vec = { 0.0f, 0.0f, 0.0f };

	float m_mouseX = 320.0f;
	float m_mouseY = 240.0f;

	int m_screenWidth = 640;
	int m_screenHeight = 480;

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	bool* m_quit;

	// a jobb olvashatóság kedvéért
	void InitShaders();
	void InitSkyBox();
};

