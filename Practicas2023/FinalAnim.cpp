/*---------------------------------------------------------*/
/* ----------------------  Proyecto ----------------------*/

#include <Windows.h>
#include <glad/glad.h>
#include <glfw3.h>	
#include <stdlib.h>		
#include <glm/glm.hpp>	
#include <glm/gtc/matrix_transform.hpp>	
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <shader_m.h>
#include <camera.h> 
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

#include <fstream>
#include <vector>
#include <sstream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

/* Ajustes */
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

/* Camara */
Camera camera(glm::vec3(0.0f, 2.0f, 0.0f));
float MovementSpeed = 0.2f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

/* Timing */
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

/* Lighting */
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);//Variable de la fuente de iluminación
glm::vec3 myPosition2(80.0f, 4.0f, 0.0f);//Variable de la fuente de iluminación
glm::vec3 lightColor(0.0f, 0.0f, 0.0f);

float myVariable = 0.0f;


/* Posiciones */
float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		movAuto_y = 0.0f,
		orienta = 0.0f;
bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;


//Keyframes (Manipulación y dibujo)
float	posXPin = 0.0f,
		posYPin = 0.0f,
		posZPin = 0.0f;

float   cuerpoPin = 0.0f,
		pataDDPin = 0.0f,
		pataDIPin = 0.0f,
		cabezaPin = 0.0f;

float	incXPin = 0.0f,
		incYPin = 0.0f,
		incZPin = 0.0f,
		incCuerpoPin = 0.0f,
		incCabezaPin = 0.0f,
		incPataDDPin = 0.0f,
		incPataDIPin = 0.0f;


/////LAMA/////
float	posXllama = 60.0f,
		posYllama = 0.0f,
		posZllama = -100.0f;
	

float movLlama_x = 0.0f;
float movLlama_z = 0.0f;
float movLlama_xinc = 0.0f;
float movLlama_zinc = 0.0f;
float giroLlama = -45.0f;
float giroLlamainc =0.0f;

//
int flag = 1;
int flag2 = 0;

#define MAX_FRAMES 10
int i_max_steps = 100;
int i_curr_steps = 10;
typedef struct _frame
{
	////Pinguino
	//float posXPin;
	//float posYPin;
	//float posZPin;
	//float cuerpoPin;
	//float pataDDPin;
	//float pataDIPin;
	//float cabezaPin;
	//
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;

	//KeyFrame para llama
	float movLlama_x;
	float movLlama_z;
	float movLlama_xinc;
	float movLlama_zinc;
	float giroLlama;
	float giroLlamainc;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 10;			//introducir número en caso de tener Key guardados
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	//KeyFrame[FrameIndex].posXPin = posXPin;
	//KeyFrame[FrameIndex].posYPin = posYPin;
	//KeyFrame[FrameIndex].posZPin = posZPin;
	//KeyFrame[FrameIndex].cuerpoPin = cuerpoPin;
	//KeyFrame[FrameIndex].pataDDPin = pataDDPin;
	//KeyFrame[FrameIndex].pataDIPin = pataDIPin;
	//KeyFrame[FrameIndex].cabezaPin = cabezaPin;


	/////LAMA/////
	KeyFrame[FrameIndex].movLlama_x = movLlama_x;
	KeyFrame[FrameIndex].movLlama_z = movLlama_z;
	KeyFrame[FrameIndex].giroLlama = giroLlama;

	FrameIndex++;
}

void resetElements(void)
{
	//posXPin = KeyFrame[0].posXPin;
	//posYPin = KeyFrame[0].posYPin;
	//posZPin = KeyFrame[0].posZPin;
	//cuerpoPin = KeyFrame[0].cuerpoPin;
	//pataDDPin = KeyFrame[0].pataDDPin;
	//pataDIPin = KeyFrame[0].pataDIPin;
	//cabezaPin = KeyFrame[0].cabezaPin;

	/////LAMA/////
	movLlama_x = KeyFrame[0].movLlama_x;
	movLlama_z = KeyFrame[0].movLlama_z;
	giroLlama = KeyFrame[0].giroLlama;
}

void interpolation(void)
{
	////Pinguino
	//incXPin = (KeyFrame[playIndex + 1].posXPin - KeyFrame[playIndex].posXPin) / i_max_steps;
	//incYPin = (KeyFrame[playIndex + 1].posYPin - KeyFrame[playIndex].posYPin) / i_max_steps;
	//incZPin = (KeyFrame[playIndex + 1].posZPin - KeyFrame[playIndex].posZPin) / i_max_steps;

	//incCuerpoPin = (KeyFrame[playIndex + 1].cuerpoPin - KeyFrame[playIndex].cuerpoPin) / i_max_steps;
	//incCabezaPin = (KeyFrame[playIndex + 1].cabezaPin - KeyFrame[playIndex].cabezaPin) / i_max_steps;
	//incPataDDPin = (KeyFrame[playIndex + 1].pataDDPin - KeyFrame[playIndex].pataDDPin) / i_max_steps;
	//incPataDIPin = (KeyFrame[playIndex + 1].pataDIPin - KeyFrame[playIndex].pataDIPin) / i_max_steps;
	////

	/////LAMA/////
	KeyFrame[playIndex].movLlama_xinc = (KeyFrame[playIndex + 1].movLlama_x - KeyFrame[playIndex].movLlama_x) / i_max_steps;
	KeyFrame[playIndex].movLlama_zinc = (KeyFrame[playIndex + 1].movLlama_z - KeyFrame[playIndex].movLlama_z) / i_max_steps;
	KeyFrame[playIndex].giroLlamainc = (KeyFrame[playIndex + 1].giroLlama - KeyFrame[playIndex].giroLlama) / i_max_steps;

}


void animate(void)
{
	myPosition2.x = 200.0f * cos(myVariable);
	myPosition2.z = 200.0f * sin(myVariable);
	
	myVariable += 2.0f;

	




	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			////Pinguino
			//posXPin += incXPin;
			//posYPin += incYPin;
			//posZPin += incZPin;
			//cabezaPin += incCabezaPin;
			//cuerpoPin += incCuerpoPin;
			//pataDIPin += incPataDIPin;
			//pataDDPin += incPataDDPin;
			////

			///LLAMA//
			movLlama_x += KeyFrame[playIndex].movLlama_xinc;
			movAuto_z += KeyFrame[playIndex].movLlama_zinc;
			giroLlama += KeyFrame[playIndex].giroLlamainc;

			i_curr_steps++;
		}
	}
	
}


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	/* glfw: inicializar y configurar */
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* Creación de ventana glfw */
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC. Zoológico virtual: Alea Iacta Est", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	/* Dile a GLFW que capture nuestro ratón */
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	/* Glad: cargar todos los punteros de función OpenGL */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/* Configurar el estado global de opengl */
	glEnable(GL_DEPTH_TEST);

	/* Construye y compila shaders */
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	/* Configuración del shader */
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	/* Modelos a cargar */
	Model piso("resources/objects/piso/FWF.obj");
	Model subpiso("resources/objects/piso/FFP2.obj");
	Model muro("resources/objects/muro/wall_1.obj");
	Model oso("resources/objects/habitats/bear.obj");
	Model treeOsos("resources/objects/tree/yamaboushi_tan_6000_a_spr1.obj");
	Model treeZebra("resources/objects/tree/Arbol7.obj");
	Model zebra("resources/objects/habitats/zebra.obj");
	Model pin("resources/objects/habitats/Pingu.obj");
	Model llama("resources/objects/habitats/llama.obj");
	Model llamaTr("resources/objects/tree/Arbol4.obj");

	Model llamaC("resources/objects/llama/llama.obj");

	//Model PinguinoCA("resources/objects/Animals2/Pinguino/CabezaPinguino1.obj");
	//Model PinguinoPD("resources/objects/Animals2/Pinguino/PinguinoPatas1.obj");
	//Model PinguinoPI("resources/objects/Animals2/Pinguino/PinguinoPatas2.obj");
	//Model PinguinoCU("resources/objects/Animals2/Pinguino/TorsoPinguino1.obj");

	//Inicialización de KeyFrames
	std::ifstream archivo("AnimacionPin.txt");

	if (archivo.is_open()) {
		std::vector<FRAME> keyFrames;
		std::string linea;

		while (std::getline(archivo, linea)) {
			//std::istringstream iss(linea);
			//FRAME keyFrame;
			//iss >> keyFrame.posXPin >> keyFrame.posYPin >> keyFrame.posZPin >> keyFrame.cuerpoPin >> keyFrame.cabezaPin >> keyFrame.pataDDPin >> keyFrame.pataDIPin;
			//keyFrames.push_back(keyFrame);
		}

		archivo.close();

		for (const FRAME& keyFrame : keyFrames) {
			//std::cout << "posX: " << keyFrame.posXPin
			//	<< ", posY: " << keyFrame.posYPin
			//	<< ", posZ: " << keyFrame.posZPin
			//	<< ", Cuerpo: " << keyFrame.cuerpoPin
			//	<< ", Cabeza: " << keyFrame.cabezaPin
			//	<< ", Pata1: " << keyFrame.pataDDPin
			//	<< ", Pata2: " << keyFrame.pataDIPin << std::endl;
			//std::cout << "-------------------------------------" << std::endl;
		}

		// Realiza la animación con los keyFrames leídos
		int i = 0;
		for (const FRAME& keyFrame : keyFrames) {
			//KeyFrame[i].posXPin = keyFrame.posXPin;
			//KeyFrame[i].posYPin = keyFrame.posYPin;
			//KeyFrame[i].posZPin = keyFrame.posZPin;

			//KeyFrame[i].cuerpoPin = keyFrame.cuerpoPin;
			//KeyFrame[i].cabezaPin = keyFrame.cabezaPin;
			//KeyFrame[i].pataDDPin = keyFrame.pataDDPin;
			//KeyFrame[i].pataDIPin = keyFrame.pataDIPin;
			i++;
		}
	}
	else {
		std::cout << "No se pudo abrir el archivo." << std::endl;
	}

	//Inicialización de KeyFrames
	/*for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}*/


	/////LLAMA/////
	KeyFrame[0].movLlama_x = -25.0f;
	KeyFrame[0].movLlama_z = 20.0f;
	KeyFrame[0].giroLlama = -45.0f;

	KeyFrame[1].movLlama_x = -25.0f;
	KeyFrame[1].movLlama_z = 20.0f;
	KeyFrame[1].giroLlama = -45.0f;

	KeyFrame[2].movLlama_x = -25.0f;
	KeyFrame[2].movLlama_z = 20.0f;
	KeyFrame[2].giroLlama = -45.0f;

	KeyFrame[3].movLlama_x = -25.0f;
	KeyFrame[3].movLlama_z = 20.0f;
	KeyFrame[3].giroLlama = -45.0f;

	KeyFrame[4].movLlama_x = -25.0f;
	KeyFrame[4].movLlama_z = 20.0f;
	KeyFrame[4].giroLlama = -45.0f;

	KeyFrame[5].movLlama_x = -25.0f;
	KeyFrame[5].movLlama_z = 20.0f;
	KeyFrame[5].giroLlama = -45.0f;

	KeyFrame[6].movLlama_x = -25.0f;
	KeyFrame[6].movLlama_z = 20.0f;
	KeyFrame[6].giroLlama = -45.0f;

	/* Loop render */
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		/* per - frame time logic */
		lastFrame = SDL_GetTicks();

		/* Input */
		animate();

		/* Render */
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* No olvide habilitar el sombreador antes de configurar los uniformes */
		staticShader.use();
		/* Configurar luces avanzadas */
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 1.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 1.00032f);

		/* Naturalmente podemos poner tantas fuentes de luz como queramos. */
		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(lightColor.x, lightColor.y, lightColor.z));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 1.0032f);

		staticShader.setVec3("pointLight[2].position", myPosition2);
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(0.0f, 0.0f, 0.2f));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 1.0f);
		staticShader.setFloat("pointLight[2].linear", 0.009f);
		staticShader.setFloat("pointLight[2].quadratic", 1.0000032f);

		/* Fuente de luz soptlight o de reflector. */
		staticShader.setVec3("spotLight[0].position", glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(camera.Front.x, camera.Front.y, camera.Front.z));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(20.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 1.0f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp1 = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		/* Luz */
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		

		/* Personaje animación */
		/* Recuerda activar el shader con la animación. */
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
		
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);


		
		/* Escenario */
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		
		////////////////////////////////////////////////////////////////////////////////////
		/* NOTA: Si van a moverle algo a la escala, intenten moverle la escala a todos los objetos*/
		/* Hábitats */
		//Osos
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		oso.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(61.0f, 0.0f, 30.0f));
		model = glm::scale(model, glm::vec3(0.009f));
		staticShader.setMat4("model", model);
		treeOsos.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(60.0f, 0.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.007f));
		staticShader.setMat4("model", model);
		treeOsos.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(57.0f, 0.0f, 5.0f));
		model = glm::scale(model, glm::vec3(0.009f));
		staticShader.setMat4("model", model);
		treeOsos.Draw(staticShader);
		//Cebras

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-70.0f, 0.0f, -90.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		treeZebra.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-80.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		staticShader.setMat4("model", model);
		treeZebra.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-80.0f, 0.0f, -47.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		treeZebra.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, -55.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		treeZebra.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		zebra.Draw(staticShader);
		//Pingüinos
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		pin.Draw(staticShader);
		
		//Llamas		

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, -90.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		staticShader.setMat4("model", model);
		llamaTr.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(15.0f, 0.0f, -47.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		staticShader.setMat4("model", model);
		llamaTr.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, -52.0f));
		model = glm::scale(model, glm::vec3(3.05f));
		staticShader.setMat4("model", model);
		llamaTr.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		llama.Draw(staticShader);

		/////////////////////////////////////LLAMA//////////////////////////////////////////
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(posXllama + movLlama_x, posYllama, posZllama + movLlama_z));
		model = glm::rotate(model, glm::radians(giroLlama), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		llamaC.Draw(staticShader);
		////////////////////////////////////////////////////////////////////////////////////


		//model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(posXPin, posYPin, posZPin));
		//tmp1 = model = glm::rotate(model, glm::radians(cuerpoPin), glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//PinguinoCU.Draw(staticShader);

		//model = glm::translate(tmp1, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(cabezaPin), glm::vec3(1.0f, 0.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//PinguinoCA.Draw(staticShader);

		//model = glm::translate(tmp1, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-pataDDPin), glm::vec3(1.0f, 0.0f, 1.0f));
		//staticShader.setMat4("model", model);
		//PinguinoPD.Draw(staticShader);

		//model = glm::translate(tmp1, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(pataDIPin), glm::vec3(0.0f, 0.0f, 1.0f));
		//staticShader.setMat4("model", model);
		//PinguinoPI.Draw(staticShader);
		////////////////////////////////////////////////////////////////////////////////////

		/* Elementos básicos del zoológico. */
		//Muros.
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		muro.Draw(staticShader);
		
		//Subzona de pinguinos
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		subpiso.Draw(staticShader);

		//Piso del ambiente.
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);
		
		/* Dibujar skybox como último */
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		/* Limitar el framerate a 60 */
		deltaTime = SDL_GetTicks() - lastFrame; 
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		/* glfw: intercambiar búferes y sondear eventos IO (teclas presionadas/soltadas, mouse movido, etc.)  */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

/* procesar todas las entradas: consultar GLFW si las teclas 
relevantes se presionaron/soltaron en este cuadro y reaccionó en consecuencia */
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	/* Movimiento de camara */
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		cuerpoPin++;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		cabezaPin++;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		pataDDPin++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		pataDIPin++;
	
	/* Para reproducir la animación KeyFrame */
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Reproducir animación" << std::endl;
			resetElements();
			/* Primer interpolacion */
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "No hay suficientes fotogramas clave" << std::endl;
		}
	}

	/* Para guardar un fotograma clave */
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

/* glfw: cada vez que cambia el tamaño de la ventana (por el sistema operativo o el cambio de tamaño del usuario), 
esta función de devolución de llamada se ejecuta */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	/* asegúrese de que la ventana gráfica coincida con las dimensiones de la nueva ventana; 
	Tenga en cuenta que el ancho y la altura será significativamente mayor que la especificada en las pantallas retina. */
	glViewport(0, 0, width, height);
}

/* glfw: cada vez que el mouse se mueve, esta devolución de llamada se llama */
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; /* invertido ya que las coordenadas y van de abajo hacia arriba */

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

/* glfw: cada vez que la rueda de desplazamiento del mouse se desplaza, esta devolución de llamada se llama */
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}