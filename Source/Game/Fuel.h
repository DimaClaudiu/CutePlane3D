#pragma once
#include <Component/Transform/Transform.h>
#include <vector>

class Fuel
{
	private:
		int count;
		std::vector<glm::vec2> positions;
		float scale;
		float hitbox;
		float rotation;
		int maxLen;
		glm::vec3 color;
		glm::mat4 modelMatrix;

	public:
		Fuel();
		~Fuel();

		int generateFuelStrip(int maxLen);

		float getScale() { return scale; }
		int getCount() { return count; }
		float getHitbox() { return hitbox; }
		glm::vec3 getColor() { return color; }
		glm::mat4 getModelMatrix() { return modelMatrix; }
		
		glm::mat4 move(float deltatime, int pos);

		bool checkCollision(glm::vec2 planePos, float planeHitbox, int pos);


};

