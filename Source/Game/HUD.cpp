#include "HUD.h"
#include "MyPlane.h"
#include <iostream>

HUD::HUD()
{
	lifeBgPosition = glm::vec2(2.5f, 7.5f);
	hearthPositions = glm::vec2(1.60f, 7.24f);

	fuelBgPosition = glm::vec2(2.5f, 7.0f);

}

HUD::~HUD()
{
}

glm::mat4 HUD::getLifeBg(int nOfLives)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(lifeBgPosition.x - (1- nOfLives / 3.0), lifeBgPosition.y , 1));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.9 * nOfLives / 3.0, 0.3, 0.001f));

	return modelMatrix;
}

glm::mat4 HUD::getHeartPos()
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(hearthPositions, 1));
	modelMatrix = glm::rotate(modelMatrix, AI_MATH_HALF_PI_F/4.0f, glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.08f, 0.1f, 0.08f));

	return modelMatrix;
}

glm::mat4 HUD::getFuelBg(float fuelPercent)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(fuelBgPosition.x - (1 - fuelPercent / 100.0f), fuelBgPosition.y, 1));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.9 * (fuelPercent / 100.0f), 0.3f, 0.001f));

	return modelMatrix;
}

glm::mat4 HUD::getFuelPos()
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(1.5f, 7.15f, 1));
	modelMatrix = glm::rotate(modelMatrix, AI_MATH_HALF_PI_F, glm::vec3(1, 0, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 0.5f));

	return modelMatrix;
}
