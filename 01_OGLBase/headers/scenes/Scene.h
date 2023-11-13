#pragma once

#include <glm/glm.hpp>

#include "../../includes/ObjParser_OGL3.h"
#include "../../includes/ProgramObject.h"
#include "../../includes/TextureObject.h"
#include "../collision_detection/HitBox.h"

#include "../../headers/scenes/SkyBox.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "../GameState.h"
#include "../Player.h"
#include "Floor.h"
#include "EnemySpawnPoint.h"

class Scene
{
protected:
	std::string m_name;
	Player* m_player;
	std::vector<std::unique_ptr<Projectile>>* m_projectiles;
	std::vector<std::unique_ptr<EnemySpawnPoint>> m_enemySpawnPoints;
	std::vector<std::shared_ptr<Entity>> m_Entities;

	SkyBox m_skyBox;
	ProgramObject m_program;

public:
	Scene();
	std::vector<std::shared_ptr<Entity>>& GetEntities();
	std::vector<std::unique_ptr<EnemySpawnPoint>>& GetSpawnPoints();
	std::vector<std::shared_ptr<Entity>>* GetEntitiesPtr();
	ProgramObject& getProgram();
	std::string& getName();
	virtual Floor* GetFloor();


	void ClearScene();
	void AddEntity(std::shared_ptr<Entity> entity);

	virtual void LoadScene() {};
	virtual void DrawScene(glm::mat4&, GameState&, glm::vec3) {};
	void DrawSkyBox(glm::mat4&, glm::vec3);
};