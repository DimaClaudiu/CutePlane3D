#include "Plane.h"
#include "MyPlane.h"
#include <iostream>

Plane::Plane(glm::vec2 aspectRatio1, glm::vec2 resolution1)
{
	aspectRatio = aspectRatio1;
	resolution = resolution1;
	Plane();
}

Plane::Plane()
{
	aspectRatio = glm::vec2(16, 9);
	resolution = glm::vec2(1920, 1080);
	rotation = 0;
	rotationOffset = 0;
	crossRotation = 0;
	position = glm::vec3(aspectRatio.x / 2, aspectRatio.y / 2, 0);
	size = 0.6;
	hitbox = 0.5;
	modelMatrix = glm::mat4(1);
	lives = 3;
	fuel = 100;
	vx = vy = ax = ay = 0.0f;
}

Plane::~Plane()
{
}

glm::mat4 Plane::move(float deltaTime, bool gameStart)
{
	if(gameStart)
		consumeFuel(deltaTime);

	position.x += vx * deltaTime;
	position.y += vy * deltaTime;

	if(vx > 0 && lives > 0)
		vx += -(position.x - 8.0f) / 2.0f * deltaTime;
	else if(vx < 0 && lives > 0)
		vx += -(position.x - 8.0f) * 160.0f * deltaTime;

	if (position.x > 8 && vy == 0)
	{
		position.x = 8;
		vx = ax = 0;
	}

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);

	crossRotation += 5*deltaTime + abs(rotation);
	if (lives <= 0) {
		rotation += rotationOffset;
		modelMatrix = glm::rotate(modelMatrix, -rotation, glm::vec3(1, 1, 1));
	}
	else
	{
		if (rotation > 0.05)
			rotation -= deltaTime * (1 + abs(rotation));
		else if (rotation < -0.05)
			rotation += deltaTime * (1 + abs(rotation));

		modelMatrix = glm::rotate(modelMatrix, -rotation, glm::vec3(2, 0, 1));
	}
	modelMatrix = glm::scale(modelMatrix, glm::vec3(size));

	return modelMatrix;
}

glm::mat4 Plane::getBumperPos()
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	if (vy != 0)
		modelMatrix = glm::rotate(modelMatrix, -rotation, glm::vec3(1, 1, 1));
	else
		modelMatrix = glm::rotate(modelMatrix, -rotation, glm::vec3(2, 0, 1));

	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.35f, 0, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.60f, 0.60f));
	
	return modelMatrix;
}

glm::mat4 Plane::getCrossPos()
{
	glm::mat4 modelMatrix = glm::mat4(1);
	//modelMatrix = glm::rotate(modelMatrix, crossRotation, glm::vec3(0, 1, 0));
	modelMatrix = glm::translate(modelMatrix, position);
	if (vy != 0)
		modelMatrix = glm::rotate(modelMatrix, -rotation, glm::vec3(1, 1, 1));
	else
		modelMatrix = glm::rotate(modelMatrix, -rotation, glm::vec3(2, 0, 1));

	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.55f, 0, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6));

	modelMatrix = glm::rotate(modelMatrix, AI_MATH_HALF_PI_F, glm::vec3(0, 0, 1));
	modelMatrix = glm::rotate(modelMatrix, crossRotation, glm::vec3(0, 1, 0));

	return modelMatrix;
}

glm::mat4 Plane::getWingCrossPos()
{
	glm::mat4 modelMatrix = glm::mat4(1);
	//modelMatrix = glm::rotate(modelMatrix, crossRotation, glm::vec3(0, 1, 0));
	modelMatrix = glm::translate(modelMatrix, position);
	if (vy != 0)
		modelMatrix = glm::rotate(modelMatrix, -rotation, glm::vec3(1, 1, 1));
	else
		modelMatrix = glm::rotate(modelMatrix, -rotation, glm::vec3(2, 0, 1));

	modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.77f, 0.36f, 0.15));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4));

	modelMatrix = glm::rotate(modelMatrix, AI_MATH_HALF_PI_F, glm::vec3(0, 0, 1));
	modelMatrix = glm::rotate(modelMatrix, -AI_MATH_HALF_PI_F, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, crossRotation, glm::vec3(0, 1, 0));

	return modelMatrix;
}

void Plane::followMouse(int x, int y, int dx, int dy)
{
	if(vy == 0)
	position.y = aspectRatio.y - y * aspectRatio.y / resolution.y;

	rotation += dy / ( 3.0f * 100.0f);
}

bool Plane::takeDamage(float obstacleY)
{
	lives--;
	if (lives > 0)
	{
		vx = -20;
		ax = 15;
		/*std::cout << (position.y - obstacleY) << "\n";
		if ((position.y - obstacleY) < 0)
			vy = 40;
		else
			vy = -50;*/
		return false;
	}
	else
		kill();
	return true;
}

bool Plane::consumeFuel(float deltatime)
{
	if (fuel <= 0) {
		if(lives > 0)
			kill();
		return true;
	}

	fuel -= deltatime * 3.0f;
	return false;
}

void Plane::kill()
{
	vx = -2;
	ax = 15;
	vy = -5;
	rotationOffset = 5.0f / 100.0f;
}


