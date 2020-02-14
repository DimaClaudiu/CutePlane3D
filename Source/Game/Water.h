#pragma once
#include <Component/Transform/Transform.h>
#include <vector>

class Water
{
	private:
		int segCount;
		std::vector<float> positions;
		float size;
		float width;
		
		glm::mat4 modelMatrix;
		glm::vec3 color;

	public:
		Water();
		~Water();

		glm::mat4 move(float deltaTime, int pos);
		int getSegCount() { return segCount; }
		glm::vec3 getColor() { return color; }
};

