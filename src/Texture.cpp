#include "Texture.h"

#include <glad/glad.h>

#include <iostream>

#include "glm/gtc/type_ptr.hpp"
#include "stb_image.h"

Texture::Texture(const std::string& filePath)
{
  stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannels;
  unsigned char* data =
    stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

  if (data)
    std::cout << "Loaded texture from " << filePath << std::endl;

  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  GLenum format = 0;
  if (nrChannels == 1)
    format = GL_RED;
  else if (nrChannels == 3)
    format = GL_RGB;
  else if (nrChannels == 4)
    format = GL_RGBA;
  glTexImage2D(
    GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);
}

void Texture::Bind(int slot)
{
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, ID);
}