#include "../../headers/collision_detection/HitBox.h"

void HitBox::Draw(ProgramObject& program, glm::mat4& viewproj)
{
	

	std::vector<glm::vec3> Points;

	glm::vec3 leftDB = Position;
	leftDB.x -= dimensions.width / 2;
	leftDB.y -= dimensions.height / 2;
	leftDB.z -= dimensions.length / 2;

	glm::vec3 rightDB = Position;
	rightDB.x += dimensions.width / 2;
	rightDB.y -= dimensions.height / 2;
	rightDB.z -= dimensions.length / 2;

	glm::vec3 rightUB = Position;
	rightUB.x += dimensions.width / 2;
	rightUB.y += dimensions.height / 2;
	rightUB.z -= dimensions.length / 2;

	glm::vec3 leftUB = Position;
	leftUB.x -= dimensions.width / 2;
	leftUB.y += dimensions.height / 2;
	leftUB.z -= dimensions.length / 2;


	glm::vec3 leftDF = Position;
	leftDF.x -= dimensions.width / 2;
	leftDF.y -= dimensions.height / 2;
	leftDF.z += dimensions.length / 2;

	glm::vec3 rightDF = Position;
	rightDF.x += dimensions.width / 2;
	rightDF.y -= dimensions.height / 2;
	rightDF.z += dimensions.length / 2;

	glm::vec3 rightUF = Position;
	rightUF.x += dimensions.width / 2;
	rightUF.y += dimensions.height / 2;
	rightUF.z += dimensions.length / 2;

	glm::vec3 leftUF = Position;
	leftUF.x -= dimensions.width / 2;
	leftUF.y += dimensions.height / 2;
	leftUF.z += dimensions.length / 2;
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------

	Points.push_back(leftDB);
	Points.push_back(rightDB);

	Points.push_back(rightDB);
	Points.push_back(rightUB);

	Points.push_back(rightUB);
	Points.push_back(leftUB);

	Points.push_back(leftUB);
	Points.push_back(leftDB);

	Points.push_back(leftDB);
	Points.push_back(leftDF);

	Points.push_back(leftUB);
	Points.push_back(leftUF);

	Points.push_back(leftUF);
	Points.push_back(leftDF);

	Points.push_back(leftUF);
	Points.push_back(rightUF);

	Points.push_back(rightUF);
	Points.push_back(rightDF);

	Points.push_back(rightDF);
	Points.push_back(leftDF);

	Points.push_back(rightDF);
	Points.push_back(rightDB);

	Points.push_back(rightUF);
	Points.push_back(rightUB);

	program.Use();

	program.SetUniform("mvp", viewproj);
	program.SetUniform("points", Points);
	glDrawArrays(GL_LINES, 0, (GLsizei)Points.size());

	program.Unuse();

	Points.clear();
}