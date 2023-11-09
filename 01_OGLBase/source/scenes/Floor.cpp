#include "../../headers/scenes/Floor.h"

glm::vec3 Floor::GetUV(float, float) { return glm::vec3(0, 0, 0); };
glm::vec3 Floor::GetNorm(float, float) { return glm::vec3(0, 1, 0); };
float Floor::GetZCoord(float, float) { return 0; };
bool Floor::DetectCollision(Entity&) { return false; };