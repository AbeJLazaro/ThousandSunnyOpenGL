/*
	Proyecto Final para la Materia de Computación Gráfica e Interacción Humano-Computadora
	Autores:		Lázaro Martínez Abraham Josué
					Ian
					Apolo
	Fecha:			22/07/2021
	Versión:		1.2

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
void MaterialesSinTexturaCocina();
void MaterialesSinTexturaFresnel();
void MaterialesConTexturasVariasLuces();
void MaterialesAnimadosVariasLuces();
void DibujaFondo();
unsigned int loadCubemap(vector<std::string> faces);
void Limpiar();

// Definición de callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Tamaño en pixeles de la ventana
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// Definición de cámara (posición en XYZ)
Camera camera(glm::vec3(0.0f, 20.0f, 70.0f));

// Definición de proyeccion y vista
// Proyección y vista
glm::mat4 projection;
glm::mat4 view;

// Controladores para el movimiento del mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

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
// Modelos del barco
// phong
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
Model* Timon;
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
// Fresnel
Model* Vidrio;
Model* VentanaPuertaSinMarco;
// Modelos Cocina 
// Phong
Model* BarraCuerpo;
Model* BarraMesa;
Model* BarraPiso;
Model* BaseSilla;
Model* Campana1;
Model* Campana2;
Model* CampanaFondo;
Model* CampanaTope;
Model* EstufaCuerpo;
Model* EstufaIntegral;
Model* EstufaIntegralMarco;
Model* EstufaMesa;
Model* EstufaPiso;
Model* Extractor;
Model* ExtractorFondo;
Model* ExtractorMarco;
Model* MangoEstufa;
Model* Mantel;
Model* Mesa;
Model* Mesita;
Model* ParedesCocina;
Model* ParedMadera;
Model* PisoAjedrez;
Model* PisoMadera1;
Model* Refri;
Model* RefriMarco;
Model* Sarten;
Model* Silla;
Model* SillaInterior;
Model* Soffa1;
Model* Soffa2;
Model* TechoCocina;
Model* TransicionTechoCocina;
Model* Tronco;
Model* TroncoSaliente;
Model* VentanaEstufa;
// Modelo del fondo
Model* Fondo;

// Animaciones
Model* Zoro;
Model* Robin;
// Máximo número de huesos: 100
#define MAX_RIGGING_BONES 100

// Pose inicial de las animaciones
glm::mat4 ZoroBones[MAX_RIGGING_BONES];
glm::mat4 RobinBones[MAX_RIGGING_BONES];

float fps = 0;
int keys = 0;
int animationCount = 0;
int animationTypeZoro = 0;
int animationTypeRobin = 0;
// Variables para la velocidad de reproducción
// de la animación
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsedTime = 0.0f;

// Variables para el control de las animaciones

glm::vec3 positionZoro(0.0f, 0.0f, 0.0f);
glm::vec3 forwardViewZoro(1.0f, 0.0f, 0.0f);
float     scaleVZoro = 0.005f;
float     rotateCharacterZoro = 0.0f;

glm::vec3 positionRobin(0.0f, 0.0f, 0.0f);
glm::vec3 forwardViewRobin(1.0f, 0.0f, 0.0f);
float     scaleVRobin = 0.005f;
float     rotateCharacterRobin = 0.0f;

// Variables para mover el timon
float timonRotation = 0.0f;
float escalaTimon = 1.0f;

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
	Limpiar();
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	Zoro->SetPose(0.0f, ZoroBones, animationTypeZoro);
	fps = (float)Zoro->getFramerate();
	keys = (int)Zoro->getNumFrames();
	Robin->SetPose(0.0f, RobinBones, animationTypeRobin);
	fps = (float)Robin->getFramerate();
	keys = (int)Robin->getNumFrames();
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
	animados = new Shader("shaders/animados.vs", "shaders/animados.fs");
	if (animados == nullptr) {
		cout << "Error creating shaders." << endl;
		return false;
	}
	fondoShader = new Shader("shaders/fondo.vs", "shaders/fondo.fs");
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
		Timon = new Model("models/Sunny/Timon.fbx");
		if (Timon == nullptr) {
			cout << "Error loading Timon Model." << endl;
			return false;
		}
		BarcoBaseTextura = new Model("models/Sunny/BarcoBaseTextura.fbx");
		if (BarcoBaseTextura == nullptr) {
			cout << "Error loading BarcoBaseTextura Model." << endl;
			return false;
		}
		BarcoCostado = new Model("models/Sunny/BarcoCostado.fbx");
		if (BarcoCostado == nullptr) {
			cout << "Error loading BarcoCostado Model." << endl;
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
	BotonesDorados = new Model("models/Sunny/BotonesDorados.fbx");
	if (BotonesDorados == nullptr) {
		cout << "Error loading BotonesDorados Model." << endl;
		return false;
	}
	
	// Materiales Cocina phong
	{
		BarraCuerpo = new Model("models/Sunny/BarraCuerpo.fbx");
		if (BarraCuerpo == nullptr) {
			cout << "Error loading BarraCuerpo Model." << endl;
			return false;
		}
		BarraMesa = new Model("models/Sunny/BarraMesa.fbx");
		if (BarraMesa == nullptr) {
			cout << "Error loading BarraMesa Model." << endl;
			return false;
		}
		BarraPiso = new Model("models/Sunny/BarraPiso.fbx");
		if (BarraPiso == nullptr) {
			cout << "Error loading BarraPiso Model." << endl;
			return false;
		}
		BaseSilla = new Model("models/Sunny/BaseSilla.fbx");
		if (BaseSilla == nullptr) {
			cout << "Error loading BaseSilla Model." << endl;
			return false;
		}
		Campana1 = new Model("models/Sunny/Campana1.fbx");
		if (Campana1 == nullptr) {
			cout << "Error loading Campana1 Model." << endl;
			return false;
		}
		Campana2 = new Model("models/Sunny/Campana2.fbx");
		if (Campana2 == nullptr) {
			cout << "Error loading Campana2 Model." << endl;
			return false;
		}
		CampanaFondo = new Model("models/Sunny/CampanaFondo.fbx");
		if (CampanaFondo == nullptr) {
			cout << "Error loading CampanaFondo Model." << endl;
			return false;
		}
		CampanaTope = new Model("models/Sunny/CampanaTope.fbx");
		if (CampanaTope == nullptr) {
			cout << "Error loading CampanaTope Model." << endl;
			return false;
		}
		EstufaCuerpo = new Model("models/Sunny/EstufaCuerpo.fbx");
		if (EstufaCuerpo == nullptr) {
			cout << "Error loading EstufaCuerpo Model." << endl;
			return false;
		}
		EstufaIntegral = new Model("models/Sunny/EstufaIntegral.fbx");
		if (EstufaIntegral == nullptr) {
			cout << "Error loading EstufaIntegral Model." << endl;
			return false;
		}
		EstufaIntegralMarco = new Model("models/Sunny/EstufaIntegralMarco.fbx");
		if (EstufaIntegralMarco == nullptr) {
			cout << "Error loading EstufaIntegralMarco Model." << endl;
			return false;
		}
		EstufaMesa = new Model("models/Sunny/EstufaMesa.fbx");
		if (EstufaMesa == nullptr) {
			cout << "Error loading EstufaMesa Model." << endl;
			return false;
		}
		EstufaPiso = new Model("models/Sunny/EstufaPiso.fbx");
		if (EstufaPiso == nullptr) {
			cout << "Error loading EstufaPiso Model." << endl;
			return false;
		}
		Extractor = new Model("models/Sunny/Extractor.fbx");
		if (Extractor == nullptr) {
			cout << "Error loading Extractor Model." << endl;
			return false;
		}
		ExtractorFondo = new Model("models/Sunny/ExtractorFondo.fbx");
		if (ExtractorFondo == nullptr) {
			cout << "Error loading ExtractorFondo Model." << endl;
			return false;
		}
		ExtractorMarco = new Model("models/Sunny/ExtractorMarco.fbx");
		if (ExtractorMarco == nullptr) {
			cout << "Error loading ExtractorMarco Model." << endl;
			return false;
		}
		MangoEstufa = new Model("models/Sunny/MangoEstufa.fbx");
		if (MangoEstufa == nullptr) {
			cout << "Error loading MangoEstufa Model." << endl;
			return false;
		}
		Mantel = new Model("models/Sunny/Mantel.fbx");
		if (Mantel == nullptr) {
			cout << "Error loading Mantel Model." << endl;
			return false;
		}
		Mesa = new Model("models/Sunny/Mesa.fbx");
		if (Mesa == nullptr) {
			cout << "Error loading Mesa Model." << endl;
			return false;
		}
		Mesita = new Model("models/Sunny/Mesita.fbx");
		if (Mesita == nullptr) {
			cout << "Error loading Mesita Model." << endl;
			return false;
		}
		ParedesCocina = new Model("models/Sunny/ParedesCocina.fbx");
		if (ParedesCocina == nullptr) {
			cout << "Error loading ParedesCocina Model." << endl;
			return false;
		}
		ParedMadera = new Model("models/Sunny/ParedMadera.fbx");
		if (ParedMadera == nullptr) {
			cout << "Error loading ParedMadera Model." << endl;
			return false;
		}
		PisoAjedrez = new Model("models/Sunny/PisoAjedrez.fbx");
		if (PisoAjedrez == nullptr) {
			cout << "Error loading PisoAjedrez Model." << endl;
			return false;
		}
		PisoMadera1 = new Model("models/Sunny/PisoMadera1.fbx");
		if (PisoMadera1 == nullptr) {
			cout << "Error loading PisoMadera1 Model." << endl;
			return false;
		}
		Refri = new Model("models/Sunny/Refri.fbx");
		if (Refri == nullptr) {
			cout << "Error loading Refri Model." << endl;
			return false;
		}
		RefriMarco = new Model("models/Sunny/RefriMarco.fbx");
		if (RefriMarco == nullptr) {
			cout << "Error loading RefriMarco Model." << endl;
			return false;
		}
		Sarten = new Model("models/Sunny/Sarten.fbx");
		if (Sarten == nullptr) {
			cout << "Error loading Sarten Model." << endl;
			return false;
		}
		Silla = new Model("models/Sunny/Silla.fbx");
		if (Silla == nullptr) {
			cout << "Error loading Silla Model." << endl;
			return false;
		}
		SillaInterior = new Model("models/Sunny/SillaInterior.fbx");
		if (SillaInterior == nullptr) {
			cout << "Error loading SillaInterior Model." << endl;
			return false;
		}
		Soffa1 = new Model("models/Sunny/Soffa1.fbx");
		if (Soffa1 == nullptr) {
			cout << "Error loading Soffa1 Model." << endl;
			return false;
		}
		Soffa2 = new Model("models/Sunny/Soffa2.fbx");
		if (Soffa2 == nullptr) {
			cout << "Error loading Soffa2 Model." << endl;
			return false;
		}
		TechoCocina = new Model("models/Sunny/TechoCocina.fbx");
		if (TechoCocina == nullptr) {
			cout << "Error loading TechoCocina Model." << endl;
			return false;
		}
		TransicionTechoCocina = new Model("models/Sunny/TransicionTechoCocina.fbx");
		if (TransicionTechoCocina == nullptr) {
			cout << "Error loading TransicionTechoCocina Model." << endl;
			return false;
		}
		Tronco = new Model("models/Sunny/Tronco.fbx");
		if (Tronco == nullptr) {
			cout << "Error loading Tronco Model." << endl;
			return false;
		}
		TroncoSaliente = new Model("models/Sunny/TroncoSaliente.fbx");
		if (TroncoSaliente == nullptr) {
			cout << "Error loading TroncoSaliente Model." << endl;
			return false;
		}
		VentanaEstufa = new Model("models/Sunny/VentanaEstufa.fbx");
		if (VentanaEstufa == nullptr) {
			cout << "Error loading VentanaEstufa Model." << endl;
			return false;
		}
	}
	
	
	// Objetos animados
	Zoro = new Model("models/Sunny/Animaciones/Zoro.fbx");
	if (Zoro == nullptr) {
		cout << "Error loading Zoro Model." << endl;
		return false;
	}
	Robin = new Model("models/Sunny/Animaciones/Robin.fbx");
	if (Robin == nullptr) {
		cout << "Error loading Robin Model." << endl;
		return false;
	}

	// Fondo Cubo 
	Fondo = new Model("models/Sunny/CubeMap.fbx");
	if (Fondo == nullptr) {
		cout << "Error loading Fondo Model." << endl;
		return false;
	}

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
	// Luz direccional negy
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

	glm::mat4 modelTimon = glm::mat4(1.0f);
	modelTimon = glm::translate(modelTimon, glm::vec3(0.367935f, 24.7927f, 26.7368f));
	modelTimon = glm::rotate(modelTimon, glm::radians(glm::radians(timonRotation)), glm::vec3(0.0f, 0.0f, 1.0f));
	modelTimon = glm::rotate(modelTimon, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelTimon = glm::scale(modelTimon, glm::vec3(escalaTimon,escalaTimon, escalaTimon));	// escala para los objetos que sean muy grandes
	materialSinTexturaVariasLucesShader->setMat4("model", modelTimon);

	Material MTimon;
	MTimon.diffuse = glm::vec4(0.078f, 0.049f, 0.033f, 1.0f);
	MTimon.specular = glm::vec4(0.078f, 0.049f, 0.033f, 1.0f);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MTimon.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MTimon.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MTimon.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MTimon.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 50);
	Timon->Draw(*materialSinTexturaVariasLucesShader);

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
	materialSinTexturaVariasLucesShader->setMat4("model", model);

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

	glUseProgram(0);
}

// Dibujo de materiales sin textura con varias luces direccionales en la cocina
void MaterialesSinTexturaCocina() {
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
	// Luz direccional negy
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

	// Propiedades materiales del Barco
	//BarraCuerpo
	Material MBarraCuerpo;
	MBarraCuerpo.diffuse = glm::vec4(0.512, 0.186, 0.091, 1.000);
	MBarraCuerpo.specular = glm::vec4(0.512, 0.186, 0.091, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MBarraCuerpo.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MBarraCuerpo.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MBarraCuerpo.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MBarraCuerpo.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	BarraCuerpo->Draw(*materialSinTexturaVariasLucesShader);

	//BarraMesa
	Material MBarraMesa;
	MBarraMesa.diffuse = glm::vec4(0.800, 0.800, 0.800, 1.000);
	MBarraMesa.specular = glm::vec4(0.800, 0.800, 0.800, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MBarraMesa.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MBarraMesa.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MBarraMesa.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MBarraMesa.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	BarraMesa->Draw(*materialSinTexturaVariasLucesShader);

	//BarraPiso
	Material MBarraPiso;
	MBarraPiso.diffuse = glm::vec4(0.356, 0.356, 0.356, 1.000);
	MBarraPiso.specular = glm::vec4(0.356, 0.356, 0.356, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MBarraPiso.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MBarraPiso.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MBarraPiso.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MBarraPiso.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	BarraPiso->Draw(*materialSinTexturaVariasLucesShader);

	//BaseSilla
	Material MBaseSilla;
	MBaseSilla.diffuse = glm::vec4(0.212, 0.072, 0.029, 1.000);
	MBaseSilla.specular = glm::vec4(0.212, 0.072, 0.029, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MBaseSilla.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MBaseSilla.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MBaseSilla.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MBaseSilla.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	BaseSilla->Draw(*materialSinTexturaVariasLucesShader);

	//Campana1
	Material MCampana1;
	MCampana1.diffuse = glm::vec4(0.800, 0.800, 0.800, 1.000);
	MCampana1.specular = glm::vec4(0.800, 0.800, 0.800, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MCampana1.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MCampana1.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MCampana1.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MCampana1.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	Campana1->Draw(*materialSinTexturaVariasLucesShader);

	//Campana2
	Material MCampana2;
	MCampana2.diffuse = glm::vec4(0.066, 0.358, 0.800, 1.000);
	MCampana2.specular = glm::vec4(0.066, 0.358, 0.800, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MCampana2.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MCampana2.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MCampana2.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MCampana2.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	Campana2->Draw(*materialSinTexturaVariasLucesShader);

	//CampanaFondo
	Material MCampanaFondo;
	MCampanaFondo.diffuse = glm::vec4(0.000, 0.000, 0.000, 1.000);
	MCampanaFondo.specular = glm::vec4(0.000, 0.000, 0.000, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MCampanaFondo.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MCampanaFondo.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MCampanaFondo.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MCampanaFondo.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	CampanaFondo->Draw(*materialSinTexturaVariasLucesShader);

	//CampanaTope
	Material MCampanaTope;
	MCampanaTope.diffuse = glm::vec4(0.270, 0.270, 0.270, 1.000);
	MCampanaTope.specular = glm::vec4(0.270, 0.270, 0.270, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MCampanaTope.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MCampanaTope.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MCampanaTope.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MCampanaTope.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	CampanaTope->Draw(*materialSinTexturaVariasLucesShader);

	//EstufaCuerpo
	Material MEstufaCuerpo;
	MEstufaCuerpo.diffuse = glm::vec4(0.153, 0.153, 0.153, 1.000);
	MEstufaCuerpo.specular = glm::vec4(0.153, 0.153, 0.153, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MEstufaCuerpo.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MEstufaCuerpo.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MEstufaCuerpo.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MEstufaCuerpo.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	EstufaCuerpo->Draw(*materialSinTexturaVariasLucesShader);

	//EstufaIntegral
	Material MEstufaIntegral;
	MEstufaIntegral.diffuse = glm::vec4(0.434, 0.388, 0.393, 1.000);
	MEstufaIntegral.specular = glm::vec4(0.434, 0.388, 0.393, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MEstufaIntegral.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MEstufaIntegral.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MEstufaIntegral.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MEstufaIntegral.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	EstufaIntegral->Draw(*materialSinTexturaVariasLucesShader);

	//EstufaIntegralMarco
	Material MEstufaIntegralMarco;
	MEstufaIntegralMarco.diffuse = glm::vec4(0.166, 0.149, 0.151, 1.000);
	MEstufaIntegralMarco.specular = glm::vec4(0.166, 0.149, 0.151, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MEstufaIntegralMarco.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MEstufaIntegralMarco.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MEstufaIntegralMarco.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MEstufaIntegralMarco.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	EstufaIntegralMarco->Draw(*materialSinTexturaVariasLucesShader);

	//EstufaMesa
	Material MEstufaMesa;
	MEstufaMesa.diffuse = glm::vec4(0.800, 0.800, 0.800, 1.000);
	MEstufaMesa.specular = glm::vec4(0.800, 0.800, 0.800, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MEstufaMesa.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MEstufaMesa.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MEstufaMesa.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MEstufaMesa.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	EstufaMesa->Draw(*materialSinTexturaVariasLucesShader);

	//EstufaPiso
	Material MEstufaPiso;
	MEstufaPiso.diffuse = glm::vec4(0.000, 0.000, 0.000, 1.000);
	MEstufaPiso.specular = glm::vec4(0.000, 0.000, 0.000, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MEstufaPiso.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MEstufaPiso.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MEstufaPiso.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MEstufaPiso.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	EstufaPiso->Draw(*materialSinTexturaVariasLucesShader);

	//Extractor
	Material MExtractor;
	MExtractor.diffuse = glm::vec4(0.800, 0.717, 0.499, 1.000);
	MExtractor.specular = glm::vec4(0.800, 0.717, 0.499, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MExtractor.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MExtractor.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MExtractor.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MExtractor.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	Extractor->Draw(*materialSinTexturaVariasLucesShader);

	//ExtractorFondo
	Material MExtractorFondo;
	MExtractorFondo.diffuse = glm::vec4(0.232, 0.151, 0.051, 1.000);
	MExtractorFondo.specular = glm::vec4(0.232, 0.151, 0.051, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MExtractorFondo.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MExtractorFondo.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MExtractorFondo.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MExtractorFondo.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	ExtractorFondo->Draw(*materialSinTexturaVariasLucesShader);

	//ExtractorMarco
	Material MExtractorMarco;
	MExtractorMarco.diffuse = glm::vec4(0.287, 0.258, 0.182, 1.000);
	MExtractorMarco.specular = glm::vec4(0.287, 0.258, 0.182, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MExtractorMarco.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MExtractorMarco.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MExtractorMarco.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MExtractorMarco.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	ExtractorMarco->Draw(*materialSinTexturaVariasLucesShader);

	//MangoEstufa
	Material MMangoEstufa;
	MMangoEstufa.diffuse = glm::vec4(0.000, 0.000, 0.000, 1.000);
	MMangoEstufa.specular = glm::vec4(0.000, 0.000, 0.000, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MMangoEstufa.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MMangoEstufa.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MMangoEstufa.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MMangoEstufa.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	MangoEstufa->Draw(*materialSinTexturaVariasLucesShader);

	//Mesa
	Material MMesa;
	MMesa.diffuse = glm::vec4(0.130, 0.022, 0.010, 1.000);
	MMesa.specular = glm::vec4(0.130, 0.022, 0.010, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MMesa.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MMesa.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MMesa.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MMesa.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	Mesa->Draw(*materialSinTexturaVariasLucesShader);

	//Mesita
	Material MMesita;
	MMesita.diffuse = glm::vec4(0.141, 0.045, 0.016, 1.000);
	MMesita.specular = glm::vec4(0.141, 0.045, 0.016, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MMesita.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MMesita.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MMesita.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MMesita.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	Mesita->Draw(*materialSinTexturaVariasLucesShader);

	//ParedesCocina
	Material MParedesCocina;
	MParedesCocina.diffuse = glm::vec4(0.280, 0.684, 0.800, 1.000);
	MParedesCocina.specular = glm::vec4(0.280, 0.684, 0.800, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MParedesCocina.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MParedesCocina.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MParedesCocina.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MParedesCocina.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	ParedesCocina->Draw(*materialSinTexturaVariasLucesShader);

	//Refri
	Material MRefri;
	MRefri.diffuse = glm::vec4(0.287, 0.287, 0.287, 1.000);
	MRefri.specular = glm::vec4(0.5, 0.5, 0.5, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MRefri.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MRefri.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MRefri.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MRefri.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	Refri->Draw(*materialSinTexturaVariasLucesShader);

	//RefriMarco
	Material MRefriMarco;
	MRefriMarco.diffuse = glm::vec4(0.070, 0.070, 0.070, 1.000);
	MRefriMarco.specular = glm::vec4(0.070, 0.070, 0.070, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MRefriMarco.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MRefriMarco.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MRefriMarco.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MRefriMarco.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	RefriMarco->Draw(*materialSinTexturaVariasLucesShader);

	//Sarten
	Material MSarten;
	MSarten.diffuse = glm::vec4(0.1, 0.1, 0.1, 1.000);
	MSarten.specular = glm::vec4(0.5, 0.5, 0.5, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MSarten.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MSarten.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MSarten.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MSarten.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	Sarten->Draw(*materialSinTexturaVariasLucesShader);

	//Silla
	Material MSilla;
	MSilla.diffuse = glm::vec4(0.563, 0.441, 0.125, 1.000);
	MSilla.specular = glm::vec4(0.663, 0.541, 0.225, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MSilla.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MSilla.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MSilla.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MSilla.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	Silla->Draw(*materialSinTexturaVariasLucesShader);

	//SillaInterior
	Material MSillaInterior;
	MSillaInterior.diffuse = glm::vec4(0.299, 0.236, 0.069, 1.000);
	MSillaInterior.specular = glm::vec4(0.299, 0.236, 0.069, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MSillaInterior.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MSillaInterior.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MSillaInterior.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MSillaInterior.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	SillaInterior->Draw(*materialSinTexturaVariasLucesShader);

	//TechoCocina
	Material MTechoCocina;
	MTechoCocina.diffuse = glm::vec4(0.816, 0.052, 0.031, 1.000);
	MTechoCocina.specular = glm::vec4(0.816, 0.052, 0.031, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MTechoCocina.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MTechoCocina.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MTechoCocina.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MTechoCocina.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	TechoCocina->Draw(*materialSinTexturaVariasLucesShader);

	//TransicionTechoCocina
	Material MTransicionTechoCocina;
	MTransicionTechoCocina.diffuse = glm::vec4(0.800, 0.800, 0.800, 1.000);
	MTransicionTechoCocina.specular = glm::vec4(0.800, 0.800, 0.800, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MTransicionTechoCocina.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MTransicionTechoCocina.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MTransicionTechoCocina.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MTransicionTechoCocina.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	TransicionTechoCocina->Draw(*materialSinTexturaVariasLucesShader);

	//TroncoSaliente
	Material MTroncoSaliente;
	MTroncoSaliente.diffuse = glm::vec4(0.356, 0.090, 0.030, 1.000);
	MTroncoSaliente.specular = glm::vec4(0.356, 0.090, 0.030, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MTroncoSaliente.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MTroncoSaliente.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MTroncoSaliente.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", MTroncoSaliente.transparency);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	TroncoSaliente->Draw(*materialSinTexturaVariasLucesShader);

	//VentanaEstufa
	Material MVentanaEstufa;
	MVentanaEstufa.diffuse = glm::vec4(0.185, 0.326, 0.280, 1.000);
	MVentanaEstufa.specular = glm::vec4(0.5, 0.5, 0.5, 1.000);
	materialSinTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MVentanaEstufa.ambient);
	materialSinTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MVentanaEstufa.diffuse);
	materialSinTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MVentanaEstufa.specular);
	materialSinTexturaVariasLucesShader->setFloat("transparency", 0.5f);
	materialSinTexturaVariasLucesShader->setInt("shininess", 5);
	VentanaEstufa->Draw(*materialSinTexturaVariasLucesShader);
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
	// Luz direccional negy
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

	Material MPasto;
	materialConTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MPasto.ambient);
	materialConTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MPasto.diffuse);
	materialConTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MPasto.specular);
	materialConTexturaVariasLucesShader->setFloat("transparency", MPasto.transparency);
	materialConTexturaVariasLucesShader->setInt("shininess", 10);
	Pasto->Draw(*materialConTexturaVariasLucesShader);

	Material MPisoMadera;
	materialConTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MPisoMadera.ambient);
	materialConTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MPisoMadera.diffuse);
	materialConTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MPisoMadera.specular);
	materialConTexturaVariasLucesShader->setFloat("transparency", MPisoMadera.transparency);
	materialConTexturaVariasLucesShader->setInt("shininess", 50);
	PisoMadera->Draw(*materialConTexturaVariasLucesShader);

	Material MMaderaArbol;
	materialConTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MMaderaArbol.ambient);
	materialConTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MMaderaArbol.diffuse);
	materialConTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MMaderaArbol.specular);
	materialConTexturaVariasLucesShader->setFloat("transparency", MMaderaArbol.transparency);
	materialConTexturaVariasLucesShader->setInt("shininess", 30);
	MaderaArbol->Draw(*materialConTexturaVariasLucesShader);

	Material MBarcoBaseTextura;
	materialConTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MBarcoBaseTextura.ambient);
	materialConTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MBarcoBaseTextura.diffuse);
	materialConTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MBarcoBaseTextura.specular);
	materialConTexturaVariasLucesShader->setFloat("transparency", MBarcoBaseTextura.transparency);
	materialConTexturaVariasLucesShader->setInt("shininess", 50);
	BarcoBaseTextura->Draw(*materialConTexturaVariasLucesShader);

	Material MBarcoCostado;
	materialConTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MBarcoCostado.ambient);
	materialConTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MBarcoCostado.diffuse);
	materialConTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MBarcoCostado.specular);
	materialConTexturaVariasLucesShader->setFloat("transparency", MBarcoCostado.transparency);
	materialConTexturaVariasLucesShader->setInt("shininess", 50);
	BarcoCostado->Draw(*materialConTexturaVariasLucesShader);

	Material MSoffa1;
	materialConTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MSoffa1.ambient);
	materialConTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MSoffa1.diffuse);
	materialConTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MSoffa1.specular);
	materialConTexturaVariasLucesShader->setFloat("transparency", MSoffa1.transparency);
	materialConTexturaVariasLucesShader->setInt("shininess", 5);
	Soffa1->Draw(*materialConTexturaVariasLucesShader);

	Material MSoffa2;
	materialConTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MSoffa2.ambient);
	materialConTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MSoffa2.diffuse);
	materialConTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MSoffa2.specular);
	materialConTexturaVariasLucesShader->setFloat("transparency", MSoffa2.transparency);
	materialConTexturaVariasLucesShader->setInt("shininess", 5);
	Soffa2->Draw(*materialConTexturaVariasLucesShader);

	Material MMantel;
	materialConTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MMantel.ambient);
	materialConTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MMantel.diffuse);
	materialConTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MMantel.specular);
	materialConTexturaVariasLucesShader->setFloat("transparency", MMantel.transparency);
	materialConTexturaVariasLucesShader->setInt("shininess", 5);
	Mantel->Draw(*materialConTexturaVariasLucesShader);

	Material MPisoAjedrez;
	materialConTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MPisoAjedrez.ambient);
	materialConTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MPisoAjedrez.diffuse);
	materialConTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MPisoAjedrez.specular);
	materialConTexturaVariasLucesShader->setFloat("transparency", MPisoAjedrez.transparency);
	materialConTexturaVariasLucesShader->setInt("shininess", 5);
	PisoAjedrez->Draw(*materialConTexturaVariasLucesShader);

	Material MPisoMadera1;
	materialConTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MPisoMadera1.ambient);
	materialConTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MPisoMadera1.diffuse);
	materialConTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MPisoMadera1.specular);
	materialConTexturaVariasLucesShader->setFloat("transparency", MPisoMadera1.transparency);
	materialConTexturaVariasLucesShader->setInt("shininess", 5);
	PisoMadera1->Draw(*materialConTexturaVariasLucesShader);

	Material MTronco;
	materialConTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MTronco.ambient);
	materialConTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MTronco.diffuse);
	materialConTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MTronco.specular);
	materialConTexturaVariasLucesShader->setFloat("transparency", MTronco.transparency);
	materialConTexturaVariasLucesShader->setInt("shininess", 5);
	Tronco->Draw(*materialConTexturaVariasLucesShader);

	Material MParedMadera;
	materialConTexturaVariasLucesShader->setVec4("MaterialAmbientColor", MParedMadera.ambient);
	materialConTexturaVariasLucesShader->setVec4("MaterialDiffuseColor", MParedMadera.diffuse);
	materialConTexturaVariasLucesShader->setVec4("MaterialSpecularColor", MParedMadera.specular);
	materialConTexturaVariasLucesShader->setFloat("transparency", MParedMadera.transparency);
	materialConTexturaVariasLucesShader->setInt("shininess", 5);
	ParedMadera->Draw(*materialConTexturaVariasLucesShader);

	glUseProgram(0);
}

// Dibujo del fondo
void DibujaFondo() {
	// Activamos el shader estático
	fondoShader->use();

	// Aplicamos transformaciones de proyección y cámara (si las hubiera)
	fondoShader->setMat4("projection", projection);
	fondoShader->setMat4("view", view);

	// Aplicamos transformaciones del modelo
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Trasladar al centro de la escena
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	fondoShader->setMat4("model", model);
	// Propiedades de la luz
	// Luz direccional posx
	fondoShader->setVec3("dirLight[0].direction", posx.Direction);
	fondoShader->setVec3("dirLight[0].ambient", posx.Ambient);
	fondoShader->setVec3("dirLight[0].diffuse", posx.Diffuse);
	fondoShader->setVec3("dirLight[0].specular", posx.Specular);
	fondoShader->setVec3("dirLight[0].power", posx.Power);
	// Luz direccional negx
	fondoShader->setVec3("dirLight[1].direction", negx.Direction);
	fondoShader->setVec3("dirLight[1].ambient", negx.Ambient);
	fondoShader->setVec3("dirLight[1].diffuse", negx.Diffuse);
	fondoShader->setVec3("dirLight[1].specular", negx.Specular);
	fondoShader->setVec3("dirLight[1].power", negx.Power);
	// Luz direccional posy
	fondoShader->setVec3("dirLight[2].direction", posy.Direction);
	fondoShader->setVec3("dirLight[2].ambient", posy.Ambient);
	fondoShader->setVec3("dirLight[2].diffuse", posy.Diffuse);
	fondoShader->setVec3("dirLight[2].specular", posy.Specular);
	fondoShader->setVec3("dirLight[2].power", posy.Power);
	//// Luz direccional negy
	fondoShader->setVec3("dirLight[3].direction", negy.Direction);
	fondoShader->setVec3("dirLight[3].ambient", negy.Ambient);
	fondoShader->setVec3("dirLight[3].diffuse", negy.Diffuse);
	fondoShader->setVec3("dirLight[3].specular", negy.Specular);
	fondoShader->setVec3("dirLight[3].power", negy.Power);
	// Luz direccional posz
	fondoShader->setVec3("dirLight[4].direction", posz.Direction);
	fondoShader->setVec3("dirLight[4].ambient", posz.Ambient);
	fondoShader->setVec3("dirLight[4].diffuse", posz.Diffuse);
	fondoShader->setVec3("dirLight[4].specular", posz.Specular);
	fondoShader->setVec3("dirLight[4].power", posz.Power);
	// Luz direccional negz
	fondoShader->setVec3("dirLight[5].direction", negz.Direction);
	fondoShader->setVec3("dirLight[5].ambient", negz.Ambient);
	fondoShader->setVec3("dirLight[5].diffuse", negz.Diffuse);
	fondoShader->setVec3("dirLight[5].specular", negz.Specular);
	fondoShader->setVec3("dirLight[5].power", negz.Power);

	// Posición de la camara
	fondoShader->setVec3("viewPos", camera.Position);

	// Habilitar blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Material MFondo;
	fondoShader->setVec4("MaterialAmbientColor", MFondo.ambient);
	fondoShader->setVec4("MaterialDiffuseColor", MFondo.diffuse);
	fondoShader->setVec4("MaterialSpecularColor", MFondo.specular);
	fondoShader->setFloat("transparency", MFondo.transparency);
	fondoShader->setInt("shininess", 50);
	Fondo->Draw(*fondoShader);

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
	
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	fresnelShader->setFloat("transparency", 0.8f);
	fresnelShader->setVec4("colorDiffuse",glm::vec4(0.20f, 1.0f, 0.96f, 1.0f));
	Vidrio->Draw(*fresnelShader);

	fresnelShader->setFloat("transparency", 0.8f);
	fresnelShader->setVec4("colorDiffuse", glm::vec4(0.20f, 1.0f, 0.96f, 1.0f));
	VentanaPuertaSinMarco->Draw(*fresnelShader);

	fresnelShader->setFloat("transparency", 1.0f);
	fresnelShader->setVec4("colorDiffuse", glm::vec4(0.87f, 0.70f, 0.0f, 1.0f));
	BotonesDorados->Draw(*fresnelShader);
	glUseProgram(0);
}

// Dibujo de animaciones con varias luces direccionales
void MaterialesAnimadosVariasLuces() {

	elapsedTime += deltaTime;
	if (elapsedTime > 1.0f / fps) {
		animationCount++;
		if (animationCount > keys - 1) {
			animationCount = 0;
		}
		// Configuración de la pose en el instante t
		Zoro->SetPose((float)animationCount, ZoroBones, animationTypeZoro);
		Robin->SetPose((float)animationCount, RobinBones, animationTypeRobin);
		elapsedTime = 0.0f;
	}

	// Activamos el shader dinámico
	animados->use();

	// Aplicamos transformaciones de proyección y cámara (si las hubiera)
	animados->setMat4("projection", projection);
	animados->setMat4("view", view);

	// Propiedades de la luz

	// Luz direccional

	// Luz direccional posx
	animados->setVec3("dirLight[0].direction", posx.Direction);
	animados->setVec3("dirLight[0].ambient", posx.Ambient);
	animados->setVec3("dirLight[0].diffuse", posx.Diffuse);
	animados->setVec3("dirLight[0].specular", posx.Specular);
	animados->setVec3("dirLight[0].power", posx.Power);
	// Luz direccional negx
	animados->setVec3("dirLight[1].direction", negx.Direction);
	animados->setVec3("dirLight[1].ambient", negx.Ambient);
	animados->setVec3("dirLight[1].diffuse", negx.Diffuse);
	animados->setVec3("dirLight[1].specular", negx.Specular);
	animados->setVec3("dirLight[1].power", negx.Power);
	// Luz direccional negy
	animados->setVec3("dirLight[2].direction", negy.Direction);
	animados->setVec3("dirLight[2].ambient", negy.Ambient);
	animados->setVec3("dirLight[2].diffuse", negy.Diffuse);
	animados->setVec3("dirLight[2].specular", negy.Specular);
	animados->setVec3("dirLight[2].power", negy.Power);
	// Luz direccional posz
	animados->setVec3("dirLight[3].direction", posz.Direction);
	animados->setVec3("dirLight[3].ambient", posz.Ambient);
	animados->setVec3("dirLight[3].diffuse", posz.Diffuse);
	animados->setVec3("dirLight[3].specular", posz.Specular);
	animados->setVec3("dirLight[3].power", posz.Power);
	// Luz direccional negz
	animados->setVec3("dirLight[4].direction", negz.Direction);
	animados->setVec3("dirLight[4].ambient", negz.Ambient);
	animados->setVec3("dirLight[4].diffuse", negz.Diffuse);
	animados->setVec3("dirLight[4].specular", negz.Specular);
	animados->setVec3("dirLight[4].power", negz.Power);

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


	// Zoro
	// Aplicamos transformaciones del modelo
	glm::mat4 modelZoro = glm::mat4(1.0f);
	modelZoro = glm::translate(modelZoro, positionZoro);
	modelZoro = glm::rotate(modelZoro, glm::radians(rotateCharacterZoro), glm::vec3(0.0f, 1.0f, 0.0f));
	modelZoro = glm::rotate(modelZoro, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelZoro = glm::scale(modelZoro, glm::vec3(0.01f, 0.01f, 0.01f));	

	animados->setMat4("model", modelZoro);

	animados->setMat4("gBones", MAX_RIGGING_BONES, ZoroBones);
	Material MZoro;
	animados->setVec4("MaterialAmbientColor", MZoro.ambient);
	animados->setVec4("MaterialDiffuseColor", MZoro.diffuse);
	animados->setVec4("MaterialSpecularColor", MZoro.specular);
	animados->setFloat("transparency", MZoro.transparency);
	animados->setInt("shininess", 1);
	Zoro->Draw(*animados);

	// Robin
	// Aplicamos transformaciones del modelo
	glm::mat4 modelRobin = glm::mat4(1.0f);
	modelRobin = glm::translate(modelRobin, glm::vec3(0.0f, 0.0f, -3.0f));
	modelRobin = glm::translate(modelRobin, positionRobin);
	modelRobin = glm::rotate(modelRobin, glm::radians(rotateCharacterRobin), glm::vec3(0.0f, 1.0f, 0.0f));
	modelRobin = glm::rotate(modelRobin, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelRobin = glm::scale(modelRobin, glm::vec3(0.01f, 0.01f, 0.01f));

	animados->setMat4("model", modelRobin);
	animados->setMat4("gBones", MAX_RIGGING_BONES, RobinBones);
	Material MRobin;
	animados->setVec4("MaterialAmbientColor", MRobin.ambient);
	animados->setVec4("MaterialDiffuseColor", MRobin.diffuse);
	animados->setVec4("MaterialSpecularColor", MRobin.specular);
	animados->setFloat("transparency", MRobin.transparency);
	animados->setInt("shininess", 1);
	Robin->Draw(*animados);

	glUseProgram(0);
}

// Dibujo de los modelos en la escena, transformaciones
bool Update() {

	// Procesa la entrada del teclado o mouse
	processInput(window);

	// Cálculo del framerate
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	elapsedTime += deltaTime;

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

	// Proyecciones y vistas
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	view = camera.GetViewMatrix();

	// Materiales Estáticos
	//		Materiales sin textura

	//			Materiales con una sola luz direccional (metales y hojas)
	MaterialesSinTexturaUnaLuz();

	//			Materiales con 6 luces posicionales
	MaterialesSinTexturaVariasLuces();
	MaterialesSinTexturaCocina();

	//			Objetos Sin textura con Iluminación de Fresnel
	MaterialesSinTexturaFresnel();

	//		Materiales Con textura
	//			Materiales con varias luces direccionales
	MaterialesConTexturasVariasLuces();
	DibujaFondo();

	// Materiales Animados
	// 	   Materiales con textura
	
	//			Materiales con textura animados phong
	MaterialesAnimadosVariasLuces();

	// glfw: swap buffers 
	/* Swap front and back buffers */
	glfwSwapBuffers(window);

	/* Poll for and process events */
	glfwPollEvents();

	return true;
}

// Función para eliminar de memoria las mallas
void Limpiar() {
	delete Canons;
	delete MarcoOroPuerta;
	delete MaderaPuerta;
	delete MarcoPuertaRojo;
	delete MarcoPuertaBlanco;
	delete CostadosRojos;
	delete FondoCanons;
	delete RejillaVentana;
	delete MarcoCostado;
	delete BarcoBase;
	delete Timon;
	delete BarcoBaseTextura;
	delete BarcoCostado;
	delete Pasto;
	delete Paredes1;
	delete PisoMadera;
	delete BarandalesYLaterales;
	delete CintaInferior;
	delete BotonesDorados;
	delete BandaSuperior;
	delete TechoParedSuperior;
	delete Propulsor;
	delete AmarilloTecho;
	delete MarcoVentana;
	delete MarcoFarol;
	delete BaseBandera;
	delete Velas;
	delete BanderaRoja;
	delete MetalMastil;
	delete BaseCuartoMastil;
	delete MaderaArbol;
	delete Hojas;
	delete MaterialCadena;
	delete FondoCara;
	delete Huesos;
	delete FondoOjos;
	delete Melena;
	// Fresnel
	delete Vidrio;
	delete VentanaPuertaSinMarco;
	// Modelos Cocina 
	// Phong
	delete BarraCuerpo;
	delete BarraMesa;
	delete BarraPiso;
	delete BaseSilla;
	delete Campana1;
	delete Campana2;
	delete CampanaFondo;
	delete CampanaTope;
	delete EstufaCuerpo;
	delete EstufaIntegral;
	delete EstufaIntegralMarco;
	delete EstufaMesa;
	delete EstufaPiso;
	delete Extractor;
	delete ExtractorFondo;
	delete ExtractorMarco;
	delete MangoEstufa;
	delete Mantel;
	delete Mesa;
	delete Mesita;
	delete ParedesCocina;
	delete ParedMadera;
	delete PisoAjedrez;
	delete PisoMadera1;
	delete Refri;
	delete RefriMarco;
	delete Sarten;
	delete Silla;
	delete SillaInterior;
	delete Soffa1;
	delete Soffa2;
	delete TechoCocina;
	delete TransicionTechoCocina;
	delete Tronco;
	delete TroncoSaliente;
	delete VentanaEstufa;
	// Modelo del fondo
	delete Fondo;

	// Animaciones
	delete Zoro;
	delete Robin;

	// Shaders
	delete materialSinTexturaUnaLuzShader;
	delete materialSinTexturaVariasLucesShader;
	delete materialConTexturaVariasLucesShader;
	delete fondoShader;
	delete fresnelShader;
	delete animados;
}

// Procesamos entradas del teclado
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Movimientos Camara
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	// Zoro Movimiento
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		positionZoro = positionZoro + scaleVZoro * forwardViewZoro;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		positionZoro = positionZoro - scaleVZoro * forwardViewZoro;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		rotateCharacterZoro += 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacterZoro), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		forwardViewZoro = glm::vec3(viewVector);
		forwardViewZoro = glm::normalize(forwardViewZoro);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		rotateCharacterZoro -= 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacterZoro), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		forwardViewZoro = glm::vec3(viewVector);
		forwardViewZoro = glm::normalize(forwardViewZoro);
	}
	// Robin Movimiento
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		positionRobin = positionRobin + scaleVRobin * forwardViewRobin;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		positionRobin = positionRobin - scaleVRobin * forwardViewRobin;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		rotateCharacterRobin += 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacterRobin), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		forwardViewRobin = glm::vec3(viewVector);
		forwardViewRobin = glm::normalize(forwardViewRobin);
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		rotateCharacterRobin -= 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacterRobin), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		forwardViewRobin = glm::vec3(viewVector);
		forwardViewRobin = glm::normalize(forwardViewRobin);
	}
	// Prender o apagar la linterna
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		Lintern.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		Lintern.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
		Lintern.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
		Lintern.setPower(30.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		Lintern.Diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
		Lintern.Specular = glm::vec3(0.0f, 0.0f, 0.0f);
		Lintern.Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		Lintern.setPower(0.0f);
	}
	// Timón
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		timonRotation += 20.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		timonRotation -= 20.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		if (escalaTimon < 2.0f) {
			escalaTimon += 0.05f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		if (escalaTimon > 1.0f) {
			escalaTimon -= 0.05f;
		}
	}
	// Control animaciones
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
		animationTypeZoro = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
		animationTypeZoro = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
		animationTypeRobin = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS) {
		animationTypeRobin = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		if (scaleVZoro < 0.1) {
			scaleVZoro += 0.005f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		if (scaleVZoro > 0.0) {
			scaleVZoro -= 0.005f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		if (scaleVRobin < 0.1) {
			scaleVRobin += 0.005f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		if (scaleVRobin > 0.0) {
			scaleVRobin -= 0.005f;
		}
	}
	// velocidad camara
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		if (camera.MovementSpeed < 20.0) {
			camera.MovementSpeed += 0.1f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		if (camera.MovementSpeed > 0.0) {
			camera.MovementSpeed -= 0.1f;
		}
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
