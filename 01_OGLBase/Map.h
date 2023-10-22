#pragma once

#include <vector>
#include <string>
#include <memory>

#include "includes/ProgramObject.h"
#include "includes/TextureObject.h"

#include "Player.h"
#include "GameState.h"

class Map
{
protected:
	Player* m_player;
	std::vector<std::unique_ptr<Projectile>>* m_projectiles;
	TextureCubeMap	m_skyboxTexture;
	std::vector<std::shared_ptr<Entity>> m_Entities;
	ProgramObject m_program;

public:
	Map();
	std::vector<std::shared_ptr<Entity>>& GetEntities();
	std::vector<std::shared_ptr<Entity>>* GetEntitiesPtr();
	TextureCubeMap& GetSkyBox();
	ProgramObject& getProgram();
	void ClearMap();
	void AddEntity(std::shared_ptr<Entity> entity);
	void SetSkyBox(std::string, std::string, std::string, std::string ,std::string ,std::string);

	virtual void LoadMap() {};
	virtual void DrawEntities(glm::mat4&, GameState&) {};
};