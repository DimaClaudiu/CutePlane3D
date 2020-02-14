#include "Obstacles.h"
#include "MyPlane.h"
#include <iostream>

Obstacles::Obstacles()
{
	scale = 0.5f;
	rotation = 0;
	count = 10;
	for (int i = 0; i < count;i++)
		positions.push_back(glm::vec2((float)(rand() % 4000 + 1600) / 100, (float)(rand() % 550 + 350) / 100));
	color = glm::vec3(0.8f, 0.33f, 0.2f);
	hitbox = scale / 2;
	speed = 5.0f;
}

Obstacles::~Obstacles()
{
}

glm::mat4 Obstacles::move(float deltatime, int pos)
{
	if (positions[pos].x <= -1)
		positions[pos] = glm::vec2((float)(rand() % 4000 + 1600) / 100, (float)(rand() % 500 + 350) / 100);

	rotation += deltatime / 2.0f;
	positions[pos].x -= deltatime * speed;
	speed += deltatime / 100.0f;
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(positions[pos].x, positions[pos].y, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(1, 1, 1));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

	return modelMatrix;
}

bool Obstacles::checkCollision(glm::vec2 planePos, float planeHitbox, int pos)
{
	if (positions[pos].x > 10 || positions[pos].x < 6)
		return false;

	glm::vec2 p1 = planePos;
	glm::vec2 p2 = positions[pos];

	float distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

	if (distance <= hitbox + planeHitbox)
	{
		positions[pos] = glm::vec2((float)(rand() % 4000 + 1600) / 100, (float)(rand() % 500 + 350) / 100);
		std::cout << "Hit\n";
		return true;
	}

	return false;
}



