#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "shader.hpp"

namespace rendering
{

class Renderer
{
  public:
    Renderer(int width, int height);
    ~Renderer();

    void render(const std::vector<std::vector<float>>& temperatureGrid);
    void setRotation(float xRotation, float yRotation);
    void setZoom(float zoom);

  private:
    int width_;
    int height_;
    shader::Shader* shader_;
    unsigned int VAO_, VBO_, EBO_;
    unsigned int temperatureTexture_;
    float xRotation_, yRotation_, zoom_;

    void setupMesh();
    std::vector<float> flattenTemperatureGrid(const std::vector<std::vector<float>>& temperatureGrid);

};

} // namespace rendering

#endif // RENDERER_HPP