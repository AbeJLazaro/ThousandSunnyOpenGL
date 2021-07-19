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

#define NR_POINT_LIGHTS 4
#define NR_DIR_LIGHTS 1

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// viewPos es eye en la práctica
// Luces
uniform DirLight dirLight[NR_DIR_LIGHTS];
uniform SpotLight spotLight;
//uniform PointLight pointLights[NR_POINT_LIGHTS];
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
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // Propiedades
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0,0.0,0.0);
    // Calculo de la luz direccional
    for(int i = 0; i < NR_DIR_LIGHTS; i++)
        result += CalcDirLight(dirLight[i], norm, viewDir);
    
    // Calculo de las luces puntuales
    //for(int i = 0; i < NR_POINT_LIGHTS; i++)
    //    result += CalcPointLight(pointLights[i], norm, FragPos, viewDir); 

    // Calculo de la luz linterna
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir); 

    // se calcula el punto de textura
    vec4 texel = texture(texture_diffuse1, TexCoords);

    vec4 color = vec4(result,1.0);
    color.a = transparency;

    FragColor = color;
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

// Calcula el color con un punto de luz
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 MaterialAmbColor = vec3(MaterialAmbientColor * MaterialDiffuseColor);
    vec3 ambient = light.ambient * MaterialAmbColor;
    vec3 diffuse = light.diffuse * light.power * diff * MaterialDiffuseColor.xyz;
    vec3 specular = light.specular * light.power * spec * MaterialSpecularColor.xyz;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// Calcula el color usando la luz de una linterna
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 MaterialAmbColor = vec3(MaterialAmbientColor * MaterialDiffuseColor);
    vec3 ambient = light.ambient * MaterialAmbColor;
    vec3 diffuse = light.diffuse * diff * MaterialDiffuseColor.xyz;
    vec3 specular = light.specular * spec * MaterialSpecularColor.xyz;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}