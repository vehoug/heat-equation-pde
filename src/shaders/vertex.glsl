#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out float Temperature;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2D temperatureMap;
uniform float maxTemperature;

void main() {
    // Sample temperature from the texture
    float temp = texture(temperatureMap, aTexCoord).r;
    Temperature = temp;
    
    // Calculate height based on temperature (0.0 to 0.5)
    float height = temp / maxTemperature * 0.5;
    
    // Update position with height
    vec3 pos = aPos;
    pos.y = height;
    
    // Calculate final position
    gl_Position = projection * view * model * vec4(pos, 1.0);
    
    // Pass texture coordinates to fragment shader
    TexCoord = aTexCoord;
}