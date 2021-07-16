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
#include <DirectionalLight.h>
#include <SpotLight.h>
#include <PointLight.h>
#include <Material.h>

// Functions
bool Start();
bool Update();
bool InicializacionShaders();
bool InicializacionModelos();

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

// Variables para las pruebas
glm::vec3 LuzPosition = glm::vec3(0.0f, 0.0f, 0.0f);

// Luces 
// Luz direccional
DirectionalLight sol;
DirectionalLight contraSol;
DirectionalLight posx;
DirectionalLight negx;
DirectionalLight posy;
DirectionalLight negy;
DirectionalLight posz;
DirectionalLight negz;
// PointLights
PointLight L0;
PointLight L1;
PointLight L2;
PointLight L3;
// Flashlight
SpotLight Lintern;

// Variables globales para la pantalla
GLFWwindow* window;
GLfloat  bgR, bgG, bgB, bgA;

// Shaders
Shader* materialSinTexturaShader;
Shader* fondoShader;
Shader* fresnelShader;

// Modelos
// Piso de prueba
Model* Floor;
// Modelos del barco
// Phong
Model* Canons;
Model* MarcoOroPuerta;
Model* MaderaPuerta;
Model* MarcoPuertaRojo;
Model* MarcoPuertaBlanco;
Model* CostadosRojos;
Model* FondoCanons;
Model* RejillaVentana;
Model* MarcoCostado;
Model* BarcoBase;
Model* Pasto;
Model* Paredes1;
Model* PisoMadera;
Model* BarandalesYLaterales;
Model* CintaInferior;
Model* BotonesDorados;
Model* BandaSuperior;
Model* TechoParedSuperior;
Model* Propulsor;
Model* AmarilloTecho;
Model* MarcoVentana;
Model* MarcoFarol;
Model* BaseBandera;
Model* Velas;
Model* BanderaRoja;
Model* MetalMastil;
Model* BaseCuartoMastil;
Model* MaderaArbol;
Model* Hojas;
Model* MaterialCadena;
Model* FondoCara;
Model* Huesos;
Model* FondoOjos;
Model* Melena;
// Fresnel
Model* Vidrio;
Model* VentanaPuertaSinMarco;

// Modelo del fondo
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
	bgR = 0.54;
	bgG = 0.86;
	bgB = 1.0f;
	bgA = 1.0f;

	// Compilación y enlace de shaders
	if (!InicializacionShaders()) {
		return false;
	}
	
	// Carga la información de los modelos
	if (!InicializacionModelos()) {
		return false;
	}

	return true;
}

// Carga y compilación de los shaders
bool InicializacionShaders() {
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
	fresnelShader = new Shader("shaders/11_Fresnel.vs", "shaders/11_Fresnel.fs");
	if (fondoShader == nullptr) {
		cout << "Error creating shaders." << endl;
		return false;
	}
	return true;
}

// Carga de los modelos
bool InicializacionModelos() {
	/*
	Floor = new Model("models/materials/floor.fbx");
	if (Floor == nullptr) {
		cout << "Error loading Floor Model." << endl;
		return false;
	}*/

	// Materiales Barco Exterior Phong 
	
	{
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
		MaderaPuerta = new Model("models/Sunny/MaderaPuerta.fbx");
		if (MaderaPuerta == nullptr) {
			cout << "Error loading MaderaPuerta Model." << endl;
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
		RejillaVentana = new Model("models/Sunny/RejillaVentana.fbx");
		if (RejillaVentana == nullptr) {
			cout << "Error loading RejillaVentana Model." << endl;
			return false;
		}

		MarcoCostado = new Model("models/Sunny/MarcoCostado.fbx");
		if (MarcoCostado == nullptr) {
			cout << "Error loading MarcoCostado Model." << endl;
			return false;
		}
		BarcoBase = new Model("models/Sunny/BarcoBase.fbx");
		if (BarcoBase == nullptr) {
			cout << "Error loading BarcoBase Model." << endl;
			return false;
		}
		Pasto = new Model("models/Sunny/Pasto.fbx");
		if (Pasto == nullptr) {
			cout << "Error loading Pasto Model." << endl;
			return false;
		}
		Paredes1 = new Model("models/Sunny/Paredes1.fbx");
		if (Paredes1 == nullptr) {
			cout << "Error loading Paredes1 Model." << endl;
			return false;
		}
		PisoMadera = new Model("models/Sunny/PisoMadera.fbx");
		if (PisoMadera == nullptr) {
			cout << "Error loading PisoMadera Model." << endl;
			return false;
		}
		BarandalesYLaterales = new Model("models/Sunny/BarandalesYLaterales.fbx");
		if (BarandalesYLaterales == nullptr) {
			cout << "Error loading BarandalesYLaterales Model." << endl;
			return false;
		}
		CintaInferior = new Model("models/Sunny/CintaInferior.fbx");
		if (CintaInferior == nullptr) {
			cout << "Error loading CintaInferior Model." << endl;
			return false;
		}
		BotonesDorados = new Model("models/Sunny/BotonesDorados.fbx");
		if (BotonesDorados == nullptr) {
			cout << "Error loading BotonesDorados Model." << endl;
			return false;
		}
		BandaSuperior = new Model("models/Sunny/BandaSuperior.fbx");
		if (BandaSuperior == nullptr) {
			cout << "Error loading BandaSuperior Model." << endl;
			return false;
		}
		TechoParedSuperior = new Model("models/Sunny/TechoParedSuperior.fbx");
		if (TechoParedSuperior == nullptr) {
			cout << "Error loading TechoParedSuperior Model." << endl;
			return false;
		}
		Propulsor = new Model("models/Sunny/Propulsor.fbx");
		if (Propulsor == nullptr) {
			cout << "Error loading Propulsor Model." << endl;
			return false;
		}
		AmarilloTecho = new Model("models/Sunny/AmarilloTecho.fbx");
		if (AmarilloTecho == nullptr) {
			cout << "Error loading AmarilloTecho Model." << endl;
			return false;
		}
		MarcoVentana = new Model("models/Sunny/MarcoVentana.fbx");
		if (MarcoVentana == nullptr) {
			cout << "Error loading MarcoVentana Model." << endl;
			return false;
		}
		MarcoFarol = new Model("models/Sunny/MarcoFarol.fbx");
		if (MarcoFarol == nullptr) {
			cout << "Error loading MarcoFarol Model." << endl;
			return false;
		}
		BaseBandera = new Model("models/Sunny/BaseBandera.fbx");
		if (BaseBandera == nullptr) {
			cout << "Error loading BaseBandera Model." << endl;
			return false;
		}
		Velas = new Model("models/Sunny/Velas.fbx");
		if (Velas == nullptr) {
			cout << "Error loading Velas Model." << endl;
			return false;
		}
		BanderaRoja = new Model("models/Sunny/BanderaRoja.fbx");
		if (BanderaRoja == nullptr) {
			cout << "Error loading BanderaRoja Model." << endl;
			return false;
		}
		MetalMastil = new Model("models/Sunny/MetalMastil.fbx");
		if (MetalMastil == nullptr) {
			cout << "Error loading MetalMastil Model." << endl;
			return false;
		}
		BaseCuartoMastil = new Model("models/Sunny/BaseCuartoMastil.fbx");
		if (BaseCuartoMastil == nullptr) {
			cout << "Error loading BaseCuartoMastil Model." << endl;
			return false;
		}
		MaderaArbol = new Model("models/Sunny/MaderaArbol.fbx");
		if (MaderaArbol == nullptr) {
			cout << "Error loading MaderaArbol Model." << endl;
			return false;
		}
		Hojas = new Model("models/Sunny/Hojas.fbx");
		if (Hojas == nullptr) {
			cout << "Error loading Hojas Model." << endl;
			return false;
		}
		MaterialCadena = new Model("models/Sunny/MaterialCadena.fbx");
		if (MaterialCadena == nullptr) {
			cout << "Error loading MaterialCadena Model." << endl;
			return false;
		}
		FondoCara = new Model("models/Sunny/FondoCara.fbx");
		if (FondoCara == nullptr) {
			cout << "Error loading FondoCara Model." << endl;
			return false;
		}
		Huesos = new Model("models/Sunny/Huesos.fbx");
		if (Huesos == nullptr) {
			cout << "Error loading Huesos Model." << endl;
			return false;
		}
		FondoOjos = new Model("models/Sunny/FondoOjos.fbx");
		if (FondoOjos == nullptr) {
			cout << "Error loading FondoOjos Model." << endl;
			return false;
		}
		Melena = new Model("models/Sunny/Melena.fbx");
		if (Melena == nullptr) {
			cout << "Error loading Melena Model." << endl;
			return false;
		}
	}
	
	/*
	A = new Model("models/Sunny/A.fbx");
	if (A == nullptr) {
		cout << "Error loading A Model." << endl;
		return false;
	}*/

	// Materiales Barco Exterior Fresnel
	Vidrio = new Model("models/Sunny/Vidrio.fbx");
	if (Vidrio == nullptr) {
		cout << "Error loading Vidrio Model." << endl;
		return false;
	}
	VentanaPuertaSinMarco = new Model("models/Sunny/VentanaPuertaSinMarco.fbx");
	if (VentanaPuertaSinMarco == nullptr) {
		cout << "Error loading VentanaPuertaSinMarco Model." << endl;
		return false;
	}

	// Fondo Cubo 
	Fondo = new Model("models/Sunny/CubeMap.fbx");
	if (Fondo == nullptr) {
		cout << "Error loading Fondo Model." << endl;
		return false;
	}
	/*
	cube = new Model("models/materials/cube.fbx");
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

	// Cambio de propiedades de la Luz
	// Luz direccional
	contraSol.Direction = glm::vec3(0.0f, 1.0f, 0.0f);
	posx.Direction = glm::vec3(1.0f, 0.0f, 0.0f);
	negx.Direction = glm::vec3(-1.0f, 0.0f, 0.0f);
	posy.Direction = glm::vec3(0.0f, 1.0f, 0.0f);
	negy.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
	posz.Direction = glm::vec3(0.0f, 0.0f, 1.0f);
	negz.Direction = glm::vec3(0.0f, 0.0f, -1.0f);

	// Puntos de luz
	L0.setPosition(38.4f, -0.6f, -22.9f);
	L1.setPosition(-28.40f, -0.6f, -22.9f);
	L2.setPosition(0.0f, -0.6, 0.0f);
	L3.setPosition(0.0f, 0.0f, -65.90f);

	// Proyecciones y vistas
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	
	// Materiales sin textura
	{
		// Activamos el shader estático
		materialSinTexturaShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		materialSinTexturaShader->setMat4("projection", projection);
		materialSinTexturaShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Trasladar al centro de la escena
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// escala para los objetos que sean muy grandes
		materialSinTexturaShader->setMat4("model", model);

		// Propiedades de la luz

		// Luz direccional 1
		materialSinTexturaShader->setVec3("dirLight[0].direction", sol.Direction);
		materialSinTexturaShader->setVec3("dirLight[0].ambient", sol.Ambient);
		materialSinTexturaShader->setVec3("dirLight[0].diffuse", sol.Diffuse);
		materialSinTexturaShader->setVec3("dirLight[0].specular", sol.Specular);
		materialSinTexturaShader->setVec3("dirLight[0].power", sol.Power);
		//// Luz direccional 2
		//materialSinTexturaShader->setVec3("dirLight[1].direction", contraSol.Direction);
		//materialSinTexturaShader->setVec3("dirLight[1].ambient", contraSol.Ambient);
		//materialSinTexturaShader->setVec3("dirLight[1].diffuse", contraSol.Diffuse);
		//materialSinTexturaShader->setVec3("dirLight[1].specular", contraSol.Specular);
		//materialSinTexturaShader->setVec3("dirLight[1].power", contraSol.Power);

		//// Luz direccional posx
		//materialSinTexturaShader->setVec3("dirLight[0].direction", posx.Direction);
		//materialSinTexturaShader->setVec3("dirLight[0].ambient", posx.Ambient);
		//materialSinTexturaShader->setVec3("dirLight[0].diffuse", posx.Diffuse);
		//materialSinTexturaShader->setVec3("dirLight[0].specular", posx.Specular);
		//materialSinTexturaShader->setVec3("dirLight[0].power", posx.Power);
		//// Luz direccional negx
		//materialSinTexturaShader->setVec3("dirLight[1].direction", negx.Direction);
		//materialSinTexturaShader->setVec3("dirLight[1].ambient", negx.Ambient);
		//materialSinTexturaShader->setVec3("dirLight[1].diffuse", negx.Diffuse);
		//materialSinTexturaShader->setVec3("dirLight[1].specular", negx.Specular);
		//materialSinTexturaShader->setVec3("dirLight[1].power", negx.Power);
		//// Luz direccional posy
		//materialSinTexturaShader->setVec3("dirLight[2].direction", posy.Direction);
		//materialSinTexturaShader->setVec3("dirLight[2].ambient", posy.Ambient);
		//materialSinTexturaShader->setVec3("dirLight[2].diffuse", posy.Diffuse);
		//materialSinTexturaShader->setVec3("dirLight[2].specular", posy.Specular);
		//materialSinTexturaShader->setVec3("dirLight[2].power", posy.Power);
		//// Luz direccional negy
		//materialSinTexturaShader->setVec3("dirLight[3].direction", negy.Direction);
		//materialSinTexturaShader->setVec3("dirLight[3].ambient", negy.Ambient);
		//materialSinTexturaShader->setVec3("dirLight[3].diffuse", negy.Diffuse);
		//materialSinTexturaShader->setVec3("dirLight[3].specular", negy.Specular);
		//materialSinTexturaShader->setVec3("dirLight[3].power", negy.Power);
		//// Luz direccional posz
		//materialSinTexturaShader->setVec3("dirLight[4].direction", posz.Direction);
		//materialSinTexturaShader->setVec3("dirLight[4].ambient", posz.Ambient);
		//materialSinTexturaShader->setVec3("dirLight[4].diffuse", posz.Diffuse);
		//materialSinTexturaShader->setVec3("dirLight[4].specular", posz.Specular);
		//materialSinTexturaShader->setVec3("dirLight[4].power", posz.Power);
		//// Luz direccional negz
		//materialSinTexturaShader->setVec3("dirLight[5].direction", negz.Direction);
		//materialSinTexturaShader->setVec3("dirLight[5].ambient", negz.Ambient);
		//materialSinTexturaShader->setVec3("dirLight[5].diffuse", negz.Diffuse);
		//materialSinTexturaShader->setVec3("dirLight[5].specular", negz.Specular);
		//materialSinTexturaShader->setVec3("dirLight[5].power", negz.Power);
		// Luz de linterna
		materialSinTexturaShader->setVec3("spotLight.position", camera.Position);
		materialSinTexturaShader->setVec3("spotLight.direction", camera.Front);
		materialSinTexturaShader->setVec3("spotLight.ambient", Lintern.Ambient);
		materialSinTexturaShader->setVec3("spotLight.diffuse", Lintern.Diffuse);
		materialSinTexturaShader->setVec3("spotLight.specular", Lintern.Specular);
		materialSinTexturaShader->setFloat("spotLight.constant", Lintern.Constant);
		materialSinTexturaShader->setFloat("spotLight.linear", Lintern.Linear);
		materialSinTexturaShader->setFloat("spotLight.quadratic", Lintern.Quadratic);
		materialSinTexturaShader->setFloat("spotLight.cutOff", Lintern.CutOff);
		materialSinTexturaShader->setFloat("spotLight.outerCutOff", Lintern.OuterCutOff);

		//// point light 1
		//materialSinTexturaShader->setVec3("pointLights[0].position", L0.Position);
		//materialSinTexturaShader->setVec3("pointLights[0].ambient", L0.Ambient);
		//materialSinTexturaShader->setVec3("pointLights[0].diffuse", L0.Diffuse);
		//materialSinTexturaShader->setVec3("pointLights[0].specular", L0.Specular);
		//materialSinTexturaShader->setFloat("pointLights[0].constant", L0.Constant);
		//materialSinTexturaShader->setFloat("pointLights[0].linear", L0.Linear);
		//materialSinTexturaShader->setFloat("pointLights[0].quadratic", L0.Quadratic);
		//materialSinTexturaShader->setVec3("pointLights[0].power", L0.Power);
		//
		//// point light 2
		//materialSinTexturaShader->setVec3("pointLights[1].position", L1.Position);
		//materialSinTexturaShader->setVec3("pointLights[1].ambient", L1.Ambient);
		//materialSinTexturaShader->setVec3("pointLights[1].diffuse", L1.Diffuse);
		//materialSinTexturaShader->setVec3("pointLights[1].specular", L1.Specular);
		//materialSinTexturaShader->setFloat("pointLights[1].constant", L1.Constant);
		//materialSinTexturaShader->setFloat("pointLights[1].linear", L1.Linear);
		//materialSinTexturaShader->setFloat("pointLights[1].quadratic", L1.Quadratic);
		//materialSinTexturaShader->setVec3("pointLights[1].power", L1.Power);

		//// point light 3
		//materialSinTexturaShader->setVec3("pointLights[2].position", L2.Position);
		//materialSinTexturaShader->setVec3("pointLights[2].ambient", L2.Ambient);
		//materialSinTexturaShader->setVec3("pointLights[2].diffuse", L2.Diffuse);
		//materialSinTexturaShader->setVec3("pointLights[2].specular", L2.Specular);
		//materialSinTexturaShader->setFloat("pointLights[2].constant", L2.Constant);
		//materialSinTexturaShader->setFloat("pointLights[2].linear", L2.Linear);
		//materialSinTexturaShader->setFloat("pointLights[2].quadratic", L2.Quadratic);
		//materialSinTexturaShader->setVec3("pointLights[2].power", L2.Power);

		//// point light 4
		//materialSinTexturaShader->setVec3("pointLights[3].position", L3.Position);
		//materialSinTexturaShader->setVec3("pointLights[3].ambient", L3.Ambient);
		//materialSinTexturaShader->setVec3("pointLights[3].diffuse", L3.Diffuse);
		//materialSinTexturaShader->setVec3("pointLights[3].specular", L3.Specular);
		//materialSinTexturaShader->setFloat("pointLights[3].constant", L3.Constant);
		//materialSinTexturaShader->setFloat("pointLights[3].linear", L3.Linear);
		//materialSinTexturaShader->setFloat("pointLights[3].quadratic", L3.Quadratic);
		//materialSinTexturaShader->setVec3("pointLights[3].power", L3.Power);
		
		// Posición de la camara
		materialSinTexturaShader->setVec3("viewPos", camera.Position);

		// Habilitar blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		/*
		// Piso de prueba
		Material m1;
		materialSinTexturaShader->setVec4("MaterialAmbientColor", m1.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", m1.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", m1.specular);
		materialSinTexturaShader->setFloat("transparency", m1.transparency);
		materialSinTexturaShader->setInt("shininess", 10);
		Floor.Draw(materialSinTexturaShader);*/

		// Propiedades materiales del Barco
		
		// Canons
		Material MCanons;
		MCanons.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		MCanons.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MCanons.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MCanons.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MCanons.specular);
		materialSinTexturaShader->setFloat("transparency", MCanons.transparency);
		materialSinTexturaShader->setInt("shininess", 5);
		Canons->Draw(*materialSinTexturaShader);

		// MarcoOroPuerta
		Material MMarcoOroPuerta;
		MMarcoOroPuerta.diffuse = glm::vec4(0.589f, 0.266f, 0.021f, 1.0f);
		MMarcoOroPuerta.specular = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MMarcoOroPuerta.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MMarcoOroPuerta.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MMarcoOroPuerta.specular);
		materialSinTexturaShader->setFloat("transparency", MMarcoOroPuerta.transparency);
		materialSinTexturaShader->setInt("shininess", 2);
		MarcoOroPuerta->Draw(*materialSinTexturaShader);
		
		// MaderaPuerta
		Material MMaderaPuerta;
		MMaderaPuerta.diffuse = glm::vec4(0.166f, 0.060f, 0.026f, 1.0f);
		MMaderaPuerta.specular = glm::vec4(0.166f, 0.060f, 0.026f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MMaderaPuerta.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MMaderaPuerta.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MMaderaPuerta.specular);
		materialSinTexturaShader->setFloat("transparency", MMaderaPuerta.transparency);
		materialSinTexturaShader->setInt("shininess", 50);
		MaderaPuerta->Draw(*materialSinTexturaShader);
		
		// MarcoPuertaRojo
		Material MMarcoPuertaRojo;
		MMarcoPuertaRojo.diffuse = glm::vec4(0.563f, 0.008f, 0.026f, 1.0f);
		MMarcoPuertaRojo.specular = glm::vec4(0.563f, 0.008f, 0.026f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MMarcoPuertaRojo.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MMarcoPuertaRojo.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MMarcoPuertaRojo.specular);
		materialSinTexturaShader->setFloat("transparency", MMarcoPuertaRojo.transparency);
		materialSinTexturaShader->setInt("shininess", 20);
		MarcoPuertaRojo->Draw(*materialSinTexturaShader);

		// MarcoPuertaBlanco
		Material MMarcoPuertaBlanco;
		MMarcoPuertaBlanco.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		MMarcoPuertaBlanco.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MMarcoPuertaBlanco.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MMarcoPuertaBlanco.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MMarcoPuertaBlanco.specular);
		materialSinTexturaShader->setFloat("transparency", MMarcoPuertaBlanco.transparency);
		materialSinTexturaShader->setInt("shininess", 20);
		MarcoPuertaBlanco->Draw(*materialSinTexturaShader);

		// CostadosRojos
		Material MCostadosRojos;
		MCostadosRojos.diffuse = glm::vec4(0.397f, 0.006f, 0.003f, 1.0f);
		MCostadosRojos.specular = glm::vec4(0.397f, 0.006f, 0.003f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MCostadosRojos.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MCostadosRojos.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MCostadosRojos.specular);
		materialSinTexturaShader->setFloat("transparency", MCostadosRojos.transparency);
		materialSinTexturaShader->setInt("shininess", 30);
		CostadosRojos->Draw(*materialSinTexturaShader);

		// FondoCanons
		Material MFondoCanons;
		MFondoCanons.diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		MFondoCanons.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MFondoCanons.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MFondoCanons.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MFondoCanons.specular);
		materialSinTexturaShader->setFloat("transparency", MFondoCanons.transparency);
		materialSinTexturaShader->setInt("shininess", 5);
		FondoCanons->Draw(*materialSinTexturaShader);

		// RejillaVentana
		Material MRejillaVentana;
		MRejillaVentana.diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		MRejillaVentana.specular = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MRejillaVentana.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MRejillaVentana.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MRejillaVentana.specular);
		materialSinTexturaShader->setFloat("transparency", MRejillaVentana.transparency);
		materialSinTexturaShader->setInt("shininess", 5);
		RejillaVentana->Draw(*materialSinTexturaShader);

		Material MMarcoCostado;
		MMarcoCostado.diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		MMarcoCostado.specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MMarcoCostado.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MMarcoCostado.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MMarcoCostado.specular);
		materialSinTexturaShader->setFloat("transparency", MMarcoCostado.transparency);
		materialSinTexturaShader->setInt("shininess", 5);
		MarcoCostado->Draw(*materialSinTexturaShader);
		
		Material MBarcoBase;
		MBarcoBase.diffuse = glm::vec4(0.078f, 0.049f, 0.033f, 1.0f);
		MBarcoBase.specular = glm::vec4(0.078f, 0.049f, 0.033f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MBarcoBase.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MBarcoBase.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MBarcoBase.specular);
		materialSinTexturaShader->setFloat("transparency", MBarcoBase.transparency);
		materialSinTexturaShader->setInt("shininess", 50);
		BarcoBase->Draw(*materialSinTexturaShader);
		
		Material MPasto;
		MPasto.diffuse = glm::vec4(0.016f, 0.336f, 0.0f, 1.0f);
		MPasto.specular = glm::vec4(0.016f, 0.336f, 0.0f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MPasto.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MPasto.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MPasto.specular);
		materialSinTexturaShader->setFloat("transparency", MPasto.transparency);
		materialSinTexturaShader->setInt("shininess", 10);
		Pasto->Draw(*materialSinTexturaShader);
		
		Material MParedes1;
		MParedes1.diffuse = glm::vec4(0.665f, 0.552f, 0.361f, 1.0f);
		MParedes1.specular = glm::vec4(0.665f, 0.552f, 0.361f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MParedes1.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MParedes1.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MParedes1.specular);
		materialSinTexturaShader->setFloat("transparency", MParedes1.transparency);
		materialSinTexturaShader->setInt("shininess", 30);
		Paredes1->Draw(*materialSinTexturaShader);
		
		Material MPisoMadera; //***************************************************************************************** checar
		MPisoMadera.diffuse = glm::vec4(0.119f, 0.029f, 0.06f, 1.0f);
		MPisoMadera.specular = glm::vec4(0.119f, 0.029f, 0.06f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MPisoMadera.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MPisoMadera.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MPisoMadera.specular);
		materialSinTexturaShader->setFloat("transparency", MPisoMadera.transparency);
		materialSinTexturaShader->setInt("shininess", 50);
		PisoMadera->Draw(*materialSinTexturaShader);
		
		// BarandalesYLaterales
		Material MBarandalesYLaterales;
		MBarandalesYLaterales.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		MBarandalesYLaterales.specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MBarandalesYLaterales.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MBarandalesYLaterales.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MBarandalesYLaterales.specular);
		materialSinTexturaShader->setFloat("transparency", MBarandalesYLaterales.transparency);
		materialSinTexturaShader->setInt("shininess", 10);
		BarandalesYLaterales->Draw(*materialSinTexturaShader);
		
		Material MCintaInferior;
		MCintaInferior.diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		MCintaInferior.specular = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MCintaInferior.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MCintaInferior.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MCintaInferior.specular);
		materialSinTexturaShader->setFloat("transparency", MCintaInferior.transparency);
		materialSinTexturaShader->setInt("shininess", 10);
		CintaInferior->Draw(*materialSinTexturaShader);
		
		Material MBotonesDorados;
		MBotonesDorados.diffuse = glm::vec4(0.589f, 0.266f, 0.021f, 1.0f);
		MBotonesDorados.specular = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MBotonesDorados.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MBotonesDorados.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MBotonesDorados.specular);
		materialSinTexturaShader->setFloat("transparency", MBotonesDorados.transparency);
		materialSinTexturaShader->setInt("shininess", 5);
		BotonesDorados->Draw(*materialSinTexturaShader);
		
		Material MBandaSuperior;
		MBandaSuperior.diffuse = glm::vec4(0.397f, 0.002f, 0.0f, 1.0f);
		MBandaSuperior.specular = glm::vec4(0.497f, 0.102f, 0.1f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MBandaSuperior.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MBandaSuperior.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MBandaSuperior.specular);
		materialSinTexturaShader->setFloat("transparency", MBandaSuperior.transparency);
		materialSinTexturaShader->setInt("shininess", 10);
		BandaSuperior->Draw(*materialSinTexturaShader);
		
		Material MTechoParedSuperior;
		MTechoParedSuperior.diffuse = glm::vec4(0.52f, 0.004f, 0.0f, 1.0f);
		MTechoParedSuperior.specular = glm::vec4(0.52f, 0.004f, 0.0f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MTechoParedSuperior.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MTechoParedSuperior.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MTechoParedSuperior.specular);
		materialSinTexturaShader->setFloat("transparency", MTechoParedSuperior.transparency);
		materialSinTexturaShader->setInt("shininess", 30);
		TechoParedSuperior->Draw(*materialSinTexturaShader);
		
		Material MPropulsor;
		MPropulsor.diffuse = glm::vec4(0.0f, 0.0f, 0.05f, 1.0f);
		MPropulsor.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MPropulsor.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MPropulsor.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MPropulsor.specular);
		materialSinTexturaShader->setFloat("transparency", MPropulsor.transparency);
		materialSinTexturaShader->setInt("shininess", 5);
		Propulsor->Draw(*materialSinTexturaShader);
		
		Material MAmarilloTecho;
		MAmarilloTecho.diffuse = glm::vec4(0.8f, 0.793f, 0.035f, 1.0f);
		MAmarilloTecho.specular = glm::vec4(0.8f, 0.793f, 0.035f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MAmarilloTecho.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MAmarilloTecho.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MAmarilloTecho.specular);
		materialSinTexturaShader->setFloat("transparency", MAmarilloTecho.transparency);
		materialSinTexturaShader->setInt("shininess", 30);
		AmarilloTecho->Draw(*materialSinTexturaShader);
		
		Material MMarcoVentana;
		MMarcoVentana.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		MMarcoVentana.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MMarcoVentana.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MMarcoVentana.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MMarcoVentana.specular);
		materialSinTexturaShader->setFloat("transparency", MMarcoVentana.transparency);
		materialSinTexturaShader->setInt("shininess", 30);
		MarcoVentana->Draw(*materialSinTexturaShader);
		
		Material MMarcoFarol;
		MMarcoFarol.diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		MMarcoFarol.specular = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MMarcoFarol.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MMarcoFarol.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MMarcoFarol.specular);
		materialSinTexturaShader->setFloat("transparency", MMarcoFarol.transparency);
		materialSinTexturaShader->setInt("shininess", 10);
		MarcoFarol->Draw(*materialSinTexturaShader);
		
		Material MBaseBandera;
		MBaseBandera.diffuse = glm::vec4(0.665f, 0.552f, 0.361f, 1.0f);
		MBaseBandera.specular = glm::vec4(0.665f, 0.552f, 0.361f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MBaseBandera.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MBaseBandera.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MBaseBandera.specular);
		materialSinTexturaShader->setFloat("transparency", MBaseBandera.transparency);
		materialSinTexturaShader->setInt("shininess", 30);
		BaseBandera->Draw(*materialSinTexturaShader);
		
		Material MVelas;
		MVelas.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		MVelas.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MVelas.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MVelas.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MVelas.specular);
		materialSinTexturaShader->setFloat("transparency", MVelas.transparency);
		materialSinTexturaShader->setInt("shininess", 10);
		Velas->Draw(*materialSinTexturaShader);

		Material MBanderaRoja;
		MBanderaRoja.diffuse = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);
		MBanderaRoja.specular = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MBanderaRoja.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MBanderaRoja.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MBanderaRoja.specular);
		materialSinTexturaShader->setFloat("transparency", MBanderaRoja.transparency);
		materialSinTexturaShader->setInt("shininess", 10);
		BanderaRoja->Draw(*materialSinTexturaShader);

		Material MMetalMastil;
		MMetalMastil.diffuse = glm::vec4(0.027f, 0.051f, 0.087f, 1.0f);
		MMetalMastil.specular = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MMetalMastil.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MMetalMastil.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MMetalMastil.specular);
		materialSinTexturaShader->setFloat("transparency", MMetalMastil.transparency);
		materialSinTexturaShader->setInt("shininess", 5);
		MetalMastil->Draw(*materialSinTexturaShader);

		Material MBaseCuartoMastil;
		MBaseCuartoMastil.diffuse = glm::vec4(0.212f, 0.262f, 0.266f, 1.0f);
		MBaseCuartoMastil.specular = glm::vec4(0.212f, 0.262f, 0.266f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MBaseCuartoMastil.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MBaseCuartoMastil.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MBaseCuartoMastil.specular);
		materialSinTexturaShader->setFloat("transparency", MBaseCuartoMastil.transparency);
		materialSinTexturaShader->setInt("shininess", 30);
		BaseCuartoMastil->Draw(*materialSinTexturaShader);
		
		Material MMaderaArbol;
		MMaderaArbol.diffuse = glm::vec4(0.05f, 0.012f, 0.003f, 1.0f);
		MMaderaArbol.specular = glm::vec4(0.05f, 0.012f, 0.003f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MMaderaArbol.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MMaderaArbol.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MMaderaArbol.specular);
		materialSinTexturaShader->setFloat("transparency", MMaderaArbol.transparency);
		materialSinTexturaShader->setInt("shininess", 30);
		MaderaArbol->Draw(*materialSinTexturaShader);
		
		Material MHojas;
		MHojas.diffuse = glm::vec4(0.052f, 0.448f, 0.017f, 1.0f);
		MHojas.specular = glm::vec4(0.052f, 0.448f, 0.017f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MHojas.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MHojas.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MHojas.specular);
		materialSinTexturaShader->setFloat("transparency", MHojas.transparency);
		materialSinTexturaShader->setInt("shininess", 10);
		Hojas->Draw(*materialSinTexturaShader);

		Material MMaterialCadena;
		MMaterialCadena.diffuse = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		MMaterialCadena.specular = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MMaterialCadena.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MMaterialCadena.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MMaterialCadena.specular);
		materialSinTexturaShader->setFloat("transparency", MMaterialCadena.transparency);
		materialSinTexturaShader->setInt("shininess", 5);
		MaterialCadena->Draw(*materialSinTexturaShader);

		Material MFondoCara;
		MFondoCara.diffuse = glm::vec4(0.8f, 0.477f, 0.15f, 1.0f);
		MFondoCara.specular = glm::vec4(0.8f, 0.477f, 0.15f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MFondoCara.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MFondoCara.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MFondoCara.specular);
		materialSinTexturaShader->setFloat("transparency", MFondoCara.transparency);
		materialSinTexturaShader->setInt("shininess", 30);
		FondoCara->Draw(*materialSinTexturaShader);

		Material MHuesos;
		MHuesos.diffuse = glm::vec4(0.955f, 0.955f, 0.955f, 1.0f);
		MHuesos.specular = glm::vec4(0.955f, 0.955f, 0.955f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MHuesos.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MHuesos.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MHuesos.specular);
		materialSinTexturaShader->setFloat("transparency", MHuesos.transparency);
		materialSinTexturaShader->setInt("shininess", 20);
		Huesos->Draw(*materialSinTexturaShader);

		Material MFondoOjos;
		MFondoOjos.diffuse = glm::vec4(0.955f, 0.955f, 0.955f, 1.0f);
		MFondoOjos.specular = glm::vec4(0.955f, 0.955f, 0.955f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MFondoOjos.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MFondoOjos.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MFondoOjos.specular);
		materialSinTexturaShader->setFloat("transparency", MFondoOjos.transparency);
		materialSinTexturaShader->setInt("shininess", 20);
		FondoOjos->Draw(*materialSinTexturaShader);

		Material MMelena;
		MMelena.diffuse = glm::vec4(0.8f, 0.037f, 0.021f, 1.0f);
		MMelena.specular = glm::vec4(0.8f, 0.037f, 0.021f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", MMelena.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", MMelena.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", MMelena.specular);
		materialSinTexturaShader->setFloat("transparency", MMelena.transparency);
		materialSinTexturaShader->setInt("shininess", 20);
		Melena->Draw(*materialSinTexturaShader);
		
		// cubo de luz
		/*glm::mat4 cuboLuzM = glm::mat4(1.0f);
		cuboLuzM = glm::translate(cuboLuzM, LuzPosition);
		materialSinTexturaShader->setMat4("model", cuboLuzM);
		Material luzp;
		luzp.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		luzp.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		luzp.specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		materialSinTexturaShader->setVec4("MaterialAmbientColor", luzp.ambient);
		materialSinTexturaShader->setVec4("MaterialDiffuseColor", luzp.diffuse);
		materialSinTexturaShader->setVec4("MaterialSpecularColor", luzp.specular);
		materialSinTexturaShader->setFloat("transparency", luzp.transparency);
		materialSinTexturaShader->setInt("shininess", 10);
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
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Pasar al centro de la escena
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));	// Es muy grande el cubo
		fondoShader->setMat4("model", model);

		// Propiedades de la luz

		// Luz direccional
		fondoShader->setVec3("dirLight.direction", sol.Direction);
		fondoShader->setVec3("dirLight.ambient", sol.Ambient);
		fondoShader->setVec3("dirLight.diffuse", sol.Diffuse);
		fondoShader->setVec3("dirLight.specular", sol.Specular);
		fondoShader->setVec3("dirLight.power", sol.Power);

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
	// Objetos Sin textura con Iluminación de Fresnel
	
	{
		// Cubemap
		vector<std::string> faces
		{
			"models/CubeMapFondo/posx.png",
			"models/CubeMapFondo/negx.png",
			"models/CubeMapFondo/posy.png",
			"models/CubeMapFondo/negy.png",
			"models/CubeMapFondo/posz.png",
			"models/CubeMapFondo/negz.png"
		};
		unsigned int cubemapTexture = loadCubemap(faces);
		// Activamos el shader estático
		fresnelShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		fresnelShader->setMat4("projection", projection);
		fresnelShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		fresnelShader->setMat4("model", model);
		fresnelShader->setFloat("transparency", 1.0f);
		// Enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		Vidrio->Draw(*fresnelShader);
		VentanaPuertaSinMarco->Draw(*fresnelShader);

		glUseProgram(0);
	}

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
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
	}


	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		Lintern.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		Lintern.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		Lintern.Diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
		Lintern.Specular = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
	}
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
	}


	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
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
