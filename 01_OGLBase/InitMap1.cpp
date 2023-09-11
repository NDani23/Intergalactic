#include "InitMap1.h"

void Map1::InitMap(Map& outMap)
{
	outMap.SetSkyBox("assets/right.png", "assets/left.png", "assets/top.png", "assets/bottom.png", "assets/front.png", "assets/back.png");

	//outMap.AddEntity({ "assets/meteor.obj", glm::vec3(0, 0, 500) ,"assets/meteor_tex.jpg", {27.0f, 27.0f, 27.0f}});
	
	CreateMeteorField(outMap);
}

static void Map1::CreateMeteorField(Map& outMap)
{
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-800, 300, 1000) ,"assets/meteor_tex.jpg", {27.0f, 27.0f, 27.0f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-820, 300, 1100), glm::rotate(1.0f, glm::vec3(1,0,1)) ,"assets/meteor_tex.jpg", {27.0f, 27.0f, 27.0f}});
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-910, 310, 1150), glm::rotate(2.3f, glm::vec3(1,0,0)) * glm::scale(glm::vec3(1.2f, 1.2f, 1.2f)) ,"assets/meteor_tex.jpg" , {32.4f, 32.4f, 32.5f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-900, 400, 1200), glm::rotate(1.3f, glm::vec3(0,0,1)) ,"assets/meteor_tex.jpg", {27.0f, 27.0f, 27.0f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-750, 250, 800), glm::rotate(1.0f, glm::vec3(0,1,1)) * glm::scale(glm::vec3(0.8f, 0.8f, 0.8f)) ,"assets/meteor_tex.jpg", {21.6f, 21.6f, 21.6f}});
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-700, 280, 1000), glm::rotate(0.6f, glm::vec3(0,1,0)) * glm::scale(glm::vec3(2.0f, 2.0f, 2.0f)) ,"assets/meteor_tex.jpg", {54.0f, 54.0f, 54.0f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-800, 500, 1050), glm::scale(glm::vec3(1.5f, 1.5f, 1.5f)) ,"assets/meteor_tex.jpg", {40.5f, 40.5f, 40.5f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-1000, 500, 750), glm::rotate(-1.0f, glm::vec3(0,1,1)) * glm::scale(glm::vec3(0.8f, 0.8f, 0.8f)) ,"assets/meteor_tex.jpg", {21.6f, 21.6f, 21.6f}});
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-950, 400, 800), glm::rotate(-1.6f, glm::vec3(0,0,1)) * glm::scale(glm::vec3(1.3f, 1.3f, 1.3f)) ,"assets/meteor_tex.jpg", {35.1f, 35.1f, 35.1f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-900, 350, 900), glm::rotate(0.3f, glm::vec3(1,1,0)) * glm::scale(glm::vec3(0.8f, 0.8f, 0.8f)) ,"assets/meteor_tex.jpg", {21.6f, 21.6f, 21.6f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-850, 325, 930), glm::rotate(0.8f, glm::vec3(0,1,0)),"assets/meteor_tex.jpg", {27.0f, 27.0f, 27.0f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-820, 400, 950), glm::rotate(2.3f, glm::vec3(1,0,1)),"assets/meteor_tex.jpg", {27.0f, 27.0f, 27.0f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-950, 280, 910), glm::rotate(1.5f, glm::vec3(1,1,0)),"assets/meteor_tex.jpg", {27.0f, 27.0f, 27.0f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-800, 200, 1000), glm::rotate(0.3f, glm::vec3(0,1,1)),"assets/meteor_tex.jpg", {27.0f, 27.0f, 27.0f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-780, 340, 980), glm::rotate(1.2f, glm::vec3(0,0,1)),"assets/meteor_tex.jpg", {27.0f, 27.0f, 27.0f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-980, 470, 800), glm::rotate(-0.5f, glm::vec3(1,1,1)) * glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)) ,"assets/meteor_tex.jpg", {13.5f, 13.5f, 13.5f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-950, 450, 780), glm::rotate(-1.3f, glm::vec3(0,1,1)) * glm::scale(glm::vec3(0.6f, 0.6f, 0.6f)) ,"assets/meteor_tex.jpg", {16.2f, 16.2f, 16.2f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-900, 470, 750), glm::rotate(0.8f, glm::vec3(1,1,1)) * glm::scale(glm::vec3(1.7f, 1.7f, 1.7f)) ,"assets/meteor_tex.jpg", {45.9f, 45.9f, 45.9f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-860, 420, 830), glm::rotate(-0.5f, glm::vec3(0,0,1)) * glm::scale(glm::vec3(0.8f, 0.8f, 0.8f)) ,"assets/meteor_tex.jpg", {21.6f, 21.6f, 21.6f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-830, 390, 870), glm::rotate(2.2f, glm::vec3(1,0,0)) * glm::scale(glm::vec3(2.0f, 2.0f, 2.0f)) ,"assets/meteor_tex.jpg", {54.0f, 54.0f, 54.0f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-788, 335, 930), glm::rotate(1.3f, glm::vec3(0,1,1)) ,"assets/meteor_tex.jpg", {27.0f, 27.0f, 27.0f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-700, 300, 1100), glm::rotate(0.5f, glm::vec3(1,1,1)) * glm::scale(glm::vec3(1.3f, 1.3f, 1.3f)) ,"assets/meteor_tex.jpg", {35.1f, 35.1f, 35.1f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-750, 280, 1000), glm::rotate(-1.3f, glm::vec3(0,1,1)) * glm::scale(glm::vec3(0.3f, 0.3f, 0.3f)) ,"assets/meteor_tex.jpg", {8.1f, 8.1f, 8.1f} });
	outMap.AddEntity({ "assets/meteor.obj", glm::vec3(-800, 350, 1150), glm::rotate(0.8f, glm::vec3(1,1,1)) * glm::scale(glm::vec3(0.4f, 0.4f, 0.4f)) ,"assets/meteor_tex.jpg", {10.8f, 10.8f, 10.8f} });
}