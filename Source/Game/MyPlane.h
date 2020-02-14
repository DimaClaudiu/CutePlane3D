#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include "Plane.h"
#include "Water.h"
#include "Obstacles.h"
#include "Fuel.h"
#include "HUD.h"

class MyPlane : public SimpleScene
{
	private:
		glm::vec2 resolution;
		glm::vec2 aspectRatio;

		glm::vec3 lightPosition;
		unsigned int materialShininess;
		float materialKd;
		float materialKs;

		Plane plane;
		Water water;
		Obstacles obstacles;
		Fuel fuel;
		HUD hud;

	public:
		MyPlane();
		~MyPlane();	

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderTexturedMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int textureID);


		void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));
		void initializeWorld(bool resetCall);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

};
