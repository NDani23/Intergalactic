#pragma once

#include <glm/glm.hpp>

#include "../includes/ObjParser_OGL3.h"
#include "../includes/ProgramObject.h"
#include "../includes/TextureObject.h"
#include "HitBox.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "GameState.h"
#include "Player.h"
#include "Floor.h"
#include "EnemySpawnPoint.h"

class Map
{
protected:
	std::string m_name;
	Player* m_player;
	std::vector<std::unique_ptr<Projectile>>* m_projectiles;
	std::vector<std::unique_ptr<EnemySpawnPoint>> m_enemySpawnPoints;
	TextureCubeMap	m_skyboxTexture;
	std::vector<std::shared_ptr<Entity>> m_Entities;

	ProgramObject m_program;
	ProgramObject m_skyBoxProgram;

public:
	Map();
	std::vector<std::shared_ptr<Entity>>& GetEntities();
	std::vector<std::unique_ptr<EnemySpawnPoint>>& GetSpawnPoints();
	std::vector<std::shared_ptr<Entity>>* GetEntitiesPtr();
	TextureCubeMap& GetSkyBox();
	ProgramObject& getProgram();
	ProgramObject& getSkyBoxProgram();
	std::string& getName();
	virtual Floor* GetFloor();


	void ClearMap();
	void AddEntity(std::shared_ptr<Entity> entity);
	void SetSkyBox(std::string, std::string, std::string, std::string ,std::string ,std::string);

	virtual void LoadMap() {};
	virtual void DrawEntities(glm::mat4&, GameState&) {};
	virtual void DrawSkyBox(glm::mat4&) {};
};