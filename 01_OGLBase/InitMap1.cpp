#include "InitMap1.h"

void Map1::InitMap(Map& outMap)
{
	outMap.SetSkyBox("assets/right.png", "assets/left.png", "assets/top.png", "assets/bottom.png", "assets/front.png", "assets/back.png");

	outMap.AddEntity({ "assets/MET01.obj", glm::vec3(50, 30, 250), "assets/MET01_tex.jpg" });
}