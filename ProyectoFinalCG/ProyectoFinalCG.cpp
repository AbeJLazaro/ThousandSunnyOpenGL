/*
	Proyecto Final para la Materia de Computación Gráfica e Interacción Humano-Computadora
	Autores:		Lázaro Martínez Abraham Josué
					Ian
					Apolo
	Fecha:			15/07/2021
	Versión:		1

*/

#include <iostream>
#include <stdlib.h>

// GLAD: Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator
// https://glad.dav1d.de/
#include <glad/glad.h>

// GLFW: https://www.glfw.org/
#include <GLFW/glfw3.h>

// GLM: OpenGL Math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Clases para importar modelos
#include <shader_m.h>
#include <camera.h>
#include <model.h>
#include <Light.h>
#include <Material.h>

// Functions
bool Start();
bool Update();

// Definición de callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

unsigned int loadCubemap(vector<std::string> faces);

// Tamaño en pixeles de la ventana
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// Definición de cámara (posición en XYZ)
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

// Controladores para el movimiento del mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Variables para la velocidad de reproducción
// de la animación
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsedTime = 0.0f;

// Variables para el control del modelo
float modelRotation = 0.0f;
Light   light0;
Light	light1;

// Variables para las pruebas
float	linear = 0.09f;
float	quadratic = 0.032f;
int		Tshine = 5;
glm::vec3 difusse = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec4 amb = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
glm::vec3 LuzPosition = glm::vec3(0.0f, 0.0f, 0.0f);

// Variables globales utiles
GLFWwindow* window;
GLfloat  bgR, bgG, bgB, bgA;

// Shaders
Shader* materialSinTexturaShader;
Shader* fondoShader;
Shader* fresnelShader;

// Modelos
Model* Floor;
Model* MaderaPuerta;
Model* BarandalesYLaterales;
Model* Canons;
Model* MarcoOroPuerta;
Model* RejillaVentana;
Model* MarcoPuertaRojo;
Model* MarcoPuertaBlanco;
Model* CostadosRojos;
Model* FondoCanons;
Model* Fondo;
Model* cube; // sirve para visualizar la posición de las luces








// Ciclo main
int main(void){
	if (!Start())
		return -1;

	/* Ciclo hasta que se deba terminar la ventana */
	while (!glfwWindowShouldClose(window))
	{
		if (!Update())
			break;
	}

	glfwTerminate();
	return 0;
}

// Inicialización del sistema
bool Start() {
	// Inicialización de GLFW

	if (!glfwInit())
		return false;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creación de la ventana con GLFW
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Thousand Sunny", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Error al crear la ventana con GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Hacemos que el contexto sea sobre esta ventana
	glfwMakeContextCurrent(window);

	// Indicamos las funciones para los callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Ocultar el cursor mientras se rota la escena
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: Cargar todos los apuntadores
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Error al inicializar GLAD" << std::endl;
		return -1;
	}

	// Activación de buffer de profundidad
	glEnable(GL_DEPTH_TEST);

	// Inicialización de las variables de fondo
	bgR = bgG = bgB = bgA = 0.5f;

	// Compilación y enlace de shaders
	materialSinTexturaShader = new Shader("shaders/pruebas2.vs", "shaders/pruebas2.fs");
	if (materialSinTexturaShader == nullptr) {
		cout << "Error creating shaders." << endl;
		return false;
	}
	fondoShader = new Shader("shaders/pruebas2tex.vs", "shaders/pruebas2tex.fs");
	if (fondoShader == nullptr) {
		cout << "Error creating shaders." << endl;
		return false;
	}
	/*fresnelShader = new Shader("shaders/11_Fresnel.vs", "shaders/11_Fresnel.fs");
	if (fondoShader == nullptr) {
		cout << "Error creating shaders." << endl;
		return false;
	}*/

	// Carga la información de los modelos
	Floor = new Model("models/materials/floor.fbx");
	if (Floor == nullptr) {
		cout << "Error loading Floor Model." << endl;
		return false;
	}
	/*
	MaderaPuerta = new Model("models/Sunny/MaderaPuerta.fbx");
	if (MaderaPuerta == nullptr) {
		cout << "Error loading MaderaPuerta Model." << endl;
		return false;
	}
	BarandalesYLaterales = new Model("models/Sunny/BarandalesYLaterales.fbx");
	if (BarandalesYLaterales == nullptr) {
		cout << "Error loading BarandalesYLaterales Model." << endl;
		return false;
	}
	Canons = new Model("models/Sunny/Canons.fbx");
	if (Canons == nullptr) {
		cout << "Error loading Canons Model." << endl;
		return false;
	}
	MarcoOroPuerta = new Model("models/Sunny/MarcoOroPuerta.fbx");
	if (MarcoOroPuerta == nullptr) {
		cout << "Error loading MarcoOroPuerta Model." << endl;
		return false;
	}
	RejillaVentana = new Model("models/Sunny/RejillaVentana.fbx");
	if (RejillaVentana == nullptr) {
		cout << "Error loading RejillaVentana Model." << endl;
		return false;
	}
	MarcoPuertaRojo = new Model("models/Sunny/MarcoPuertaRojo.fbx");
	if (MarcoPuertaRojo == nullptr) {
		cout << "Error loading MarcoPuertaRojo Model." << endl;
		return false;
	}
	MarcoPuertaBlanco = new Model("models/Sunny/MarcoPuertaBlanco.fbx");
	if (MarcoPuertaBlanco == nullptr) {
		cout << "Error loading MarcoPuertaBlanco Model." << endl;
		return false;
	}
	CostadosRojos = new Model("models/Sunny/CostadosRojos.fbx");
	if (CostadosRojos == nullptr) {
		cout << "Error loading CostadosRojos Model." << endl;
		return false;
	}
	FondoCanons = new Model("models/Sunny/FondoCanons.fbx");
	if (FondoCanons == nullptr) {
		cout << "Error loading FondoCanons Model." << endl;
		return false;
	}
	*/
	Fondo = new Model ("models/Sunny/CubeMap.fbx");
	if (Fondo == nullptr) {
		cout << "Error loading Fondo Model." << endl;
		return false;
	}
	/*cube = new Model("models/materials/cube.fbx");
	if (cube == nullptr) {
		cout << "Error loading cube Model." << endl;
		return false;
	}*/

	return true;
}

// Dibujo de los modelos en la escena, transformaciones
bool Update() {

	// Cálculo del framerate
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	elapsedTime += deltaTime;

	// Procesa la entrada del teclado o mouse
	processInput(window);
	
	// Renderizado R - G - B - A
	glClearColor(bgR, bgG, bgB, bgA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Materiales sin textura
	{
		/*// Activamos el shader estático
		materialSinTexturaShader.use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		materialSinTexturaShader.setMat4("projection", projection);
		materialSinTexturaShader.setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Trasladar al centro de la escena
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// escala para los objetos que sean muy grandes
		materialSinTexturaShader.setMat4("model", model);

		// Propiedades de la luz

		// Luz direccional
		materialSinTexturaShader.setVec3("dirLight.direction", 0.0f, -1.0f, 0.0f);
		materialSinTexturaShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		materialSinTexturaShader.setVec3("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
		materialSinTexturaShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		materialSinTexturaShader.setVec3("dirLight.power", 5.0f, 5.0f, 5.0f);

		// Luz de linterna
		materialSinTexturaShader.setVec3("spotLight.position", camera.Position);
		materialSinTexturaShader.setVec3("spotLight.direction", camera.Front);
		materialSinTexturaShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		materialSinTexturaShader.setVec3("spotLight.diffuse", difusse);
		materialSinTexturaShader.setVec3("spotLight.specular", specular);
		materialSinTexturaShader.setFloat("spotLight.constant", 1.0f);
		materialSinTexturaShader.setFloat("spotLight.linear", 0.09);
		materialSinTexturaShader.setFloat("spotLight.quadratic", 0.032);
		materialSinTexturaShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		materialSinTexturaShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// point light 1
		materialSinTexturaShader.setVec3("pointLights[0].position", 38.4f,-0.6f,-22.9f);
		materialSinTexturaShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		materialSinTexturaShader.setVec3("pointLights[0].diffuse", 1.0f, 1.0f, 1.0f);
		materialSinTexturaShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		materialSinTexturaShader.setFloat("pointLights[0].constant", 1.0f);
		materialSinTexturaShader.setFloat("pointLights[0].linear", linear);
		materialSinTexturaShader.setFloat("pointLights[0].quadratic", quadratic);
		materialSinTexturaShader.setVec3("pointLights[0].power", 30.0f, 30.0f, 30.0f);

		// point light 2
		materialSinTexturaShader.setVec3("pointLights[1].position", -28.40f, -0.6f,-22.9f);
		materialSinTexturaShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		materialSinTexturaShader.setVec3("pointLights[1].diffuse", 1.0f, 1.0f, 1.0f);
		materialSinTexturaShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		materialSinTexturaShader.setFloat("pointLights[1].constant", 1.0f);
		materialSinTexturaShader.setFloat("pointLights[1].linear", linear);
		materialSinTexturaShader.setFloat("pointLights[1].quadratic", quadratic);
		materialSinTexturaShader.setVec3("pointLights[1].power", 30.0f, 30.0f, 30.0f);

		// point light 3
		materialSinTexturaShader.setVec3("pointLights[2].position", 0.0f, -0.6, 0.0f);
		materialSinTexturaShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		materialSinTexturaShader.setVec3("pointLights[2].diffuse", 1.0f, 1.0f, 1.0f);
		materialSinTexturaShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		materialSinTexturaShader.setFloat("pointLights[2].constant", 1.0f);
		materialSinTexturaShader.setFloat("pointLights[2].linear", linear);
		materialSinTexturaShader.setFloat("pointLights[2].quadratic", quadratic);
		materialSinTexturaShader.setVec3("pointLights[2].power", 30.0f, 30.0f, 30.0f);

		// point light 4
		materialSinTexturaShader.setVec3("pointLights[3].position", 0.0f,0.0f,-65.90f);
		materialSinTexturaShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		materialSinTexturaShader.setVec3("pointLights[3].diffuse", 1.0f, 1.0f, 1.0f);
		materialSinTexturaShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		materialSinTexturaShader.setFloat("pointLights[3].constant", 1.0f);
		materialSinTexturaShader.setFloat("pointLights[3].linear", linear);
		materialSinTexturaShader.setFloat("pointLights[3].quadratic", quadratic);
		materialSinTexturaShader.setVec3("pointLights[3].power", 30.0f, 30.0f, 30.0f);

		materialSinTexturaShader.setVec3("viewPos", camera.Position);

		// Habilitar blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		// Piso de prueba
		Material m1;
		materialSinTexturaShader.setVec4("MaterialAmbientColor", m1.ambient);
		materialSinTexturaShader.setVec4("MaterialDiffuseColor", m1.diffuse);
		materialSinTexturaShader.setVec4("MaterialSpecularColor", m1.specular);
		materialSinTexturaShader.setFloat("transparency", m1.transparency);
		materialSinTexturaShader.setInt("shininess", 10);
		Floor.Draw(materialSinTexturaShader);*/

		// Propiedades materiales del Barco
		/*
		// MaderaPuerta
		Material MMaderaPuerta;
		MMaderaPuerta.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		MMaderaPuerta.diffuse = glm::vec4(0.166f, 0.060f, 0.026f, 1.0f);
		MMaderaPuerta.specular = glm::vec4(0.166f, 0.060f, 0.026f, 1.0f);
		materialSinTexturaShader.setVec4("MaterialAmbientColor", MMaderaPuerta.ambient);
		materialSinTexturaShader.setVec4("MaterialDiffuseColor", MMaderaPuerta.diffuse);
		materialSinTexturaShader.setVec4("MaterialSpecularColor", MMaderaPuerta.specular);
		materialSinTexturaShader.setFloat("transparency", MMaderaPuerta.transparency);
		materialSinTexturaShader.setInt("shininess", 50);
		MaderaPuerta.Draw(materialSinTexturaShader);

		// BarandalesYLaterales
		Material MBarandalesYLaterales;
		MBarandalesYLaterales.ambient = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		MBarandalesYLaterales.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		MBarandalesYLaterales.specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		materialSinTexturaShader.setVec4("MaterialAmbientColor", MBarandalesYLaterales.ambient);
		materialSinTexturaShader.setVec4("MaterialDiffuseColor", MBarandalesYLaterales.diffuse);
		materialSinTexturaShader.setVec4("MaterialSpecularColor", MBarandalesYLaterales.specular);
		materialSinTexturaShader.setFloat("transparency", MBarandalesYLaterales.transparency);
		materialSinTexturaShader.setInt("shininess", 10);
		BarandalesYLaterales.Draw(materialSinTexturaShader);

		// Canons
		Material MCanons;
		MCanons.ambient = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		MCanons.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		MCanons.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		materialSinTexturaShader.setVec4("MaterialAmbientColor", MCanons.ambient);
		materialSinTexturaShader.setVec4("MaterialDiffuseColor", MCanons.diffuse);
		materialSinTexturaShader.setVec4("MaterialSpecularColor", MCanons.specular);
		materialSinTexturaShader.setFloat("transparency", MCanons.transparency);
		materialSinTexturaShader.setInt("shininess", 5);
		Canons.Draw(materialSinTexturaShader);

		// MarcoOroPuerta
		Material MMarcoOroPuerta;
		MMarcoOroPuerta.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		MMarcoOroPuerta.diffuse = glm::vec4(0.589f, 0.266f, 0.021f, 1.0f);
		MMarcoOroPuerta.specular = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
		materialSinTexturaShader.setVec4("MaterialAmbientColor", MMarcoOroPuerta.ambient);
		materialSinTexturaShader.setVec4("MaterialDiffuseColor", MMarcoOroPuerta.diffuse);
		materialSinTexturaShader.setVec4("MaterialSpecularColor", MMarcoOroPuerta.specular);
		materialSinTexturaShader.setFloat("transparency", MMarcoOroPuerta.transparency);
		materialSinTexturaShader.setInt("shininess", 2);
		MarcoOroPuerta.Draw(materialSinTexturaShader);

		// RejillaVentana
		Material MRejillaVentana;
		MRejillaVentana.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		MRejillaVentana.diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		MRejillaVentana.specular = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
		materialSinTexturaShader.setVec4("MaterialAmbientColor", MRejillaVentana.ambient);
		materialSinTexturaShader.setVec4("MaterialDiffuseColor", MRejillaVentana.diffuse);
		materialSinTexturaShader.setVec4("MaterialSpecularColor", MRejillaVentana.specular);
		materialSinTexturaShader.setFloat("transparency", MRejillaVentana.transparency);
		materialSinTexturaShader.setInt("shininess", 5);
		RejillaVentana.Draw(materialSinTexturaShader);

		// MarcoPuertaRojo
		Material MMarcoPuertaRojo;
		MMarcoPuertaRojo.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		MMarcoPuertaRojo.diffuse = glm::vec4(0.563f, 0.008f, 0.026f, 1.0f);
		MMarcoPuertaRojo.specular = glm::vec4(0.563f, 0.008f, 0.026f, 1.0f);
		materialSinTexturaShader.setVec4("MaterialAmbientColor", MMarcoPuertaRojo.ambient);
		materialSinTexturaShader.setVec4("MaterialDiffuseColor", MMarcoPuertaRojo.diffuse);
		materialSinTexturaShader.setVec4("MaterialSpecularColor", MMarcoPuertaRojo.specular);
		materialSinTexturaShader.setFloat("transparency", MMarcoPuertaRojo.transparency);
		materialSinTexturaShader.setInt("shininess", 20);
		MarcoPuertaRojo.Draw(materialSinTexturaShader);

		// MarcoPuertaBlanco
		Material MMarcoPuertaBlanco;
		MMarcoPuertaBlanco.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		MMarcoPuertaBlanco.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		MMarcoPuertaBlanco.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		materialSinTexturaShader.setVec4("MaterialAmbientColor", MMarcoPuertaBlanco.ambient);
		materialSinTexturaShader.setVec4("MaterialDiffuseColor", MMarcoPuertaBlanco.diffuse);
		materialSinTexturaShader.setVec4("MaterialSpecularColor", MMarcoPuertaBlanco.specular);
		materialSinTexturaShader.setFloat("transparency", MMarcoPuertaBlanco.transparency);
		materialSinTexturaShader.setInt("shininess", 20);
		MarcoPuertaBlanco.Draw(materialSinTexturaShader);

		// CostadosRojos
		Material MCostadosRojos;
		MCostadosRojos.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		MCostadosRojos.diffuse = glm::vec4(0.397f, 0.006f, 0.003f, 1.0f);
		MCostadosRojos.specular = glm::vec4(0.397f, 0.006f, 0.003f, 1.0f);
		materialSinTexturaShader.setVec4("MaterialAmbientColor", MCostadosRojos.ambient);
		materialSinTexturaShader.setVec4("MaterialDiffuseColor", MCostadosRojos.diffuse);
		materialSinTexturaShader.setVec4("MaterialSpecularColor", MCostadosRojos.specular);
		materialSinTexturaShader.setFloat("transparency", MCostadosRojos.transparency);
		materialSinTexturaShader.setInt("shininess", 30);
		CostadosRojos.Draw(materialSinTexturaShader);

		// FondoCanons
		Material MFondoCanons;
		MFondoCanons.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		MFondoCanons.diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		MFondoCanons.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		materialSinTexturaShader.setVec4("MaterialAmbientColor", MFondoCanons.ambient);
		materialSinTexturaShader.setVec4("MaterialDiffuseColor", MFondoCanons.diffuse);
		materialSinTexturaShader.setVec4("MaterialSpecularColor", MFondoCanons.specular);
		materialSinTexturaShader.setFloat("transparency", MFondoCanons.transparency);
		materialSinTexturaShader.setInt("shininess", 5);
		FondoCanons.Draw(materialSinTexturaShader);
		*/

		// cubo de luz
		/*glm::mat4 cuboLuzM = glm::mat4(1.0f);
		cuboLuzM = glm::translate(cuboLuzM, LuzPosition);
		materialSinTexturaShader.setMat4("model", cuboLuzM);
		Material luzp;
		luzp.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		luzp.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		luzp.specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		materialSinTexturaShader.setVec4("MaterialAmbientColor", luzp.ambient);
		materialSinTexturaShader.setVec4("MaterialDiffuseColor", luzp.diffuse);
		materialSinTexturaShader.setVec4("MaterialSpecularColor", luzp.specular);
		materialSinTexturaShader.setFloat("transparency", luzp.transparency);
		materialSinTexturaShader.setInt("shininess", 10);
		cube.Draw(materialSinTexturaShader);*/

		//glUseProgram(0);

	}

	// Materiales con textura
	{
		// Activamos el shader estático
		fondoShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		fondoShader->setMat4("projection", projection);
		fondoShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));	// it's a bit too big for our scene, so scale it down
		fondoShader->setMat4("model", model);

		// Propiedades de la luz

		// Luz direccional
		fondoShader->setVec3("dirLight.direction", 0.0f, -1.0f, 0.0f);
		fondoShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		fondoShader->setVec3("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
		fondoShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		fondoShader->setVec3("dirLight.power", 5.0f, 5.0f, 5.0f);

		// Enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Fondo
		Material MFondo;
		fondoShader->setVec4("MaterialAmbientColor", MFondo.ambient);
		fondoShader->setVec4("MaterialDiffuseColor", MFondo.diffuse);
		fondoShader->setVec4("MaterialSpecularColor", MFondo.specular);
		fondoShader->setFloat("transparency", MFondo.transparency);
		fondoShader->setInt("shininess", 1);
		Fondo->Draw(*fondoShader);

		glUseProgram(0);

	}

	/*// Objetos Sin textura con Iluminación de Fresnel
	
	{
		// Activamos el shader estático
		fresnelShader.use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		fresnelShader.setMat4("projection", projection);
		fresnelShader.setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		fresnelShader.setMat4("model", model);
		fresnelShader.setFloat("transparency", 0.2f);
		// Enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		sphere.Draw(fresnelShader);

		glUseProgram(0);
	}*/

	// glfw: swap buffers 
	/* Swap front and back buffers */
	glfwSwapBuffers(window);

	/* Poll for and process events */
	glfwPollEvents();

	return true;
}

// Procesamos entradas del teclado
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	//if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	// Character movement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		linear += 0.001f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		linear -= 0.001f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		quadratic += 0.001f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		quadratic -= 0.001f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		quadratic -= 0.001f;
	}

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		difusse = glm::vec3(1.0f, 1.0f, 1.0f);
		specular = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		difusse = glm::vec3(0.0f, 0.0f, 0.0f);
		specular = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		amb += glm::vec4(0.01f, 0.01f, 0.01f, 0.0f);
		std::cout << amb.x << amb.y << amb.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		amb -= glm::vec4(0.01f, 0.01f, 0.01f, 0.0f);
		std::cout << amb.x << amb.y << amb.z << std::endl;
	}


	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		Tshine += 1;
		std::cout << Tshine << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		std::cout << Tshine << std::endl;
		Tshine -= 1;
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		LuzPosition += glm::vec3(0.1f, 0.0f, 0.0f);
		std::cout << LuzPosition.x << "," << LuzPosition.y << "," << LuzPosition.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		LuzPosition -= glm::vec3(0.1f, 0.0f, 0.0f);
		std::cout << LuzPosition.x << "," << LuzPosition.y << "," << LuzPosition.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		LuzPosition += glm::vec3(0.0f, 0.1f, 0.0f);
		std::cout << LuzPosition.x << "," << LuzPosition.y << "," << LuzPosition.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		LuzPosition -= glm::vec3(0.0f, 0.1f, 0.0f);
		std::cout << LuzPosition.x << "," << LuzPosition.y << "," << LuzPosition.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		LuzPosition += glm::vec3(0.0f, 0.0f, 0.1f);
		std::cout << LuzPosition.x << "," << LuzPosition.y << "," << LuzPosition.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		LuzPosition -= glm::vec3(0.0f, 0.0f, 0.1f);
		std::cout << LuzPosition.x << "," << LuzPosition.y << "," << LuzPosition.z << std::endl;
	}
}

// glfw: Actualizamos el puerto de vista si hay cambios del tamaño
// de la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: Callback del movimiento y eventos del mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);

}

// glfw: Complemento para el movimiento y eventos del mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
