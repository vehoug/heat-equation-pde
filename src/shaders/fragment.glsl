#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in float Temperature;

uniform float maxTemperature;

void main() {
    // Normalize temperature (0.0 to 1.0)
    float normalizedTemp = Temperature / maxTemperature;
    
    // Create a heat map color (blue to red)
    vec3 color;
    if (normalizedTemp < 0.25) {
        // Blue to cyan (0.0 to 0.25)
        float t = normalizedTemp / 0.25;
        color = mix(vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 1.0), t);
    } else if (normalizedTemp < 0.5) {
        // Cyan to green (0.25 to 0.5)
        float t = (normalizedTemp - 0.25) / 0.25;
        color = mix(vec3(0.0, 1.0, 1.0), vec3(0.0, 1.0, 0.0), t);
    } else if (normalizedTemp < 0.75) {
        // Green to yellow (0.5 to 0.75)
        float t = (normalizedTemp - 0.5) / 0.25;
        color = mix(vec3(0.0, 1.0, 0.0), vec3(1.0, 1.0, 0.0), t);
    } else {
        // Yellow to red (0.75 to 1.0)
        float t = (normalizedTemp - 0.75) / 0.25;
        color = mix(vec3(1.0, 1.0, 0.0), vec3(1.0, 0.0, 0.0), t);
    }
    
    FragColor = vec4(color, 1.0);
}