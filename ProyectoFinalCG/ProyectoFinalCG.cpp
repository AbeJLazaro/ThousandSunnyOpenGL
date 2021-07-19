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
void MaterialesSinTexturaUnaLuz();
void MaterialesSinTexturaVariasLuces();
void MaterialesSinTexturaFresnel();
void MaterialesConTexturasVariasLuces();

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

// Definición de proyeccion y vista
// Proyecciones y vistas
glm::mat4 projection;
glm::mat4 view;

// Controladores para el movimiento del mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Variables para la velocidad de reproducción
// de la animación
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsedTime = 0.0f;

glm::vec3 position(0.0f, 0.0f, 0.0f);

// Variables para el control del modelo
float modelRotation = 0.0f;

// Variables para las pruebas
glm::vec3 LuzPosition = glm::vec3(0.0f, 0.0f, 0.0f);

// Luces 
// Luz direccional
DirectionalLight sol;
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
Shader* materialSinTexturaUnaLuzShader;
Shader* materialSinTexturaVariasLucesShader;
Shader* materialConTexturaVariasLucesShader;
Shader* fondoShader;
Shader* fresnelShader;
Shader* animados;	

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
Model* BarcoBaseTextura;
Model* BarcoCostado;
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
Model* Character;
// Fresnel
Model* Vidrio;
Model* VentanaPuertaSinMarco;

// Modelo del fondo
Model* Fondo;

Model* cube; // sirve para visualizar la posición de las luces

// Máximo número de huesos: 100
#define MAX_RIGGING_BONES 100

// Pose inicial del modelo
glm::mat4 gBones[MAX_RIGGING_BONES];

float fps = 0;
int keys = 0;
int animationCount = 0;


// Ciclo main
int main(void) {
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
	
	animados->setBonesIDs(MAX_RIGGING_BONES);
	
	// Carga la información de los modelos
	if (!InicializacionModelos()) {
		return false;
	}

	// time, arrays
	Character->SetPose(0.0f, gBones);

	fps = (float)Character->getFramerate();
	keys = (int)Character->getNumFrames();

	return true;
}

// Carga y compilación de los shaders
bool InicializacionShaders() {
	materialSinTexturaUnaLuzShader = new Shader("shaders/objetosSinTextura1Luz.vs", "shaders/objetosSinTextura1Luz.fs");
	if (materialSinTexturaUnaLuzShader == nullptr) {
		cout << "Error creating shaders." << endl;
		return false;
	}
	materialSinTexturaVariasLucesShader = new Shader("shaders/objetosSinTexturaVariasLuces.vs", "shaders/objetosSinTexturaVariasLuces.fs");
	if (materialSinTexturaVariasLucesShader == nullptr) {
		cout << "Error creating shaders." << endl;
		return false;
	}
	materialConTexturaVariasLucesShader = new Shader("shaders/objetosConTexturaVariasLuces.vs", "shaders/objetosConTexturaVariasLuces.fs");
	if (materialConTexturaVariasLucesShader == nullptr) {
		cout << "Error creating shaders." << endl;
		return false;
	}
	animados = new Shader("shaders/pruebas3.vs", "shaders/pruebas3.fs");
	if (animados == nullptr) {
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

	// Materiales Barco Exterior Phong 

	{
		/*Canons = new Model("models/Sunny/Canons.fbx");
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
		}*/
		/*BarcoBaseTextura = new Model("models/Sunny/BarcoBaseTextura.fbx");
		if (BarcoBaseTextura == nullptr) {
			cout << "Error loading BarcoBaseTextura Model." << endl;
			return false;
		}*/
		
		BarcoCostado = new Model("models/Sunny/BarcoCostadoS.fbx");
		if (BarcoCostado == nullptr) {
			cout << "Error loading BarcoCostado Model." << endl;
			return false;
		}
		
		/*Pasto = new Model("models/Sunny/Pasto.fbx");
		if (Pasto == nullptr) {
			cout << "Error loading Pasto Model." << endl;
			return false;
		}*/
		/*Paredes1 = new Model("models/Sunny/Paredes1.fbx");
		if (Paredes1 == nullptr) {
			cout << "Error loading Paredes1 Model." << endl;
			return false;
		}*/
		/*PisoMadera = new Model("models/Sunny/PisoMadera.fbx");
		if (PisoMadera == nullptr) {
			cout << "Error loading PisoMadera Model." << endl;
			return false;
		}*/
		/*BarandalesYLaterales = new Model("models/Sunny/BarandalesYLaterales.fbx");
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
		}*/
		/*MaderaArbol = new Model("models/Sunny/MaderaArbol.fbx");
		if (MaderaArbol == nullptr) {
			cout << "Error loading MaderaArbol Model." << endl;
			return false;
		}*/
		/*Hojas = new Model("models/Sunny/Hojas.fbx");
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
		}*/
	}

	/*
	A = new Model("models/Sunny/A.fbx");
	if (A == nullptr) {
		cout << "Error loading A Model." << endl;
		return false;
	}*/

	// Materiales Barco Exterior Fresnel
	/*Vidrio = new Model("models/Sunny/Vidrio.fbx");
	if (Vidrio == nullptr) {
		cout << "Error loading Vidrio Model." << endl;
		return false;
	}
	VentanaPuertaSinMarco = new Model("models/Sunny/VentanaPuertaSinMarco.fbx");
	if (VentanaPuertaSinMarco == nullptr) {
		cout << "Error loading VentanaPuertaSinMarco Model." << endl;
		return false;
	}*/

	// Objetos animados
	//Character = new Model("models/ZoroSaludando.fbx");
	//if (Character == nullptr) {
	//	cout << "Error loading Character Model." << endl;
	//	return false;
	//}

	//// Fondo Cubo 
	//Fondo = new Model("models/Sunny/CubeMap.fbx");
	//if (Fondo == nullptr) {
	//	cout << "Error loading Fondo Model." << endl;
	//	return false;
	//}
	/*
	cube = new Model("models/materials/cube.fbx");
	if (cube == nullptr) {
		cout << "Error loading cube Model." << endl;
		return false;
	}*/
	return true;
}

// Dibujo de materiales sin textura con una sola luz direccional
void MaterialesSinTexturaUnaLuz() {
	// Activamos el shader estático
	materialSinTexturaUnaLuzShader->use();

	// Aplicamos transformaciones de proyección y cámara (si las hubiera)
	materialSinTexturaUnaLuzShader->setMat4("projection", projection);
	materialSinTexturaUnaLuzShader->setMat4("view", view);

	// Aplicamos transformaciones del modelo
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Trasladar al centro de la escena
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// escala para los objetos que sean muy grandes
	materialSinTexturaUnaLuzShader->setMat4("model", model);

	// Propiedades de la luz

	// Luz direccional 1
	materialSinTexturaUnaLuzShader->setVec3("dirLight[0].direction", sol.Direction);
	materialSinTexturaUnaLuzShader->setVec3("dirLight[0].ambient", sol.Ambient);
	materialSinTexturaUnaLuzShader->setVec3("dirLight[0].diffuse", sol.Diffuse);
	materialSinTexturaUnaLuzShader->setVec3("dirLight[0].specular", sol.Specular);
	materialSinTexturaUnaLuzShader->setVec3("dirLight[0].power", sol.Power);

	// Luz de linterna
	materialSinTexturaUnaLuzShader->setVec3("spotLight.position", camera.Position);
	materialSinTexturaUnaLuzShader->setVec3("spotLight.direction", camera.Front);
	materialSinTexturaUnaLuzShader->setVec3("spotLight.ambient", Lintern.Ambient);
	materialSinTexturaUnaLuzShader->setVec3("spotLight.diffuse", Lintern.Diffuse);
	materialSinTexturaUnaLuzShader->setVec3("spotLight.specular", Lintern.Specular);
	materialSinTexturaUnaLuzShader->setFloat("spotLight.constant", Lintern.Constant);
	materialSinTexturaUnaLuzShader->setFloat("spotLight.linear", Lintern.Linear);
	materialSinTexturaUnaLuzShader->setFloat("spotLight.quadratic", Lintern.Quadratic);
	materialSinTexturaUnaLuzShader->setFloat("spotLight.cutOff", Lintern.CutOff);
	materialSinTexturaUnaLuzShader->setFloat("spotLight.outerCutOff", Lintern.OuterCutOff);

	// Posición de la camara
	materialSinTexturaUnaLuzShader->setVec3("viewPos", camera.Position);

	// Habilitar blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// MarcoOroPuerta
	Material MMarcoOroPuerta;
	MMarcoOroPuerta.diffuse = glm::vec4(0.589f, 0.266f, 0.021f, 1.0f);
	MMarcoOroPuerta.specular = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
	materialSinTexturaUnaLuzShader->setVec4("MaterialAmbientColor", MMarcoOroPuerta.ambient);
	materialSinTexturaUnaLuzShader->setVec4("MaterialDiffuseColor", MMarcoOroPuerta.diffuse);
	materialSinTexturaUnaLuzShader->setVec4("MaterialSpecularColor", MMarcoOroPuerta.specular);
	materialSinTexturaUnaLuzShader->setFloat("transparency", MMarcoOroPuerta.transparency);
	materialSinTexturaUnaLuzShader->setInt("shininess", 2);
	MarcoOroPuerta->Draw(*materialSinTexturaUnaLuzShader);

	// FondoCanons
	Material MFondoCanons;
	MFondoCanons.diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	MFondoCanons.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	materialSinTexturaUnaLuzShader->setVec4("MaterialAmbientColor", MFondoCanons.ambient);
	materialSinTexturaUnaLuzShader->setVec4("MaterialDiffuseColor", MFondoCanons.diffuse);
	materialSinTexturaUnaLuzShader->setVec4("MaterialSpecularColor", MFondoCanons.specular);
	materialSinTexturaUnaLuzShader->setFloat("transparency", MFondoCanons.transparency);
	materialSinTexturaUnaLuzShader->setInt("shininess", 5);
	FondoCanons->Draw(*materialSinTexturaUnaLuzShader);

	// RejillaVentana
	Material MRejillaVentana;
	MRejillaVentana.diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	MRejillaVentana.specular = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
	materialSinTexturaUnaLuzShader->setVec4("MaterialAmbientColor", MRejillaVentana.ambient);
	materialSinTexturaUnaLuzShader->setVec4("MaterialDiffuseColor", MRejillaVentana.diffuse);
	materialSinTexturaUnaLuzShader->setVec4("MaterialSpecularColor", MRejillaVentana.specular);
	materialSinTexturaUnaLuzShader->setFloat("transparency", MRejillaVentana.transparency);
	materialSinTexturaUnaLuzShader->setInt("shininess", 5);
	RejillaVentana->Draw(*materialSinTexturaUnaLuzShader);

	Material MBotonesDorados;
	MBotonesDorados.diffuse = glm::vec4(0.589f, 0.266f, 0.021f, 1.0f);
	MBotonesDorados.specular = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
	materialSinTexturaUnaLuzShader->setVec4("MaterialAmbientColor", MBotonesDorados.ambient);
	materialSinTexturaUnaLuzShader->setVec4("MaterialDiffuseColor", MBotonesDorados.diffuse);
	materialSinTexturaUnaLuzShader->setVec4("MaterialSpecularColor", MBotonesDorados.specular);
	materialSinTexturaUnaLuzShader->setFloat("transparency", MBotonesDorados.transparency);
	materialSinTexturaUnaLuzShader->setInt("shininess", 2);
	BotonesDorados->Draw(*materialSinTexturaUnaLuzShader);

	Material MPropulsor;
	MPropulsor.diffuse = glm::vec4(0.0f, 0.0f, 0.05f, 1.0f);
	MPropulsor.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	materialSinTexturaUnaLuzShader->setVec4("MaterialAmbientColor", MPropulsor.ambient);
	materialSinTexturaUnaLuzShader->setVec4("MaterialDiffuseColor", MPropulsor.diffuse);
	materialSinTexturaUnaLuzShader->setVec4("MaterialSpecularColor", MPropulsor.specular);
	materialSinTexturaUnaLuzShader->setFloat("transparency", MPropulsor.transparency);
	materialSinTexturaUnaLuzShader->setInt("shininess", 5);
	Propulsor->Draw(*materialSinTexturaUnaLuzShader);

	Material MMarcoFarol;
	MMarcoFarol.diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	MMarcoFarol.specular = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	materialSinTexturaUnaLuzShader->setVec4("MaterialAmbientColor", MMarcoFarol.ambient);
	materialSinTexturaUnaLuzShader->setVec4("MaterialDiffuseColor", MMarcoFarol.diffuse);
	materialSinTexturaUnaLuzShader->setVec4("MaterialSpecularColor", MMarcoFarol.specular);
	materialSinTexturaUnaLuzShader->setFloat("transparency", MMarcoFarol.transparency);
	materialSinTexturaUnaLuzShader->setInt("shininess", 10);
	MarcoFarol->Draw(*materialSinTexturaUnaLuzShader);

	Material MMetalMastil;
	MMetalMastil.diffuse = glm::vec4(0.027f, 0.051f, 0.087f, 1.0f);
	MMetalMastil.specular = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
	materialSinTexturaUnaLuzShader->setVec4("MaterialAmbientColor", MMetalMastil.ambient);
	materialSinTexturaUnaLuzShader->setVec4("MaterialDiffuseColor", MMetalMastil.diffuse);
	materialSinTexturaUnaLuzShader->setVec4("MaterialSpecularColor", MMetalMastil.specular);
	materialSinTexturaUnaLuzShader->setFloat("transparency", MMetalMastil.transparency);
	materialSinTexturaUnaLuzShader->setInt("shininess", 5);
	MetalMastil->Draw(*materialSinTexturaUnaLuzShader);

	Material MHojas;
	MHojas.diffuse = glm::vec4(0.052f, 0.448f, 0.017f, 1.0f);
	MHojas.specular = glm::vec4(0.052f, 0.448f, 0.017f, 1.0f);
	materialSinTexturaUnaLuzShader->setVec4("MaterialAmbientColor", MHojas.ambient);
	materialSinTexturaUnaLuzShader->setVec4("MaterialDiffuseColor", MHojas.diffuse);
	materialSinTexturaUnaLuzShader->setVec4("MaterialSpecularColor", MHojas.specular);
	materialSinTexturaUnaLuzShader->setFloat("transparency", MHojas.transparency);
	materialSinTexturaUnaLuzShader->setInt("shininess", 10);
	Hojas->Draw(*materialSinTexturaUnaLuzShader);

	Material MMaterialCadena;
	MMaterialCadena.diffuse = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	MMaterialCadena.specular = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
	materialSinTexturaUnaLuzShader->setVec4("MaterialAmbientColor", MMaterialCadena.ambient);
	materialSinTexturaUnaLuzShader->setVec4("MaterialDiffuseColor", MMaterialCadena.diffuse);
	materialSinTexturaUnaLuzShader->setVec4("MaterialSpecularColor", MMaterialCadena.specular);
	materialSinTexturaUnaLuzShader->setFloat("transparency", MMaterialCadena.transparency);
	materialSinTexturaUnaLuzShader->setInt("shininess", 5);
	MaterialCadena->Draw(*materialSinTexturaUnaLuzShader);

	glUseProgram(0);
}

// Dibujo de materiales sin textura con varias luces direccionales
void MaterialesSinTexturaVariasLuces() {
	// Activamos el shader estático
	materialSinTexturaVariasLucesShader->use();

	// Aplicamos transformaciones de proyección y cámara (si las hubiera)
	materialSinTexturaVariasLucesShader->setMat4("projection", projection);
	materialSinTexturaVariasLucesShader->setMat4("view", view);

	// Aplicamos transformaciones del modelo
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Trasladar al centro de la escena
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// escala para los objetos que sean muy grandes
	materialSinTexturaVariasLucesShader->setMat4("model", model);
	// Propiedades de la luz
	// Luz direccional posx
	materialSinTexturaVariasLucesShader->setVec3("dirLight[0].direction", posx.Direction);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[0].ambient", posx.Ambient);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[0].diffuse", posx.Diffuse);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[0].specular", posx.Specular);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[0].power", posx.Power);
	// Luz direccional negx
	materialSinTexturaVariasLucesShader->setVec3("dirLight[1].direction", negx.Direction);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[1].ambient", negx.Ambient);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[1].diffuse", negx.Diffuse);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[1].specular", negx.Specular);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[1].power", negx.Power);
	// Luz direccional posy
	/*materialSinTexturaVariasLucesShader->setVec3("dirLight[2].direction", posy.Direction);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[2].ambient", posy.Ambient);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[2].diffuse", posy.Diffuse);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[2].specular", posy.Specular);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[2].power", posy.Power);*/
	//// Luz direccional negy
	materialSinTexturaVariasLucesShader->setVec3("dirLight[2].direction", negy.Direction);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[2].ambient", negy.Ambient);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[2].diffuse", negy.Diffuse);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[2].specular", negy.Specular);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[2].power", negy.Power);
	// Luz direccional posz
	materialSinTexturaVariasLucesShader->setVec3("dirLight[3].direction", posz.Direction);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[3].ambient", posz.Ambient);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[3].diffuse", posz.Diffuse);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[3].specular", posz.Specular);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[3].power", posz.Power);
	// Luz direccional negz
	materialSinTexturaVariasLucesShader->setVec3("dirLight[4].direction", negz.Direction);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[4].ambient", negz.Ambient);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[4].diffuse", negz.Diffuse);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[4].specular", negz.Specular);
	materialSinTexturaVariasLucesShader->setVec3("dirLight[4].power", negz.Power);
	// Luz de linterna
	materialSinTexturaVariasLucesShader->setVec3("spotLight.position", camera.Position);
	materialSinTexturaVariasLucesShader->setVec3("spotLight.direction", camera.Front);
	materialSinTexturaVariasLucesShader->setVec3("spotLight.ambient", Lintern.Ambient);
	materialSinTexturaVariasLucesShader->setVec3("spotLight.diffuse", Lintern.Diffuse);
	materialSinTexturaVariasLucesShader->setVec3("spotLight.specular", Lintern.Specular);
	materialSinTexturaVariasLucesShader->setFloat("spotLight.constant", Lintern.Constant);
	materialSinTexturaVariasLucesShader->setFloat("spotLight.linear", Lintern.Linear);
	materialSinTexturaVariasLucesShader->setFloat("spotLight.quadratic", Lintern.Quadratic);
	materialSinTexturaVariasLucesShader->setFloat("spotLight.cutOff", Lintern.CutOff);
	materialSinTexturaVariasLucesShader->setFloat("spotLight.outerCutOff", Lintern.OuterCutOff);

	// Posición de la camara
	materialSinTexturaVariasLucesShader->setVec3("viewPos", camera.Position);

	// Habilitar blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*
	// Piso de prueba
	Material m1;
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", m1.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", m1.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", m1.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", m1.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 10);
	Floor.Draw(materialSinTexturaVariasLucesShader);*/

	// Propiedades materiales del Barco

	// Canons
	Material MCanons;
	MCanons.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	MCanons.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MCanons.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MCanons.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MCanons.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MCanons.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	Canons->Draw(*materialSinTexturaVariasLucesShader);

	// MaderaPuerta
	Material MMaderaPuerta;
	MMaderaPuerta.diffuse = glm::vec4(0.166f, 0.060f, 0.026f, 1.0f);
	MMaderaPuerta.specular = glm::vec4(0.166f, 0.060f, 0.026f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MMaderaPuerta.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MMaderaPuerta.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MMaderaPuerta.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MMaderaPuerta.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 50);
	MaderaPuerta->Draw(*materialSinTexturaVariasLucesShader);

	// MarcoPuertaRojo
	Material MMarcoPuertaRojo;
	MMarcoPuertaRojo.diffuse = glm::vec4(0.563f, 0.008f, 0.026f, 1.0f);
	MMarcoPuertaRojo.specular = glm::vec4(0.563f, 0.008f, 0.026f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MMarcoPuertaRojo.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MMarcoPuertaRojo.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MMarcoPuertaRojo.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MMarcoPuertaRojo.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 20);
	MarcoPuertaRojo->Draw(*materialSinTexturaVariasLucesShader);

	// MarcoPuertaBlanco
	Material MMarcoPuertaBlanco;
	MMarcoPuertaBlanco.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	MMarcoPuertaBlanco.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MMarcoPuertaBlanco.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MMarcoPuertaBlanco.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MMarcoPuertaBlanco.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MMarcoPuertaBlanco.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 20);
	MarcoPuertaBlanco->Draw(*materialSinTexturaVariasLucesShader);

	// CostadosRojos
	Material MCostadosRojos;
	MCostadosRojos.diffuse = glm::vec4(0.397f, 0.006f, 0.003f, 1.0f);
	MCostadosRojos.specular = glm::vec4(0.397f, 0.006f, 0.003f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MCostadosRojos.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MCostadosRojos.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MCostadosRojos.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MCostadosRojos.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 30);
	CostadosRojos->Draw(*materialSinTexturaVariasLucesShader);

	Material MMarcoCostado;
	MMarcoCostado.diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	MMarcoCostado.specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MMarcoCostado.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MMarcoCostado.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MMarcoCostado.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MMarcoCostado.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	MarcoCostado->Draw(*materialSinTexturaVariasLucesShader);

	Material MBarcoBase;
	MBarcoBase.diffuse = glm::vec4(0.078f, 0.049f, 0.033f, 1.0f);
	MBarcoBase.specular = glm::vec4(0.078f, 0.049f, 0.033f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MBarcoBase.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MBarcoBase.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MBarcoBase.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MBarcoBase.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 50);
	BarcoBase->Draw(*materialSinTexturaVariasLucesShader);

	Material MParedes1;
	MParedes1.diffuse = glm::vec4(0.665f, 0.552f, 0.361f, 1.0f);
	MParedes1.specular = glm::vec4(0.665f, 0.552f, 0.361f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MParedes1.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MParedes1.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MParedes1.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MParedes1.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 30);
	Paredes1->Draw(*materialSinTexturaVariasLucesShader);

	Material MCintaInferior;
	MCintaInferior.diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	MCintaInferior.specular = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MCintaInferior.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MCintaInferior.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MCintaInferior.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MCintaInferior.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 10);
	CintaInferior->Draw(*materialSinTexturaVariasLucesShader);

	Material MBandaSuperior;
	MBandaSuperior.diffuse = glm::vec4(0.397f, 0.002f, 0.0f, 1.0f);
	MBandaSuperior.specular = glm::vec4(0.497f, 0.102f, 0.1f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MBandaSuperior.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MBandaSuperior.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MBandaSuperior.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MBandaSuperior.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 10);
	BandaSuperior->Draw(*materialSinTexturaVariasLucesShader);

	Material MTechoParedSuperior;
	MTechoParedSuperior.diffuse = glm::vec4(0.52f, 0.004f, 0.0f, 1.0f);
	MTechoParedSuperior.specular = glm::vec4(0.52f, 0.004f, 0.0f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MTechoParedSuperior.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MTechoParedSuperior.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MTechoParedSuperior.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MTechoParedSuperior.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 30);
	TechoParedSuperior->Draw(*materialSinTexturaVariasLucesShader);

	Material MAmarilloTecho;
	MAmarilloTecho.diffuse = glm::vec4(0.8f, 0.793f, 0.035f, 1.0f);
	MAmarilloTecho.specular = glm::vec4(0.8f, 0.793f, 0.035f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MAmarilloTecho.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MAmarilloTecho.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MAmarilloTecho.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MAmarilloTecho.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 30);
	AmarilloTecho->Draw(*materialSinTexturaVariasLucesShader);

	Material MMarcoVentana;
	MMarcoVentana.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	MMarcoVentana.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MMarcoVentana.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MMarcoVentana.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MMarcoVentana.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MMarcoVentana.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 30);
	MarcoVentana->Draw(*materialSinTexturaVariasLucesShader);

	Material MBaseBandera;
	MBaseBandera.diffuse = glm::vec4(0.665f, 0.552f, 0.361f, 1.0f);
	MBaseBandera.specular = glm::vec4(0.665f, 0.552f, 0.361f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MBaseBandera.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MBaseBandera.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MBaseBandera.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MBaseBandera.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 30);
	BaseBandera->Draw(*materialSinTexturaVariasLucesShader);

	Material MVelas;
	MVelas.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	MVelas.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MVelas.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MVelas.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MVelas.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MVelas.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 10);
	Velas->Draw(*materialSinTexturaVariasLucesShader);

	Material MBanderaRoja;
	MBanderaRoja.diffuse = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);
	MBanderaRoja.specular = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MBanderaRoja.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MBanderaRoja.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MBanderaRoja.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MBanderaRoja.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 10);
	BanderaRoja->Draw(*materialSinTexturaVariasLucesShader);

	Material MBaseCuartoMastil;
	MBaseCuartoMastil.diffuse = glm::vec4(0.212f, 0.262f, 0.266f, 1.0f);
	MBaseCuartoMastil.specular = glm::vec4(0.212f, 0.262f, 0.266f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MBaseCuartoMastil.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MBaseCuartoMastil.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MBaseCuartoMastil.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MBaseCuartoMastil.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 30);
	BaseCuartoMastil->Draw(*materialSinTexturaVariasLucesShader);

	Material MFondoCara;
	MFondoCara.diffuse = glm::vec4(0.8f, 0.477f, 0.15f, 1.0f);
	MFondoCara.specular = glm::vec4(0.8f, 0.477f, 0.15f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MFondoCara.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MFondoCara.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MFondoCara.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MFondoCara.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 30);
	FondoCara->Draw(*materialSinTexturaVariasLucesShader);

	Material MHuesos;
	MHuesos.diffuse = glm::vec4(0.955f, 0.955f, 0.955f, 1.0f);
	MHuesos.specular = glm::vec4(0.955f, 0.955f, 0.955f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MHuesos.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MHuesos.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MHuesos.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MHuesos.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 20);
	Huesos->Draw(*materialSinTexturaVariasLucesShader);

	Material MFondoOjos;
	MFondoOjos.diffuse = glm::vec4(0.955f, 0.955f, 0.955f, 1.0f);
	MFondoOjos.specular = glm::vec4(0.955f, 0.955f, 0.955f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MFondoOjos.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MFondoOjos.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MFondoOjos.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MFondoOjos.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 20);
	FondoOjos->Draw(*materialSinTexturaVariasLucesShader);

	Material MMelena;
	MMelena.diffuse = glm::vec4(0.8f, 0.037f, 0.021f, 1.0f);
	MMelena.specular = glm::vec4(0.8f, 0.037f, 0.021f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MMelena.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MMelena.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MMelena.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MMelena.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 20);
	Melena->Draw(*materialSinTexturaVariasLucesShader);

	// Luz direccional posx
	materialSinTexturaVariasLucesShader->setVec3("dirLight[3].direction", glm::vec3(0.0f));
	materialSinTexturaVariasLucesShader->setVec3("dirLight[3].ambient", glm::vec3(0.0f));
	materialSinTexturaVariasLucesShader->setVec3("dirLight[3].diffuse", glm::vec3(0.0f));
	materialSinTexturaVariasLucesShader->setVec3("dirLight[3].specular", glm::vec3(0.0f));
	materialSinTexturaVariasLucesShader->setVec3("dirLight[3].power", glm::vec3(0.0f));
	// Luz direccional negx
	materialSinTexturaVariasLucesShader->setVec3("dirLight[4].direction", glm::vec3(0.0f));
	materialSinTexturaVariasLucesShader->setVec3("dirLight[4].ambient", glm::vec3(0.0f));
	materialSinTexturaVariasLucesShader->setVec3("dirLight[4].diffuse", glm::vec3(0.0f));
	materialSinTexturaVariasLucesShader->setVec3("dirLight[4].specular", glm::vec3(0.0f));
	materialSinTexturaVariasLucesShader->setVec3("dirLight[4].power", glm::vec3(0.0f));

	// BarandalesYLaterales
	Material MBarandalesYLaterales;
	MBarandalesYLaterales.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	MBarandalesYLaterales.specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MBarandalesYLaterales.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MBarandalesYLaterales.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MBarandalesYLaterales.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MBarandalesYLaterales.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 10);
	BarandalesYLaterales->Draw(*materialSinTexturaVariasLucesShader);

	// cubo de luz
	/*glm::mat4 cuboLuzM = glm::mat4(1.0f);
	cuboLuzM = glm::translate(cuboLuzM, LuzPosition);
	materialSinTexturaVariasLucesShader->setMat4("model", cuboLuzM);
	Material luzp;
	luzp.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	luzp.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	luzp.specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", luzp.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", luzp.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", luzp.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", luzp.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 10);
	cube.Draw(materialSinTexturaVariasLucesShader);*/

	glUseProgram(0);
}

// Dibujo de materiales con textura con varias luces direccionales
void MaterialesConTexturasVariasLuces() {
	// Activamos el shader estático
	materialConTexturaVariasLucesShader->use();

	// Aplicamos transformaciones de proyección y cámara (si las hubiera)
	materialConTexturaVariasLucesShader->setMat4("projection", projection);
	materialConTexturaVariasLucesShader->setMat4("view", view);

	// Aplicamos transformaciones del modelo
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Trasladar al centro de la escena
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// escala para los objetos que sean muy grandes
	materialConTexturaVariasLucesShader->setMat4("model", model);
	// Propiedades de la luz
	// Luz direccional posx
	materialConTexturaVariasLucesShader->setVec3("dirLight[0].direction", posx.Direction);
	materialConTexturaVariasLucesShader->setVec3("dirLight[0].ambient", posx.Ambient);
	materialConTexturaVariasLucesShader->setVec3("dirLight[0].diffuse", posx.Diffuse);
	materialConTexturaVariasLucesShader->setVec3("dirLight[0].specular", posx.Specular);
	materialConTexturaVariasLucesShader->setVec3("dirLight[0].power", posx.Power);
	// Luz direccional negx
	materialConTexturaVariasLucesShader->setVec3("dirLight[1].direction", negx.Direction);
	materialConTexturaVariasLucesShader->setVec3("dirLight[1].ambient", negx.Ambient);
	materialConTexturaVariasLucesShader->setVec3("dirLight[1].diffuse", negx.Diffuse);
	materialConTexturaVariasLucesShader->setVec3("dirLight[1].specular", negx.Specular);
	materialConTexturaVariasLucesShader->setVec3("dirLight[1].power", negx.Power);
	// Luz direccional posy
	/*materialConTexturaVariasLucesShader->setVec3("dirLight[2].direction", posy.Direction);
	materialConTexturaVariasLucesShader->setVec3("dirLight[2].ambient", posy.Ambient);
	materialConTexturaVariasLucesShader->setVec3("dirLight[2].diffuse", posy.Diffuse);
	materialConTexturaVariasLucesShader->setVec3("dirLight[2].specular", posy.Specular);
	materialConTexturaVariasLucesShader->setVec3("dirLight[2].power", posy.Power);*/
	//// Luz direccional negy
	materialConTexturaVariasLucesShader->setVec3("dirLight[2].direction", negy.Direction);
	materialConTexturaVariasLucesShader->setVec3("dirLight[2].ambient", negy.Ambient);
	materialConTexturaVariasLucesShader->setVec3("dirLight[2].diffuse", negy.Diffuse);
	materialConTexturaVariasLucesShader->setVec3("dirLight[2].specular", negy.Specular);
	materialConTexturaVariasLucesShader->setVec3("dirLight[2].power", negy.Power);
	// Luz direccional posz
	materialConTexturaVariasLucesShader->setVec3("dirLight[3].direction", posz.Direction);
	materialConTexturaVariasLucesShader->setVec3("dirLight[3].ambient", posz.Ambient);
	materialConTexturaVariasLucesShader->setVec3("dirLight[3].diffuse", posz.Diffuse);
	materialConTexturaVariasLucesShader->setVec3("dirLight[3].specular", posz.Specular);
	materialConTexturaVariasLucesShader->setVec3("dirLight[3].power", posz.Power);
	// Luz direccional negz
	materialConTexturaVariasLucesShader->setVec3("dirLight[4].direction", negz.Direction);
	materialConTexturaVariasLucesShader->setVec3("dirLight[4].ambient", negz.Ambient);
	materialConTexturaVariasLucesShader->setVec3("dirLight[4].diffuse", negz.Diffuse);
	materialConTexturaVariasLucesShader->setVec3("dirLight[4].specular", negz.Specular);
	materialConTexturaVariasLucesShader->setVec3("dirLight[4].power", negz.Power);
	// Luz de linterna
	materialConTexturaVariasLucesShader->setVec3("spotLight.position", camera.Position);
	materialConTexturaVariasLucesShader->setVec3("spotLight.direction", camera.Front);
	materialConTexturaVariasLucesShader->setVec3("spotLight.ambient", Lintern.Ambient);
	materialConTexturaVariasLucesShader->setVec3("spotLight.diffuse", Lintern.Diffuse);
	materialConTexturaVariasLucesShader->setVec3("spotLight.specular", Lintern.Specular);
	materialConTexturaVariasLucesShader->setFloat("spotLight.constant", Lintern.Constant);
	materialConTexturaVariasLucesShader->setFloat("spotLight.linear", Lintern.Linear);
	materialConTexturaVariasLucesShader->setFloat("spotLight.quadratic", Lintern.Quadratic);
	materialConTexturaVariasLucesShader->setFloat("spotLight.cutOff", Lintern.CutOff);
	materialConTexturaVariasLucesShader->setFloat("spotLight.outerCutOff", Lintern.OuterCutOff);

	// Posición de la camara
	materialConTexturaVariasLucesShader->setVec3("viewPos", camera.Position);

	// Habilitar blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Material MPasto;
	//MPasto.diffuse = glm::vec4(0.016f, 0.336f, 0.0f, 1.0f);
	//MPasto.specular = glm::vec4(0.016f, 0.336f, 0.0f, 1.0f);
	//materialConTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MPasto.ambient);
	//materialConTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MPasto.diffuse);
	//materialConTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MPasto.specular);
	//materialConTexturaVariasLucesShader->setFloat("transparency", MPasto.transparency);
	//materialConTexturaVariasLucesShader->setInt("shininess", 10);
	//Pasto->Draw(*materialConTexturaVariasLucesShader);

	//Material MPisoMadera; //***************************************************************************************** checar
	//MPisoMadera.diffuse = glm::vec4(0.119f, 0.029f, 0.06f, 1.0f);
	//MPisoMadera.specular = glm::vec4(0.119f, 0.029f, 0.06f, 1.0f);
	//materialConTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MPisoMadera.ambient);
	//materialConTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MPisoMadera.diffuse);
	//materialConTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MPisoMadera.specular);
	//materialConTexturaVariasLucesShader->setFloat("transparency", MPisoMadera.transparency);
	//materialConTexturaVariasLucesShader->setInt("shininess", 50);
	//PisoMadera->Draw(*materialConTexturaVariasLucesShader);

	//Material MMaderaArbol;
	//MMaderaArbol.diffuse = glm::vec4(0.05f, 0.012f, 0.003f, 1.0f);
	//MMaderaArbol.specular = glm::vec4(0.05f, 0.012f, 0.003f, 1.0f);
	//materialConTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MMaderaArbol.ambient);
	//materialConTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MMaderaArbol.diffuse);
	//materialConTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MMaderaArbol.specular);
	//materialConTexturaVariasLucesShader->setFloat("transparency", MMaderaArbol.transparency);
	//materialConTexturaVariasLucesShader->setInt("shininess", 30);
	//MaderaArbol->Draw(*materialConTexturaVariasLucesShader);

	//Material MBarcoBaseTextura;
	//MBarcoBaseTextura.diffuse = glm::vec4(0.078f, 0.049f, 0.033f, 1.0f);
	//MBarcoBaseTextura.specular = glm::vec4(0.078f, 0.049f, 0.033f, 1.0f);
	//materialConTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MBarcoBaseTextura.ambient);
	//materialConTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MBarcoBaseTextura.diffuse);
	//materialConTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MBarcoBaseTextura.specular);
	//materialConTexturaVariasLucesShader->setFloat("transparency", MBarcoBaseTextura.transparency);
	//materialConTexturaVariasLucesShader->setInt("shininess", 50);
	//BarcoBaseTextura->Draw(*materialConTexturaVariasLucesShader);

	Material MBarcoCostado;
	MBarcoCostado.diffuse = glm::vec4(0.078f, 0.049f, 0.033f, 1.0f);
	MBarcoCostado.specular = glm::vec4(0.078f, 0.049f, 0.033f, 1.0f);
	materialConTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MBarcoCostado.ambient);
	materialConTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MBarcoCostado.diffuse);
	materialConTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MBarcoCostado.specular);
	materialConTexturaVariasLucesShader->setFloat("transparency", MBarcoCostado.transparency);
	materialConTexturaVariasLucesShader->setInt("shininess", 50);
	BarcoCostado->Draw(*materialConTexturaVariasLucesShader);

	glUseProgram(0);
}

// Dibujo de materiales sin textura con reflejos
void MaterialesSinTexturaFresnel() {
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

// Dibujo de los modelos en la escena, transformaciones
bool Update() {

	// Cálculo del framerate
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	elapsedTime += deltaTime;

	elapsedTime += deltaTime;
	if (elapsedTime > 1.0f / fps) {
		animationCount++;
		if (animationCount > keys - 1) {
			animationCount = 0;
		}
		// Configuración de la pose en el instante t
		Character->SetPose((float)animationCount, gBones);
		elapsedTime = 0.0f;
	}

	// Procesa la entrada del teclado o mouse
	processInput(window);

	// Renderizado R - G - B - A
	glClearColor(bgR, bgG, bgB, bgA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Cambio de propiedades de la Luz
	// Luz direccional
	posx.Direction = glm::vec3(1.0f, 0.0f, 0.0f);
	negx.Direction = glm::vec3(-1.0f, 0.0f, 0.0f);
	posy.Direction = glm::vec3(0.0f, 1.0f, 0.0f);
	negy.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
	posz.Direction = glm::vec3(0.0f, 0.0f, 1.0f);
	negz.Direction = glm::vec3(0.0f, 0.0f, -1.0f);
	sol.setPower(3.0f);
	//sol.Direction = glm::vec3(-0.5f, -0.86f, 0.0f);

	// Puntos de luz
	L0.setPosition(38.4f, -0.6f, -22.9f);
	L1.setPosition(-28.40f, -0.6f, -22.9f);
	L2.setPosition(0.0f, -0.6, 0.0f);
	L3.setPosition(0.0f, 0.0f, -65.90f);

	// Proyecciones y vistas
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// Materiales Estáticos
	//		Materiales sin textura

	//			Materiales con una sola luz direccional (metales y hojas)
	//MaterialesSinTexturaUnaLuz();

	//			Materiales con 6 luces posicionales
	//MaterialesSinTexturaVariasLuces();

	//			Objetos Sin textura con Iluminación de Fresnel
	//MaterialesSinTexturaFresnel();

	//		Materiales Con textura
	//			Materiales con varias luces direccionales
	MaterialesConTexturasVariasLuces();

	// Puntos de luz
	/*
	// point light 1
	materialSinTexturaVariasLucesShader->setVec3("pointLights[0].position", L0.Position);
	materialSinTexturaVariasLucesShader->setVec3("pointLights[0].ambient", L0.Ambient);
	materialSinTexturaVariasLucesShader->setVec3("pointLights[0].diffuse", L0.Diffuse);
	materialSinTexturaVariasLucesShader->setVec3("pointLights[0].specular", L0.Specular);
	materialSinTexturaVariasLucesShader->setFloat("pointLights[0].constant", L0.Constant);
	materialSinTexturaVariasLucesShader->setFloat("pointLights[0].linear", L0.Linear);
	materialSinTexturaVariasLucesShader->setFloat("pointLights[0].quadratic", L0.Quadratic);
	materialSinTexturaVariasLucesShader->setVec3("pointLights[0].power", L0.Power);
	
	// point light 2
	materialSinTexturaVariasLucesShader->setVec3("pointLights[1].position", L1.Position);
	materialSinTexturaVariasLucesShader->setVec3("pointLights[1].ambient", L1.Ambient);
	materialSinTexturaVariasLucesShader->setVec3("pointLights[1].diffuse", L1.Diffuse);
	materialSinTexturaVariasLucesShader->setVec3("pointLights[1].specular", L1.Specular);
	materialSinTexturaVariasLucesShader->setFloat("pointLights[1].constant", L1.Constant);
	materialSinTexturaVariasLucesShader->setFloat("pointLights[1].linear", L1.Linear);
	materialSinTexturaVariasLucesShader->setFloat("pointLights[1].quadratic", L1.Quadratic);
	materialSinTexturaVariasLucesShader->setVec3("pointLights[1].power", L1.Power);

	// point light 3
	materialSinTexturaVariasLucesShader->setVec3("pointLights[2].position", L2.Position);
	materialSinTexturaVariasLucesShader->setVec3("pointLights[2].ambient", L2.Ambient);
	materialSinTexturaVariasLucesShader->setVec3("pointLights[2].diffuse", L2.Diffuse);
	materialSinTexturaVariasLucesShader->setVec3("pointLights[2].specular", L2.Specular);
	materialSinTexturaVariasLucesShader->setFloat("pointLights[2].constant", L2.Constant);
	materialSinTexturaVariasLucesShader->setFloat("pointLights[2].linear", L2.Linear);
	materialSinTexturaVariasLucesShader->setFloat("pointLights[2].quadratic", L2.Quadratic);
	materialSinTexturaVariasLucesShader->setVec3("pointLights[2].power", L2.Power);

	// point light 4
	materialSinTexturaVariasLucesShader->setVec3("pointLights[3].position", L3.Position);
	materialSinTexturaVariasLucesShader->setVec3("pointLights[3].ambient", L3.Ambient);
	materialSinTexturaVariasLucesShader->setVec3("pointLights[3].diffuse", L3.Diffuse);
	materialSinTexturaVariasLucesShader->setVec3("pointLights[3].specular", L3.Specular);
	materialSinTexturaVariasLucesShader->setFloat("pointLights[3].constant", L3.Constant);
	materialSinTexturaVariasLucesShader->setFloat("pointLights[3].linear", L3.Linear);
	materialSinTexturaVariasLucesShader->setFloat("pointLights[3].quadratic", L3.Quadratic);
	materialSinTexturaVariasLucesShader->setVec3("pointLights[3].power", L3.Power);
	*/

	// Materiales Animados
	// 	   Materiales con textura
	
	//			Materiales con textura animados phong
	/*{
		// Activamos el shader dinámico
		animados->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		animados->setMat4("projection", projection);
		animados->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Pasar al centro de la escena
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// Es muy grande el cubo
		animados->setMat4("model", model);

		animados->setMat4("gBones", MAX_RIGGING_BONES, gBones);

		// Propiedades de la luz

		// Luz direccional
		animados->setVec3("dirLight.direction", sol.Direction);
		animados->setVec3("dirLight.ambient", sol.Ambient);
		animados->setVec3("dirLight.diffuse", sol.Diffuse);
		animados->setVec3("dirLight.specular", sol.Specular);
		animados->setVec3("dirLight.power", sol.Power);

		// Luz de linterna
		animados->setVec3("spotLight.position", camera.Position);
		animados->setVec3("spotLight.direction", camera.Front);
		animados->setVec3("spotLight.ambient", Lintern.Ambient);
		animados->setVec3("spotLight.diffuse", Lintern.Diffuse);
		animados->setVec3("spotLight.specular", Lintern.Specular);
		animados->setFloat("spotLight.constant", Lintern.Constant);
		animados->setFloat("spotLight.linear", Lintern.Linear);
		animados->setFloat("spotLight.quadratic", Lintern.Quadratic);
		animados->setFloat("spotLight.cutOff", Lintern.CutOff);
		animados->setFloat("spotLight.outerCutOff", Lintern.OuterCutOff);

		// Posición de la camara
		animados->setVec3("viewPos", camera.Position);

		// Enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Fondo
		Material MFondo1;
		animados->setVec4("MaterialAmbientColor", MFondo1.ambient);
		animados->setVec4("MaterialDiffuseColor", MFondo1.diffuse);
		animados->setVec4("MaterialSpecularColor", MFondo1.specular);
		animados->setFloat("transparency", MFondo1.transparency);
		animados->setInt("shininess", 1);
		Character->Draw(*animados);

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


	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
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
