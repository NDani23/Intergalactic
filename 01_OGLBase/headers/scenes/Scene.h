#pragma once

#include <glm/glm.hpp>

#include "../../includes/ObjParser_OGL3.h"
#include "../../includes/ProgramObject.h"
#include "../../includes/TextureObject.h"
#include "../collision_detection/HitBox.h"
#include "../../headers/collision_detection/GJK.h"

#include "../../headers/scenes/SkyBox.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "../GameState.h"
#include "../Player.h"

#include "Floor.h"
#include "EnemySpawnPoint.h"
#include "ParticleSystem.h"

class Scene
{
protected:
	std::string m_name;
	Player* m_player;
	ParticleSystem m_particleSystem;

	std::vector<std::unique_ptr<Projectile>> m_projectiles;
	std::vector<std::unique_ptr<EnemySpawnPoint>> m_enemySpawnPoints;
	std::vector<std::shared_ptr<Entity>> m_Entities;
	ParticleProps m_explosionProp;
	//std::vector<ParticleProps> m_particlePool;

	std::unique_ptr<Floor> m_floor;
	SkyBox m_skyBox;
	ProgramObject m_program;

	void UpdateProjectiles(const float&);
	void UpdateEntities(const float&, GameState&);

	bool CheckForCollision();
	bool DetectHits();

	void DrawSkyBox(glm::mat4&, glm::vec3);
	void DrawEntities(glm::mat4&);
	void DrawProjectiles(glm::mat4&, ProgramObject&);

	void Explosion(glm::vec3&);

public:
	Scene();
	std::vector<std::shared_ptr<Entity>>& GetEntities();
	std::vector<std::unique_ptr<EnemySpawnPoint>>& GetSpawnPoints();
	std::vector<std::shared_ptr<Entity>>* GetEntitiesPtr();
	ProgramObject& getProgram();
	std::string& getName();
	std::unique_ptr<Floor>& GetFloor();

	bool Update(const float&, GameState&);

	void AddEntity(std::shared_ptr<Entity> entity);

	virtual void LoadScene() {};
	void DrawScene(glm::mat4&, GameState&, glm::vec3, ProgramObject&);
};