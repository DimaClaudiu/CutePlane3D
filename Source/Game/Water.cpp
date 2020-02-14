#include "Water.h"
#include "MyPlane.h"

Water::Water()
{
	size = 2.0f;
	segCount = 6;
	width = 8.8f;
	for (int i = 0; i < segCount; i++)
		positions.push_back((i - 1) * width);
	color = glm::vec3(0.445, 0.912, 1);
}

Water::~Water()
{
}

glm::mat4 Water::move(float deltaTime, int pos)
{
		positions[pos] -= deltaTime;
		if (positions[pos] <= -width)
			positions[pos] = (segCount - 1) * width;
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(positions[pos], 2.5, -3));
		modelMatrix = glm::rotate(modelMatrix, 0.08726646f, glm::vec3(1, 0, 0)); // 5 degrees
		modelMatrix = glm::scale(modelMatrix, glm::vec3(size));

		return modelMatrix;
}