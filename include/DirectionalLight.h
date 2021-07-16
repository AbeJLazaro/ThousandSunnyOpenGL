#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class DirectionalLight{
public:
	// Light Attributes
	glm::vec3 Direction;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	glm::vec3 Power;

	DirectionalLight() {
		Direction = glm::vec3(0.0f, -1.0f, 0.0f); // Dirección
		Ambient = glm::vec3(0.05f, 0.05f, 0.05f); // Componente Ambiental
		Diffuse = glm::vec3(1.0f, 1.0f, 1.0f); // Color de la fuente de luz
		Specular = glm::vec3(0.5f, 0.5f, 0.5f); // Componente Especular
		Power = glm::vec3(2.0f, 2.0f, 2.0f); // Potencia en Watts
	}
	~DirectionalLight() {}

	void setPower(float intensity) {
		this->Power = glm::vec3(intensity, intensity, intensity);
	}
private:

};

#endif
