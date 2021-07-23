#version 330 core
out vec4 FragColor;

// structuras de datos para implementar las luces +

//checar como quitar Material
struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 power;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 power;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_DIR_LIGHTS 6

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// viewPos es eye en la práctica
// Luces
uniform DirLight dirLight[NR_DIR_LIGHTS];
// propiedades material
uniform vec4 MaterialAmbientColor;
uniform vec4 MaterialDiffuseColor;
uniform vec4 MaterialSpecularColor;
uniform float transparency;
uniform sampler2D texture_diffuse1;
uniform int  shininess;
//propiedades luz
uniform vec3 viewPos;


// funciones
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{    
    // Propiedades
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0,0.0,0.0);
    
    // Calculo de la luz direccional
    for(int i = 0; i < NR_DIR_LIGHTS; i++)
        result += CalcDirLight(dirLight[i], norm, viewDir);

    // se calcula el punto de textura
    vec4 texel = texture(texture_diffuse1, TexCoords);

    vec4 color = vec4(result,1.0);
    color.a = transparency;

    FragColor = texel*color;
}

// Calcula la luz direccional
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // se toma el vector contrario por que se ocupa el vector del punto a la luz
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 MaterialAmbColor = vec3(MaterialAmbientColor * MaterialDiffuseColor);
    vec3 ambient = light.ambient * MaterialAmbColor;
    vec3 diffuse = light.diffuse * light.power * diff * MaterialDiffuseColor.xyz;
    vec3 specular = light.specular * light.power * spec * MaterialSpecularColor.xyz;
    return (ambient + diffuse + specular);
}
