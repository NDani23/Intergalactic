#include "../../headers/scenes/DesertFloor.h"

DesertFloor::DesertFloor()
{
	m_Res = glm::vec2(99,99);

	m_Program.AttachShaders({
		{ GL_VERTEX_SHADER, "shaders/proceduralVert.vert"},
		{ GL_FRAGMENT_SHADER, "shaders/proceduralFrag.frag"}
		});

	m_Program.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_norm" },
		{ 2, "vs_in_col" },
		});

	m_Program.LinkProgram();


	std::vector<Vertex> vert((m_Res.x + 1) * (m_Res.y + 1));
	for (int j = 0; j <= m_Res.y; ++j)
	{
		for (int i = 0; i <= m_Res.x; ++i)
		{
			float u = i / (float)m_Res.x;
			float v = j / (float)m_Res.y;
			int index = i + j * (m_Res.x + 1);
			vert[index].p = GetUV(u, v);
			vert[index].n = GetNorm(u, v);
			vert[index].c = glm::vec3(154.f/255.f, 138.f/255.f, 108.f/255.f);
		}
	}



	std::vector<GLushort> indices(3 * 2 * (m_Res.x) * (m_Res.y));
	for (int j = 0; j < m_Res.y; ++j)
	{
		for (int i = 0; i < m_Res.x; ++i)
		{
			int index = i * 6 + j * (6 * m_Res.x);
			indices[index + 0] = (i)+(j) * (m_Res.x + 1);
			indices[index + 1] = (i + 1) + (j) * (m_Res.x + 1);
			indices[index + 2] = (i)+(j + 1) * (m_Res.x + 1);
			indices[index + 3] = (i + 1) + (j) * (m_Res.x + 1);
			indices[index + 4] = (i + 1) + (j + 1) * (m_Res.x + 1);
			indices[index + 5] = (i)+(j + 1) * (m_Res.x + 1);
		}
	}

	glGenVertexArrays(1, &m_VaoID);
	glBindVertexArray(m_VaoID);

	glGenBuffers(1, &m_VboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_VboID);
	glBufferData(GL_ARRAY_BUFFER,
		vert.size() * sizeof(Vertex),
		vert.data(),
		GL_STATIC_DRAW);

	// VAO-ban jegyezzük fel, hogy a VBO-ban az elsõ 3 float sizeof(Vertex)-enként lesz az elsõ attribútum (pozíció)
	glEnableVertexAttribArray(0); // ez lesz majd a pozíció
	glVertexAttribPointer(
		0,				// a VB-ben található adatok közül a 0. "indexû" attribútumait állítjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymás után)
		0				// a 0. indexû attribútum hol kezdõdik a sizeof(Vertex)-nyi területen belül
	);

	// a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín)
	glEnableVertexAttribArray(1); // ez lesz majd a normál
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)));

	glEnableVertexAttribArray(2); // ez lesz majd a szín
	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3) * 2));

	glGenBuffers(1, &m_IbID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IbID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le
}

glm::vec3 DesertFloor::GetUV(float u, float v)
{
	u = u * 5000 - 2500;
	v = v * 5000 - 2500;

	return glm::vec3(v, GetZCoord(v, u), u);
}

glm::vec3 DesertFloor::GetNorm(float u, float v)
{
	glm::vec3 du = GetUV(u + 0.01, v) - GetUV(u - 0.01, v);
	glm::vec3 dv = GetUV(u, v + 0.01) - GetUV(u, v - 0.01);

	return glm::normalize(glm::cross(du, dv));
}

float DesertFloor::GetZCoord(float x, float y)
{
	return sin(x * 0.005026f) * 20.f + cos(y * 0.005f) * 30.f - 50;
}


void DesertFloor::DrawFloor(glm::mat4& viewproj, Player* player)
{
	int N = 99;
	int M = 99;

	m_Program.Use();
	glBindVertexArray(m_VaoID);

	int player_offsetX = std::round(player->GetPos().x / 5000.f);
	int player_offsetZ = std::round(player->GetPos().z / 5000.f);


	glm::mat4 BaseTransform = glm::translate(glm::vec3(5000 * player_offsetX, 0, 5000 * player_offsetZ));

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{

			glm::mat4 transform = BaseTransform * glm::translate(glm::vec3((i - 1) * 5000, 0, (j - 1) * 5000));
			m_Program.SetUniform("MVP", viewproj * transform);
			m_Program.SetUniform("world", transform);
			m_Program.SetUniform("worldIT", glm::inverse(glm::transpose(transform)));
			m_Program.SetUniform("playerPos", player->GetPos());
			glDrawElements(GL_TRIANGLES,
				3 * 2 * (N) * (M),
				GL_UNSIGNED_SHORT,
				0);
		}
	}



	// VAO kikapcsolasa
	glBindVertexArray(0);

	m_Program.Unuse();
}

bool DesertFloor::DetectCollision(Entity& entity)
{
	float floorHeightBeneath = GetZCoord(entity.GetPos().x, entity.GetPos().z);
	return entity.GetPos().y <= floorHeightBeneath;
}