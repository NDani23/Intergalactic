#include "InitMap1.h"

void Map1::InitMap(Map& outMap)
{
	outMap.SetSkyBox("assets/right.png", "assets/left.png", "assets/top.png", "assets/bottom.png", "assets/front.png", "assets/back.png");
	
	//CreateMeteorField(outMap);
}

static void Map1::CreateMeteorField(Map& outMap)
{
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-800, 300, 2000) ,"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-820, 300, 2100), glm::rotate(1.0f, glm::vec3(1,0,1)) ,"assets/MET01_tex.jpg"});
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-910, 310, 2150), glm::rotate(2.3f, glm::vec3(1,0,0)) * glm::scale(glm::vec3(1.2f, 1.2f, 1.2f)) ,"assets/MET01_tex.jpg"});
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-900, 400, 2200), glm::rotate(1.3f, glm::vec3(0,0,1)) ,"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-750, 250, 1800), glm::rotate(1.0f, glm::vec3(0,1,1)) * glm::scale(glm::vec3(0.8f, 0.8f, 0.8f)) ,"assets/MET01_tex.jpg"});
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-700, 280, 2000), glm::rotate(0.6f, glm::vec3(0,1,0)) * glm::scale(glm::vec3(0.6f, 0.6f, 0.6f)) ,"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-800, 500, 2050), glm::scale(glm::vec3(1.5f, 1.5f, 1.5f)) ,"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-1000, 500, 1750), glm::rotate(-1.0f, glm::vec3(0,1,1)) * glm::scale(glm::vec3(0.8f, 0.8f, 0.8f)) ,"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-950, 400, 1800), glm::rotate(-1.6f, glm::vec3(0,0,1)) * glm::scale(glm::vec3(1.3f, 1.3f, 1.3f)) ,"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-900, 350, 1900), glm::rotate(0.3f, glm::vec3(1,1,0)) * glm::scale(glm::vec3(0.8f, 0.8f, 0.8f)) ,"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-850, 325, 1930), glm::rotate(0.8f, glm::vec3(0,1,0)),"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-820, 400, 1950), glm::rotate(2.3f, glm::vec3(1,0,1)),"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-950, 280, 1910), glm::rotate(1.5f, glm::vec3(1,1,0)),"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-800, 200, 2000), glm::rotate(0.3f, glm::vec3(0,1,1)),"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-780, 340, 1980), glm::rotate(1.2f, glm::vec3(0,0,1)),"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-980, 470, 1800), glm::rotate(-0.5f, glm::vec3(1,1,1)) * glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)) ,"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-950, 450, 1780), glm::rotate(-1.3f, glm::vec3(0,1,1)) * glm::scale(glm::vec3(0.3f, 0.3f, 0.3f)) ,"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-900, 470, 1750), glm::rotate(0.8f, glm::vec3(1,1,1)) * glm::scale(glm::vec3(0.4f, 0.4f, 0.4f)) ,"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-860, 420, 1830), glm::rotate(-0.5f, glm::vec3(0,0,1)) * glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)) ,"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-830, 390, 1870), glm::rotate(2.2f, glm::vec3(1,0,0)) * glm::scale(glm::vec3(0.3f, 0.3f, 0.3f)) ,"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-788, 335, 1930), glm::rotate(1.3f, glm::vec3(0,1,1)) * glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)) ,"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-700, 300, 2100), glm::rotate(0.5f, glm::vec3(1,1,1)) * glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)) ,"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-750, 280, 2000), glm::rotate(-1.3f, glm::vec3(0,1,1)) * glm::scale(glm::vec3(0.3f, 0.3f, 0.3f)) ,"assets/MET01_tex.jpg" });
	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(-800, 350, 2150), glm::rotate(0.8f, glm::vec3(1,1,1)) * glm::scale(glm::vec3(0.4f, 0.4f, 0.4f)) ,"assets/MET01_tex.jpg" });
}