#pragma once
#include "Model.h"
#include "Pear.h"
#include "Logging.h"
#include "Camera.h"
#include "BenchmarkTimer.h"
#define __BENCHMARK_RENDERLOOP__
//#define __GAMES_FOLDER_DEVELOPMENT__ "D:/Workspace/PeartreeEngine/branches/BaseGame"
#include <fstream>
#include <glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <future>

//PearMath

void CB_FrameBufferSize(GLFWwindow* window, int width, int height);
void CB_MouseCursor(GLFWwindow* window, double xpos, double ypos);
void CB_MouseScroll(GLFWwindow* window, double xoffset, double yoffset);
void CB_MouseButton(GLFWwindow* window, int button, int action, int mods);
void ProcessInput(GLFWwindow* window);
void InitPears();

std::vector<Pear*> gameObjects;
PearTree::Model* jeep;

std::vector<std::future<void>> m_futures;
std::string m_GamesFolder;

bool lookAroundFlag = false;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;

struct PearSpace
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
} pearSpace;

int main(unsigned int argc, const char** argv)
{
#ifdef __GAMES_FOLDER_DEVELOPMENT__
	m_GamesFolder = __GAMES_FOLDER_DEVELOPMENT__;
#else
	if (argc > 1)
	{
		LOG(argv[0]);
		m_GamesFolder = argv[1];
		if (m_GamesFolder == "")
			m_GamesFolder = "../BaseGame/"; //Default folder if we navigate from the Core's root folder.
	}
	else
		m_GamesFolder = "../BaseGame/"; //Default folder if we navigate from the Core's root folder.
#endif

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef APPLE_LOL	
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(800, 600, "Peartree 0.1", NULL, NULL);
	if (window == NULL)
	{
		LOG("Failed creating GLFW window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG("Failed to initialize GLAD");
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	
	LOG(glGetString(GL_VERSION));

	glfwSetFramebufferSizeCallback(window, CB_FrameBufferSize);
	glfwSetCursorPosCallback(window, CB_MouseCursor);
	glfwSetScrollCallback(window, CB_MouseScroll);
	glfwSetMouseButtonCallback(window, CB_MouseButton);
	stbi_set_flip_vertically_on_load(true);

	//Creates our Game Objects that we will be rendering
	InitPears();

	jeep = new PearTree::Model(m_GamesFolder + "Media/Models/jeep/jeep.obj");

	const std::string vertexShaderFile = m_GamesFolder + "Media/Shaders/ModelVertexShader.glsl";
	const std::string fragmentShaderFile = m_GamesFolder + "Media/Shaders/ModelFragmentShader.glsl";

	unsigned int tex = PearTree::Model::TextureFromFile("Media/Models/jeep/texture.jpg", m_GamesFolder, false);

	Shader jeepShader(vertexShaderFile, fragmentShaderFile);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view = glm::mat4(1.0f);
	// note that we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	pearSpace.model = model;
	pearSpace.view = view;
	pearSpace.projection = projection;
	
	std::ofstream outfile("outputfile.txt", std::ios::out);

	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef __BENCHMARK_RENDERLOOP__
		BenchmarkTimer bt("RenderLoop", std::cout);
#endif

		/*pearSpace.projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
		pearSpace.view = camera.GetViewMatrix();*/

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		jeepShader.setMat4("projection", projection);
		jeepShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		jeepShader.setMat4("model", model);

		pearSpace.projection = projection;
		pearSpace.view = view;
		pearSpace.model = model;

		//Render our Pears.
		std::vector<Pear*>::iterator goIt = gameObjects.begin();
		while( goIt != gameObjects.end())
		{			
			(*goIt)->Draw(pearSpace.model, pearSpace.view, pearSpace.projection);
			goIt++;
		}
		
		//Render our Jeep.
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex);

			jeepShader.Use();
			jeep->Draw(jeepShader);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		glfwSwapBuffers(window);
	}

	//Clean-up all our Game Objects
	std::vector<Pear*>::iterator goIt = gameObjects.begin();
	while (goIt != gameObjects.end())
	{
		delete (*goIt);
		(*goIt) = nullptr;
		goIt++;
	}

	glfwTerminate();
	LOG("===== SUCCESSFULLY SHUTDOWN SYSTEM =====");
	return 0;
}

void CB_FrameBufferSize(GLFWwindow* window, int width, int height)
{
	LOG("===== New Viewport WIDTH : " + std::to_string(width));
	LOG("===== New Viewport HEIGHT : " + std::to_string(height));
	glViewport(0, 0, width, height);
}

void CB_MouseCursor(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);
		firstMouse = false;
	}

	float xoffset = static_cast<float>(xpos) - lastX;
	float yoffset = lastY - static_cast<float>(ypos); // reversed since y-coordinates go from bottom to top

	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);

	if (!lookAroundFlag)
		return;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void CB_MouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void CB_MouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		lookAroundFlag = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		lookAroundFlag = false;
	}
}


void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		LOG("===== WIREFRAME MODE:::ON");
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		LOG("===== WIREFRAME MODE:::OFF");
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		glEnable(GL_MULTISAMPLE);
		LOG("===== ANTIALIASING:::ON");
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		glDisable(GL_MULTISAMPLE);
		LOG("===== ANTIALIASING:::OFF");
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, 0.1f);
		LOG("===== MOVING CAMERA:::FORWARD");
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, 0.1f);
		LOG("===== MOVING CAMERA:::LEFT");
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, 0.1f);
		LOG("===== MOVING CAMERA:::BACKWARD");
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, 0.1f);
		LOG("===== MOVING CAMERA:::RIGHT");
	}
}	

//static std::mutex s_GameObjectMutex;
void InitPear(std::vector<Pear*>& _gameObjects,
	std::vector<float>& _vertices,
	std::vector<int>& _indices,
	std::vector<unsigned int> _textureIDs,
	const std::string& _vertexShaderFile,
	const std::string& _fragmentShaderFile)
{
	//std::lock_guard<std::mutex> lock(s_GameObjectMutex);
	_gameObjects.push_back(new Pear(_vertices, _indices, _textureIDs, _vertexShaderFile, _fragmentShaderFile));
};

void InitPears()
{
	//Pear 1
	const std::string vertexShaderFile = m_GamesFolder + "Media/Shaders/BaseVertexShader.glsl";
	const std::string fragmentShaderFile = m_GamesFolder + "Media/Shaders/BaseFragmentShader.glsl";

	std::vector<int> indices = std::vector<int>();
	std::vector<int> indices2 =
	{
		0, 1, 3,
		1, 2, 3
	};

	std::vector<float> vertices2 = 
	{
		 // positions         // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	std::vector<float> vertices =
	{
		// positions          // colors          // texture coords
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
							  
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
							  
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
							  
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
							  
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
							  
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = 
	{
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	unsigned int tex = PearTree::Model::TextureFromFile("Media/Graphics/Mario.jpg", m_GamesFolder, false);
	unsigned int tex2 = PearTree::Model::TextureFromFile("Media/Graphics/HappyDog.png", m_GamesFolder, false);
	std::vector<unsigned int> textureIDs = { tex , tex2 };
	unsigned int MAX_NUM_GAME_OBJECTS = 10;
	unsigned int count = 0;
	{
		//BenchmarkTimer bt("InitPears 500 Reserved Pears", std::cout);
		while (MAX_NUM_GAME_OBJECTS != count)
		{
			//m_futures.push_back(std::async(std::launch::async, InitPear, gameObjects, vertices, indices, tex.ID, vertexShaderFile, fragmentShaderFile));
			InitPear(gameObjects, vertices, indices, textureIDs, vertexShaderFile, fragmentShaderFile);
			gameObjects.at(count)->SetXPos(cubePositions[count].r);
			gameObjects.at(count)->SetYPos(cubePositions[count].g);
			gameObjects.at(count)->SetZPos(cubePositions[count].b);
			count++;
		}

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				InitPear(gameObjects, vertices2, indices2, textureIDs, vertexShaderFile, fragmentShaderFile);
				gameObjects.at(count)->SetXPos(static_cast<float>(i));
				gameObjects.at(count)->SetYPos(static_cast<float>(j));
				count++;
			}
		}
	}
}

void InitPlayer();
void InitPlayer()
{
}


