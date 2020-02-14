#include "Fuel.h"
#include "MyPlane.h"

Fuel::Fuel()
{
	maxLen = 14;
	for (int i = 0; i <= maxLen; i++)
		positions.push_back(glm::vec2(-1));
	scale = 0.5f;
	rotation = 0;
	hitbox = 0.15f;
	color = glm::vec3(0.4f, 1.0f, 0.0f);
	count = generateFuelStrip(maxLen);
}

Fuel::~Fuel()
{
}

int Fuel::generateFuelStrip(int maxLen)
{
	float maxLenf = maxLen;
	float startPos = (float)(rand() % 2 * 314) / 100;
	float startingHeight = (float)(rand() % 400) / 100 + 3.0f;
	float startingX = (float)(rand() % 4000 + 4000) / 100;

	int minCount = 8;
	int count = rand() % (maxLen - minCount) + minCount;
	int c = 0;
	for (float i = 0; i < 2 * AI_MATH_PI * count / maxLenf; i += 2 * AI_MATH_PI / maxLenf)
	{
		positions[c] = (glm::vec2(i + startingX, sin(i + startPos) + startingHeight));
		c++;
	}

	return count;
}

glm::mat4 Fuel::move(float deltatime, int pos)
{
	positions[pos].x -= deltatime * 5;
	if (positions[pos].x < -1 && pos == count - 1)
		count = generateFuelStrip(maxLen);

	rotation += deltatime / 2.0f;
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(positions[pos].x, positions[pos].y, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0, 0, 1));
	modelMatrix = glm::rotate(modelMatrix, AI_MATH_HALF_PI_F, glm::vec3(1, 0, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f * scale, 0.6f * scale, scale));

	return modelMatrix;
}

bool Fuel::checkCollision(glm::vec2 planePos, float planeHitbox, int pos)
{
	if (positions[pos].x > 10 || positions[pos].x < 6)
		return false;

	glm::vec2 p1 = planePos;
	glm::vec2 p2 = positions[pos];

	float distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

	if (distance <= hitbox + planeHitbox)
	{
		positions[pos].y = -1;
		return true;
	}

	return false;
}
