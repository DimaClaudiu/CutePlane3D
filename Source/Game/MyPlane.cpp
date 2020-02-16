#include "MyPlane.h"

#include <vector>
#include <string>
#include <iostream>
#include <time.h>

#include <Core/Engine.h>
#include "Object2D.h"
#include "Transform2D.h"

using namespace std;

MyPlane::MyPlane()
{
	this->resolution = window->GetResolution();
	this->aspectRatio = glm::vec2(16, 9); // sorry ultrawide
}

MyPlane::~MyPlane()
{
}

bool gameStart;
float mytime = 0;
int sign = 1;
float myrotation = 0;
glm::vec2 mousePos;
vector<glm::vec3> clouds;

glm::vec3 generateCloud()
{
	return glm::vec3((float)(rand() % 4000 + 4000) / 100, 
		(float)(rand() % 50 + 200) / 100, 
		-(float)(rand() % 1000 + 400) / 100);
}

void MyPlane::initializeWorld(bool resetCall)
{
	gameStart = false;
	srand(time(NULL));
	plane = Plane();
	obstacles = Obstacles();
	fuel = Fuel();
	hud = HUD();

	if (!resetCall)
	{
		water = Water();
		clouds.clear();
		for (int i = 0; i < 12; i++)
			clouds.push_back(generateCloud());
	}
}

void MyPlane::Init()
{
	GetSceneCamera()->SetPosition(glm::vec3(8.04945, 4.54662, 7.90402));
	initializeWorld(false);
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* square = Object2D::CreateSquare(("square"), glm::vec3(2.5f, 7.5f, 0), 2, glm::vec3(0, 0, 1), true);
		AddMeshToList(square);
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("water");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "World", "water.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("icosahedron");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "icosahedron.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("octahedron");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "octahedron.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("cloud");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "World", "cloud.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("plane");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "World", "plane.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("heart");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "World", "heart.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("cross");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "World", "cross.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		vector<VertexFormat> vertices1
		{
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(0, 1, 0), glm::vec3(1, 0, 1)),
			VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)),
			VertexFormat(glm::vec3(0, 0, 1), glm::vec3(1, 0, 0)),
		};

		vector<unsigned short> indices1 =
		{
			0, 2, 1,
			1, 3, 2,
			0, 1, 3,
			2, 3, 1,
		};

		meshes["tetrahedron"] = new Mesh("generated tetrahedron");
		meshes["tetrahedron"]->InitFromData(vertices1, indices1);
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("ShaderOscilate");
		shader->AddShader("Source/Game/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Game/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("ShaderLight");
		shader->AddShader("Source/Game/Shaders/VertexShader2.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Game/Shaders/FragmentShader2.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("ShaderPlane");
		shader->AddShader("Source/Game/Shaders/VertexShader3.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Game/Shaders/FragmentShader3.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//Light & material properties
	{
		lightPosition = glm::vec3(8, 6, 4);
		materialShininess = 20;
		materialKd = 3;
		materialKs = 5;
	}
}

void MyPlane::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);	
}

void MyPlane::Update(float deltaTimeSeconds)
{
	// Move and draw water
	for (int i = 0; i < water.getSegCount(); i++)
		RenderSimpleMesh(meshes["water"], shaders["ShaderOscilate"], water.move(deltaTimeSeconds, i), water.getColor());

	if (gameStart) {
		// Move and draw obstacles
		for (int i = 0; i < obstacles.getCount(); i++) {
			if (obstacles.checkCollision(plane.getPosition(), plane.getHitbox(), i) && plane.getLives() > 0)
				plane.takeDamage(obstacles.getPosition(i).y);
			RenderSimpleMesh(meshes["icosahedron"], shaders["ShaderOscilate"], obstacles.move(deltaTimeSeconds, i), obstacles.getColor());
		}

		// Move and draw fuel strip
		int constCount = fuel.getCount();
		for (int i = 0; i < constCount; i++)
		{
			if (fuel.checkCollision(plane.getPosition(), plane.getHitbox(), i))
				plane.restoreFuel();
			RenderSimpleMesh(meshes["octahedron"], shaders["ShaderPlane"], fuel.move(deltaTimeSeconds, i), fuel.getColor());

		}
	}

	// Move and draw plane
	{
		RenderSimpleMesh(meshes["plane"], shaders["ShaderPlane"], plane.move(deltaTimeSeconds, gameStart), glm::vec3(1.0f, 0.0f, 0.0f));
		RenderSimpleMesh(meshes["box"], shaders["ShaderPlane"], plane.getBumperPos(), glm::vec3(255, 255, 255));
		RenderSimpleMesh(meshes["cross"], shaders["ShaderPlane"], plane.getCrossPos(), glm::vec3(0, 0, 0));
		RenderSimpleMesh(meshes["cross"], shaders["ShaderPlane"], plane.getWingCrossPos(), glm::vec3(0, 0, 0));

		if (plane.getPosition().y < 0.5)
			initializeWorld(true);
	}

	// Move and draw coulds
	for (int i = 0; i < clouds.size(); i++)
	{
		clouds[i].x -= deltaTimeSeconds * 2;
		if (clouds[i].x < -20)
			clouds[i] = generateCloud();

		if (clouds[i].x <= 30)
			clouds[i].y = sin(clouds[i].x * AI_MATH_PI / 20.0f) + 7.0f;

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, clouds[i]);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));
		RenderSimpleMesh(meshes["cloud"], shaders["ShaderOscilate"], modelMatrix, glm::vec3(1, 1, 1));

	}


	// Render the point light in the scene
	{	

		if (mytime >= 1)
			sign = -1;
		else if (mytime <= 0)
			sign = 1;

		mytime += sign * 0.2f * deltaTimeSeconds;
		
		// Render the light source position
		/*glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, lightPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));
		RenderSimpleMesh(meshes["sphere"], shaders["VertexColor"], modelMatrix, glm::vec3(1, 0, 0));*/
	}

	// HUD
	{
		RenderTexturedMesh(meshes["box"], shaders["Simple"], hud.getLifeBg(plane.getLives()), 3);
		RenderSimpleMesh(meshes["heart"], shaders["ShaderPlane"], hud.getHeartPos(), glm::vec3(10, 0, 0));
		
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(glm::vec2(2.5f, 7.5f), 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0, 0.4, 0.001f));
		RenderSimpleMesh(meshes["box"], shaders["ShaderPlane"], modelMatrix, glm::vec3(0, 0, 0));


		RenderTexturedMesh(meshes["box"], shaders["Simple"], hud.getFuelBg(plane.getFuel()), 4);
		RenderSimpleMesh(meshes["octahedron"], shaders["ShaderPlane"], hud.getFuelPos(), fuel.getColor());

		
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(glm::vec2(2.5f, 7.0f), 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0, 0.4, 0.001f));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLight"], modelMatrix, glm::vec3(0, 0, 0));
	}

	// Rotate and draw the skybox
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(8, 0, 0));
		myrotation += deltaTimeSeconds / 2.0f;
		modelMatrix = glm::rotate(modelMatrix, myrotation, glm::vec3(1, 1, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(80.0f));
		RenderTexturedMesh(meshes["sphere"], shaders["Simple"], modelMatrix, 2);
	}
}

void MyPlane::FrameEnd()
{
	//DrawCoordinatSystem();
}

void MyPlane::RenderTexturedMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int textureID)
{
	if (!mesh || !shader || !shader->program)
		return;

	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render(textureID);
}

void MyPlane::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// Set light position uniform
	GLint loc_light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(loc_light_position, 1, glm::value_ptr(lightPosition));

	// Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();

	GLint loc_eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(loc_eye_position, 1, glm::value_ptr(eyePosition));

	// Set material property uniforms (shininess, kd, ks, object color) 
	GLint loc = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(loc, materialShininess);

	loc = glGetUniformLocation(shader->program, "material_kd");  // Difuse
	glUniform1f(loc, materialKd);

	loc = glGetUniformLocation(shader->program, "material_ks");  // Specular
	glUniform1f(loc, materialKs);

	loc = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(loc, 1, glm::value_ptr(color));

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	
	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Add time for a pseudo-random effect
	int locationTime = glGetUniformLocation(shader->program, "Time");
	glUniform1f(locationTime, mytime);
	
	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void MyPlane::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 2;

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

		// Control light position using on W, A, S, D, E, Q
		if (window->KeyHold(GLFW_KEY_W)) lightPosition -= forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_A)) lightPosition -= right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_S)) lightPosition += forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_D)) lightPosition += right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_E)) lightPosition += up * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_Q)) lightPosition -= up * deltaTime * speed;
	}
	if (window->KeyHold(GLFW_KEY_1))
	{
		glm::vec3 cameraPos = GetSceneCamera()->transform->GetWorldPosition();
		cout << "x: " << lightPosition.x << " y: " << lightPosition.y << " z:  " << lightPosition.z << endl;
	}
}

void MyPlane::OnKeyPress(int key, int mods)
{
	if(key == GLFW_KEY_SPACE)
	{
		gameStart = !gameStart;
	}
}

void MyPlane::OnKeyRelease(int key, int mods)
{
}

void MyPlane::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	plane.followMouse(mouseX, mouseY, deltaX, deltaY);
	mousePos.x = (float)mouseX * 16.0f / 1920.0f;
	mousePos.y = 9.0f - (float)mouseY * 9.0f / 1080.0f;
}

void MyPlane::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void MyPlane::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void MyPlane::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void MyPlane::OnWindowResize(int width, int height)
{
}
