#ifdef _WIN32
#pragma once
#endif

/*These will enable essentially what they imply, logged to the console. */
//#define __BENCHMARK_RENDERLOOP__
//#define __DEBUG_CAMERA_POSITION__

#define SDL_MAIN_HANDLED // Tell SDL not to mess with main()
#include <unordered_map>
#include <fstream>
#include <future>
#include <deque>
#include <glad.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_opengl.h>
#include <stb_image.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "Pear.h"
#include "Logging.h"
#include "Camera.h"
#include "BenchmarkTimer.h"

//TODO: Implement Window Resizing CB for SDL
void OnWindowResize(SDL_Window* window, int width, int height);

void OnMouseMotion(SDL_Window* window, double xpos, double ypos);
void OnMouseScroll(SDL_Window* window, double xoffset, double yoffset);
void ProcessInput(SDL_Window* window);

static unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);

//Core Vars
std::string m_GamesFolder;
bool running = true;

// camera
Camera camera(glm::vec3(0.0f, 1.0f, 0.0f));
float lastX = 1200 / 2.0f;
float lastY = 800 / 2.0f;
bool lookAroundFlag = true;
bool firstMouse = true;

struct PearSpace
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	uint16_t width;
	uint16_t height;
} pearSpace;

/*############### MAIN ##################*/
int main(int argc, const char** argv)
{
#ifdef __GAMES_FOLDER_DEVELOPMENT__
	m_GamesFolder = __GAMES_FOLDER_DEVELOPMENT__;
#else
	if (argc > 1)
	{
		LOG(argv[0]);
		m_GamesFolder = argv[1];
		if (m_GamesFolder == "")
			m_GamesFolder = "../../../../BaseGame/"; //Default folder if we navigate from the Core's root build folder.
	}
	else
		m_GamesFolder = "../../../../BaseGame/"; //Default folder if we navigate from the Core's root build folder.
#endif

	m_GamesFolder = "../../../../BaseGame/";
	std::cout << m_GamesFolder << "\n";

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		LOG(SDL_GetError());
	}
	SDL_GL_LoadLibrary(NULL);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	SDL_SetRelativeMouseMode(SDL_TRUE);

	SDL_Window* window = SDL_CreateWindow("Peartree 0.5", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (window == NULL)
	{
		LOG("Failed creating SDL window");
		return -1;
	}
	SDL_GLContext maincontext = SDL_GL_CreateContext(window);
	if (maincontext == NULL)
	{
		LOG("Failed creating OpenGL context");
		return -1;
	}

	gladLoadGLLoader(SDL_GL_GetProcAddress);

	LOG(glGetString(GL_VENDOR));
	LOG(glGetString(GL_RENDERER));
	LOG(glGetString(GL_VERSION));

	// V-Sync ... Does this *need* to be called after window creation or can we set this with the other folks up there?
	SDL_GL_SetSwapInterval(1);

	//World-space vars
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 view = glm::mat4(1.0f);
	// note that we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	uint16_t gameWidth = 1200;
	uint16_t gameHeight = 800;

	pearSpace.model = model;
	pearSpace.view = view;
	pearSpace.projection = projection;
	pearSpace.width = gameWidth;
	pearSpace.height = gameHeight;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glViewport(0, 0, pearSpace.width, pearSpace.height);
	stbi_set_flip_vertically_on_load(true);

	//Shaders
	//We need a better way of handling these obviously.
	const std::string vertexShaderFile = "../Shaders/BaseVertexShader.glsl";
	const std::string fragmentShaderFile = "../Shaders/BaseFragmentShader.glsl";
	Shader* baseShader = new Shader(vertexShaderFile, fragmentShaderFile);
	
	while (running)
	{
#ifdef __BENCHMARK_RENDERLOOP__
		BenchmarkTimer bt("RenderLoop", std::cout);
#endif
		ProcessInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera.GetViewMatrix();
		pearSpace.view = view;
		
		// std::deque<Pear*>::iterator goIt = gameObjects.begin();
		// while( goIt != gameObjects.end() )
		// {		
		// 	(*goIt)->Draw(baseShader, pearSpace.model, pearSpace.view, pearSpace.projection);
		// 	goIt++;
		// }
		
		SDL_GL_SwapWindow(window);
	}

	// std::deque<Pear*>::iterator goIt = gameObjects.begin();
	// while (goIt != gameObjects.end())
	// {
	// 	delete (*goIt);
	// 	(*goIt) = nullptr;
	// 	goIt++;
	// }

	SDL_DestroyWindow(window);
	SDL_Quit();
	LOG("===== SUCCESSFULLY SHUTDOWN SYSTEM =====");
	return 0;
}

//TODO: Implement Window Resizing for SDL
void OnWindowResize(SDL_Window* window, int width, int height)
{
	LOG("===== New Viewport WIDTH : " + std::to_string(width));
	LOG("===== New Viewport HEIGHT : " + std::to_string(height));
	glViewport(0, 0, width, height);
}

void OnMouseMotion(SDL_Window* window, double xpos, double ypos)
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

void OnMouseScroll(SDL_Window* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void ProcessInput(SDL_Window* window)
{
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_W])
	{
		camera.ProcessKeyboard(Camera_Movement::FORWARD, 0.1f);
	}
	if (keys[SDL_SCANCODE_A])
	{
		camera.ProcessKeyboard(Camera_Movement::LEFT, 0.1f);
	}
	if (keys[SDL_SCANCODE_S])
	{
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, 0.1f);
	}
	if (keys[SDL_SCANCODE_D])
	{
		camera.ProcessKeyboard(Camera_Movement::RIGHT, 0.1f);
	}
	if (keys[SDL_SCANCODE_SPACE])
	{
		camera.Position.g += 0.1f;
	}

#ifdef __DEBUG_CAMERA_POSITION__
	LOG("===== NEW CAMERA POS ===== \n x : " << camera.Position.r << " y : " << camera.Position.g << " z : " << camera.Position.b);
#endif

	SDL_Event evt;
	while (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
		{
			switch (evt.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				running = false;
				break;
			case SDLK_1:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				LOG("===== WIREFRAME MODE:::ON");
				break;
			case SDLK_2:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				LOG("===== WIREFRAME MODE:::OFF");
				break;
			case SDLK_3:
				glEnable(GL_MULTISAMPLE);
				LOG("===== ANTIALIASING:::ON");
				break;
			case SDLK_4:
				glDisable(GL_MULTISAMPLE);
				LOG("===== ANTIALIASING:::OFF");
				break;
			}
		}
		break;
		// case SDL_MOUSEMOTION:
		// 	static int x, y = 0;
		// 	x += evt.motion.xrel;
		// 	y += evt.motion.yrel;
		// 	OnMouseMotion(window, x, y);
		// 	break;
		}
	}
}	

static unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);

	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_BLUE; //default.
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}


