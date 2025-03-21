#include "shader.hpp"   

namespace shader
{

Shader::Shader(const char* vertexPath, const char* fragmentPath) 
{
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
    
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
  try 
  {
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    
    vShaderFile.close();
    fShaderFile.close();
    
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }
  catch (std::ifstream::failure& e) 
  {
    std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << '\n';
  }
    
  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();
    
  unsigned int vertex, fragment;
    
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");
    
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");
    
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  checkCompileErrors(ID, "PROGRAM");
    
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}
    
Shader::~Shader() 
{
  glDeleteProgram(ID);
}
    
void Shader::use() 
{
  glUseProgram(ID);
}
    
void Shader::setBool(const std::string& name, bool value) const 
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
    
void Shader::setInt(const std::string& name, int value) const 
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
    
void Shader::setFloat(const std::string& name, float value) const 
{
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
    
void Shader::setVec2(const std::string& name, const glm::vec2& value) const 
{
  glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}
    
void Shader::setVec3(const std::string& name, const glm::vec3& value) const 
{
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}
    
void Shader::setMat4(const std::string& name, const glm::mat4& value) const 
{
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
    
void Shader::checkCompileErrors(unsigned int shader, std::string type) 
{
  int success;
  char infoLog[1024];
  if (type != "PROGRAM") 
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << '\n';
    }
  }
  else 
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) 
    {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << '\n';
    }
  }
}

} // namespace shader