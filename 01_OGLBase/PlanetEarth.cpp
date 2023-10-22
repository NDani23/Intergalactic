#include "PlanetEarth.h"


PlanetEarth::PlanetEarth(std::vector<std::unique_ptr<Projectile>>* projectiles, Player* player)
{
	m_name = "Planet Earth";

	m_player = player;
	m_projectiles = projectiles;

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

	m_floorProgram.AttachShaders({
		{ GL_VERTEX_SHADER, "proceduralVert.vert"},
		{ GL_FRAGMENT_SHADER, "proceduralFrag.frag"}
		});

	m_floorProgram.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_norm" },
		{ 2, "vs_in_col" },
		});

	m_floorProgram.LinkProgram();

	InitFloor();

	SetSkyBox("assets/xpos.png", "assets/xneg.png", "assets/ypos.png", "assets/yneg.png", "assets/zpos.png", "assets/zneg.png");
}

void PlanetEarth::LoadMap()
{
	AddEntity(std::make_shared<Entity>("assets/ufo.obj", glm::vec3(500, 200, 1000), "assets/ufo_tex.png", Dimensions{ 50.0f, 17.0f, 50.0f }));
}

void PlanetEarth::DrawEntities(glm::mat4& viewproj, GameState& state)
{
	DrawFloor(viewproj);
	m_program.Use();
	if (state.play)
	{
		for (std::shared_ptr<Entity>& entity : m_Entities)
		{
			
			entity->DrawMesh(m_program, viewproj);
		}
	}
	m_program.Unuse();
}

void PlanetEarth::InitFloor()
{
	int N = 100;
	int M = 100;


	std::vector<Vertex> vert((N + 1) * (M + 1));
	for (int j = 0; j <= M; ++j)
	{
		for (int i = 0; i <= N; ++i)
		{
			float u = i / (float)N;
			float v = j / (float)M;
			int index = i + j * (N + 1);
			vert[index].p = GetFloorUV(u, v);
			vert[index].n = GetFloorNorm(u, v);
			vert[index].c = glm::vec3(194.f/255.f, 178.f/255.f, 128.f/255.f);
		}
	}



	std::vector<GLushort> indices(3 * 2 * (N) * (M));
	for (int j = 0; j < M; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			int index = i * 6 + j * (6 * N);
			indices[index + 0] = (i)+(j) * (N + 1);
			indices[index + 1] = (i + 1) + (j) * (N + 1);
			indices[index + 2] = (i)+(j + 1) * (N + 1);
			indices[index + 3] = (i + 1) + (j) * (N + 1);
			indices[index + 4] = (i + 1) + (j + 1) * (N + 1);
			indices[index + 5] = (i)+(j + 1) * (N + 1);
		}
	}

	glGenVertexArrays(1, &m_FloorVaoID);
	glBindVertexArray(m_FloorVaoID);

	glGenBuffers(1, &m_FloorVboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_FloorVboID);
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

	glGenBuffers(1, &m_FloorIbID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FloorIbID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le

}

glm::vec3 PlanetEarth::GetFloorUV(float u, float v)
{
	return glm::vec3((v) * 110.f,(cos(v * 50.f) + 2 * sin(u * 20.f)) * 0.3f , (u) * 110.f);
}

glm::vec3 PlanetEarth::GetFloorNorm(float u, float v)
{
	glm::vec3 du = GetFloorUV(u + 0.01, v) - GetFloorUV(u - 0.01, v);
	glm::vec3 dv = GetFloorUV(u, v + 0.01) - GetFloorUV(u, v - 0.01);

	return glm::normalize(glm::cross(du, dv));

}

void PlanetEarth::DrawFloor(glm::mat4& viewproj)
{
	int N = 100;
	int M = 100;

	glm::mat4 basetransform = glm::translate(glm::vec3(-2600, -50, -2600));
	m_floorProgram.Use();
	glBindVertexArray(m_FloorVaoID);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			glm::mat4 transform = basetransform * glm::translate(glm::vec3((i-1) * 5500, 0, (j-1) * 5200)) * glm::scale(glm::vec3(50.f, 50.f, 50.f));
			m_floorProgram.SetUniform("MVP", viewproj * transform);
			m_floorProgram.SetUniform("world", transform);
			m_floorProgram.SetUniform("worldIT", glm::inverse(glm::transpose(transform)));
			glDrawElements(GL_TRIANGLES,
				3 * 2 * (N) * (M),
				GL_UNSIGNED_SHORT,
				0);
		}
	}
	


	// VAO kikapcsolasa
	glBindVertexArray(0);

	m_floorProgram.Unuse();
}