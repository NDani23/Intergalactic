#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "../../includes/ProgramObject.h"
#include "../../includes/BufferObject.h"
#include "../../includes/VertexArrayObject.h"
#include "../../includes/TextureObject.h"

class SkyBox
{
private:
	VertexArrayObject	m_Vao;
	IndexBuffer			m_Indices;
	ArrayBuffer			m_Pos;

	TextureCubeMap		m_Texture;

	ProgramObject		m_Program;
public:
	SkyBox();

	void SetShaders(std::string, std::string);
	void SetTexture(std::string, std::string, std::string, std::string, std::string, std::string);
	void Draw(glm::mat4&,glm::vec3);
};
