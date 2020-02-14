#pragma once
#include <Component/Transform/Transform.h>
#include <vector>

class Obstacles
{
	private:
		int count;
		std::vector<glm::vec2> positions;
		float scale;
		float speed;
		float hitbox;
		float rotation;
		glm::vec3 color;
		glm::mat4 modelMatrix;

	public:
		Obstacles();
		~Obstacles();

		float getScale() { return scale; }
		int getCount() { return count; }
		float getHitbox() { return hitbox; }
		glm::vec2 getPosition(int pos) { return positions[pos]; }
		glm::vec3 getColor() { return color; }
		glm::mat4 getModelMatrix() { return modelMatrix; }
		
		glm::mat4 move(float deltatime, int pos);
		bool checkCollision(glm::vec2 planePos, float planeHitbox, int pos);

};

