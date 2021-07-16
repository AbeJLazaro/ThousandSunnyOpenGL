#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class PointLight {
public:
	// Light Attributes
	glm::vec3 Position;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	glm::vec3 Power;
	float Constant;
	float Linear;
	float Quadratic;


	PointLight() {
		Position = glm::vec3(0.0f, 0.0f, 0.0f); // Dirección
		Ambient = glm::vec3(0.05f, 0.05f, 0.05f); // Componente Ambiental
		Diffuse = glm::vec3(1.0f, 1.0f, 1.0f); // Color de la fuente de luz
		Specular = glm::vec3(1.0f, 1.0f, 1.0f); // Componente Especular
		Constant = 1.0f;
		Linear = 0.09;
		Quadratic = 0.032;
		Power = glm::vec3(30.0f, 30.0f, 30.0f); // Potencia en Watts
	}
	~PointLight() {}

	void setPosition(glm::vec3 vector) {
		this->Position = vector;
	}
	void setPosition(float x, float y, float z) {
		this->Position = glm::vec3(x,y,z);
	}
	void setColor(glm::vec3 vector) {
		this->Diffuse = vector;
	}
	void setColor(float x, float y, float z) {
		this->Diffuse = glm::vec3(x, y, z);
	}
	void setPower(float intensity) {
		this->Power = glm::vec3(intensity, intensity, intensity);
	}
private:

};

#endif

