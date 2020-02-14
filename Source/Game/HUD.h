#pragma once
#include <Component/Transform/Transform.h>
#include <vector>

class HUD
{
	private:
		glm::vec2 lifeBgPosition;
		glm::vec2 hearthPositions;
		glm::vec2 fuelBgPosition;

	public:
		HUD();
		~HUD();

		glm::mat4 getLifeBg(int nOfLives);
		glm::mat4 getHeartPos();

		glm::mat4 getFuelBg(float fuelPercent);
		glm::mat4 getFuelPos();
};

