#version 330 core
// estas no se mueven, ya están integradas en el código
layout (location = 0) in vec3  aPos;
layout (location = 1) in vec3  aNormal;
layout (location = 2) in vec2  aTexCoords;
layout (location = 3) in vec3  tangent;
layout (location = 4) in vec3  bitangent;

// para dibujar la escena, se mandan estas 3 matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// para la luz segun el tutorial
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    // calcula en que posición está el fragmento
    vec4 PosL = vec4(aPos, 1.0f);
    FragPos = vec3(model * PosL);

    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * model * PosL;
}