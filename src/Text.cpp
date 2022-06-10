#include "Text.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

static Shader* shader;
static VertexArray* VAO;
static Layout layout;
static VertexBuffer* VBO;
static GLFWwindow* window;

struct Character
{
  Texture texture;    // ID handle of the glyph texture
  glm::ivec2 size;    // Size of glyph
  glm::ivec2 bearing; // Offset from baseline to left/top of glyph
  long advance;       // Offset to advance to next glyph
};

static std::map<char, Character> characters;

int Text::init(GLFWwindow* window_)
{
  window = window_;
  FT_Library ft;
  FT_Face face;
  if (FT_Init_FreeType(&ft)) {
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library"
              << std::endl;
    return -1;
  }
  if (FT_New_Face(ft, "res/fonts/arial.ttf", 0, &face)) {
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    return -1;
  }
  FT_Set_Pixel_Sizes(face, 0, 48);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
  for (unsigned char c = 0; c < 128; c++) {

    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      continue;
    }

    Texture texture(GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    face->glyph->bitmap.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    Character character{
      texture,
      glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
      glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
      face->glyph->advance.x
    };
    characters.insert(std::make_pair(c, character));
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  shader = new Shader("res/shaders/text.vert", "res/shaders/text.frag");
  VAO = new VertexArray;
  VAO->Bind();
  layout.add(GL_FLOAT, 2, "pos").add(GL_FLOAT, 2, "tex");
  VBO = new VertexBuffer(layout);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  return 0;
}

void Text::render(const std::string& text,
                  float x,
                  float y,
                  float scale,
                  const glm::vec3& color)
{
  shader->Bind();
  shader->setVec3("textColor", color);
  glActiveTexture(GL_TEXTURE0);
  VAO->Bind();

  glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT);

  shader->setMat4("projection", projection);

  for (auto c = text.begin(); c != text.end(); c++) {
    Character& ch = characters.at(*c);

    float xpos = x + ch.bearing.x * scale;
    float ypos = y - (ch.size.y - ch.bearing.y) * scale;

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;
    // update VBO for each character
    float vertices[6][4] = { { xpos, ypos + h, 0.0f, 0.0f },
                             { xpos, ypos, 0.0f, 1.0f },
                             { xpos + w, ypos, 1.0f, 1.0f },

                             { xpos, ypos + h, 0.0f, 0.0f },
                             { xpos + w, ypos, 1.0f, 1.0f },
                             { xpos + w, ypos + h, 1.0f, 0.0f } };

    // render glyph texture over quad
    ch.texture.Bind(0);
    VBO->Bind();
    VBO->subData(vertices, 0, sizeof(vertices));

    glDrawArrays(GL_TRIANGLES, 0, 6);
    // now advance cursors for next glyph (note that advance is number of 1/64
    // pixels)
    x += (ch.advance >> 6) * scale;
    // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount
    // of 1/64th pixels by 64 to get amount of pixels))
  }
}