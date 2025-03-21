#include "renderer.hpp"

namespace rendering
{

Renderer::Renderer(int width, int height) : width_(width), height_(height)
{
  xRotation_ = -30.0f;
  yRotation_ = 0.0f;
  zoom_ = 0.8f;

  shader_ = new shader::Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
  setupMesh();

  glGenTextures(1, &temperatureTexture_);
  glBindTexture(GL_TEXTURE_2D, temperatureTexture_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Renderer::~Renderer()
{
  delete shader_;

  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &VBO_);
  glDeleteBuffers(1, &EBO_);
  glDeleteTextures(1, &temperatureTexture_);
}

void Renderer::setupMesh()
{
  std::vector<float> vertices;
  std::vector<unsigned int> indices;

  for (int y = 0; y < height_; ++y)
  {
    for (int x = 0; x < width_; ++x)
    {
      float xPos = static_cast<float>(x) / static_cast<float>(width_ - 1) * 2.0f - 1.0f;
      float zPos = static_cast<float>(y) / static_cast<float>(height_ - 1) * 2.0f - 1.0f;
      float yPos = 0.0f;

      vertices.push_back(xPos);
      vertices.push_back(yPos);
      vertices.push_back(zPos);

      vertices.push_back(static_cast<float>(x) / (width_ - 1));
      vertices.push_back(static_cast<float>(y) / (height_ - 1));
    }
  }

  for (int y = 0; y < height_ - 1; ++y) 
  {
    for (int x = 0; x < width_ - 1; ++x) 
    {
      unsigned int topLeft = y * width_ + x;
      unsigned int topRight = topLeft + 1;
      unsigned int bottomLeft = (y + 1) * width_ + x;
      unsigned int bottomRight = bottomLeft + 1;
        
      indices.push_back(topLeft);
      indices.push_back(bottomLeft);
      indices.push_back(topRight);
        
      indices.push_back(topRight);
      indices.push_back(bottomLeft);
      indices.push_back(bottomRight);
    }
  }

  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glGenBuffers(1, &EBO_);
    
  glBindVertexArray(VAO_);
    
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
    
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
    
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

std::vector<float> Renderer::flattenTemperatureGrid(const std::vector<std::vector<float>>& temperatureGrid)
{
  std::vector<float> flattenedGrid(width_ * height_);

  for (int y = 0; y < height_; ++y)
  {
    for (int x = 0; x < width_; ++x)
    {
      flattenedGrid.at(y * width_ + x) = temperatureGrid.at(y).at(x);
    }
  }

  return flattenedGrid;
}

void Renderer::render(const std::vector<std::vector<float>>& temperatureGrid)
{
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
  glEnable(GL_DEPTH_TEST);
    
  shader_->use();
    
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(xRotation_), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(yRotation_), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::scale(model, glm::vec3(zoom_, zoom_, zoom_));
    
  glm::mat4 view = glm::lookAt(
      glm::vec3(0.0f, 1.5f, 2.0f),  
      glm::vec3(0.0f, 0.0f, 0.0f),  
      glm::vec3(0.0f, 1.0f, 0.0f)   
  );
    
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    
  shader_->setMat4("model", model);
  shader_->setMat4("view", view);
  shader_->setMat4("projection", projection);
    
  float maxTemp = 0.0f;
  for (const auto& row : temperatureGrid) 
  {
    for (const float& temp : row) 
    {
      maxTemp = std::max(maxTemp, temp);
    }
  }
    
  maxTemp = std::max(maxTemp, 0.01f);
    
  std::vector<float> flatTemp = flattenTemperatureGrid(temperatureGrid);
    
  glBindTexture(GL_TEXTURE_2D, temperatureTexture_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width_, height_, 0, GL_RED, GL_FLOAT, flatTemp.data());
    
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, temperatureTexture_);
  shader_->setInt("temperatureMap", 0);
    
  shader_->setFloat("maxTemperature", maxTemp); 
    
  glBindVertexArray(VAO_);
  glDrawElements(GL_TRIANGLES, (width_ - 1) * (height_ - 1) * 6, GL_UNSIGNED_INT, 0);  
}

void Renderer::setRotation(float xRotation, float yRotation)
{
  xRotation_ = xRotation;
  yRotation_ = yRotation;
}

void Renderer::setZoom(float zoom)
{
  zoom_ = zoom; 
}

} // namespace rendering