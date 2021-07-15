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


#define NR_POINT_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// viewPos es eye en la práctica
// Luces
uniform DirLight dirLight;
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
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // Propiedades
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // Calculo de la luz direccional
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    
    // Calculo de las luces puntuales
    //for(int i = 0; i < NR_POINT_LIGHTS; i++)
    //    result += CalcPointLight(pointLights[i], norm, FragPos, viewDir); 

    // Calculo de la luz linterna
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir); 

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
