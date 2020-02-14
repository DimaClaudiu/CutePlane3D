#pragma once
#include <Component/Transform/Transform.h>


class Plane
{
	private:
		glm::vec3 position;
		float size;
		float hitbox;
		glm::mat4 modelMatrix;
		float rotation;
		float rotationOffset;
		float crossRotation;
		int lives;
		float fuel;

		glm::vec2 aspectRatio;
		glm::vec2 resolution;

		float vx, vy, ax, ay;

	public:
		Plane();
		Plane(glm::vec2 aspectRatio1, glm::vec2 resolution1);
		~Plane();

		glm::mat4 move(float deltaTime, bool gameStart);

		glm::mat4 getBumperPos();

		glm::mat4 getCrossPos();

		glm::mat4 getWingCrossPos();

		void followMouse(int x, int y, int dx, int dy);

		bool takeDamage(float obstacleY);
		bool consumeFuel(float deltatime);
		void restoreFuel() { fuel += 5; fuel = std::fmin(fuel, 100); }


		glm::vec3 getPosition() { return position; }
		float getSize() { return size; }
		float getHitbox() { return hitbox; }
		glm::mat4 getModelMatrix() { return modelMatrix; }
		int getLives() { return lives; }
		float getFuel() { return fuel; }

	private:
		void kill();
};

