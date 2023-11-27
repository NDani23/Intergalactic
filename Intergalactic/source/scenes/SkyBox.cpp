#include "headers/scenes/SkyBox.h"

SkyBox::SkyBox()
{
	m_Pos.BufferData(
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


	m_Indices.BufferData(
		std::vector<int>{
		// hátsó lap
		0, 1, 2,
		2, 3, 0,
		// elülsõ lap
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
		// felsõ
		3, 2, 6,
		3, 6, 7,
	}
	);

	m_Vao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_Pos },
		}, m_Indices
		);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void SkyBox::SetTexture(std::string right,
	std::string left,
	std::string top,
	std::string bottom,
	std::string front,
	std::string back)
{
	m_Texture.AttachFromFile(right, false, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	m_Texture.AttachFromFile(left, false, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	m_Texture.AttachFromFile(top, false, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	m_Texture.AttachFromFile(bottom, false, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	m_Texture.AttachFromFile(front, false, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	m_Texture.AttachFromFile(back, true, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
}

void SkyBox::SetShaders(std::string vert_source, std::string frag_source)
{
	m_Program.AttachShaders({
		{ GL_VERTEX_SHADER, vert_source},
		{ GL_FRAGMENT_SHADER, frag_source}
		});

	m_Program.BindAttribLocations({
		{ 0, "vs_in_pos" },
		});

	m_Program.LinkProgram();
}

void SkyBox::Draw(glm::mat4& viewProj, glm::vec3 eye_pos)
{
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	glDepthFunc(GL_LEQUAL);

	m_Vao.Bind();

	m_Program.Use();
	m_Program.SetUniform("MVP", viewProj * glm::translate(eye_pos));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
	glUniform1i(m_Program.GetLocation("skyboxTexture"), 0);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	m_Program.Unuse();

	glDepthFunc(prevDepthFnc);
}