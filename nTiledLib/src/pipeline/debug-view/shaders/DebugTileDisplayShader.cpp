#include "pipeline\debug-view\shaders\DebugTileDisplayShader.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\shader-util\LoadShaders.h"
#include "pipeline\pipeline-util\ConstructQuad.h"

// ----------------------------------------------------------------------------
//  #define
// ----------------------------------------------------------------------------
#define VERT_SHADER_PATH_TILE std::string("./src/pipeline/debug-view/shaders-glsl/tile-display/tile.vert")
#define FRAG_SHADER_PATH_TILE std::string("./src/pipeline/debug-view/shaders-glsl/tile-display/tile.frag")

#define VERT_SHADER_PATH_GRID std::string("./src/pipeline/debug-view/shaders-glsl/tile-display/grid.vert")
#define FRAG_SHADER_PATH_GRID std::string("./src/pipeline/debug-view/shaders-glsl/tile-display/grid.frag")

namespace nTiled {
namespace pipeline {

DebugTileDisplayShader::DebugTileDisplayShader(GLfloat z_value,
                                               const state::View& view,
                                               const world::World& world,
                                               glm::uvec2 tile_size) : 
    DebugShader(z_value),
    view(view),
    world(world),
    projector(BoxProjector()),
    tiled_light_manager(TiledLightManager(world, view,
                                          tile_size.x, tile_size.y,
                                          projector)) {
  // Load Shaders 
  // --------------------------------------------------------------------------
  // Quad shader
  // -----------
  // Vertex Shader
  std::stringstream vert_tile_buffer = readShader(VERT_SHADER_PATH_TILE);
  GLuint vert_tile_shader = compileShader(GL_VERTEX_SHADER,
                                          vert_tile_buffer.str());

  // Fragment Shader
  std::stringstream frag_tile_buffer = readShader(FRAG_SHADER_PATH_TILE);
  GLuint frag_tile_shader = compileShader(GL_FRAGMENT_SHADER,
                                            frag_tile_buffer.str());

  this->tile_shader_program = createProgram(vert_tile_shader, 
                                            frag_tile_shader);

  // Grid shader
  // -----------
  std::stringstream vert_grid_buffer = readShader(VERT_SHADER_PATH_GRID);
  GLuint vert_grid_shader = compileShader(GL_VERTEX_SHADER,
                                          vert_grid_buffer.str());

  // Fragment Shader
  std::stringstream frag_grid_buffer = readShader(FRAG_SHADER_PATH_GRID);
  GLuint frag_grid_shader = compileShader(GL_FRAGMENT_SHADER,
                                          frag_grid_buffer.str());

  this->grid_shader_program = createProgram(vert_grid_shader,
                                            frag_grid_shader);

  // Setup PipelineObjects
  // --------------------------------------------------------------------------
  float ndc_x_tile = (float) this->tiled_light_manager.light_grid.tile_width /
                     (float) this->tiled_light_manager.light_grid.total_width * 2;
  float ndc_y_tile = (float) this->tiled_light_manager.light_grid.tile_height /
                     (float) this->tiled_light_manager.light_grid.total_height * 2;

  // Quad Objects
  // ------------
  for (unsigned int y_cursor = 0; y_cursor < this->tiled_light_manager.light_grid.n_y; y_cursor++) {
    for (unsigned int x_cursor = 0; x_cursor < this->tiled_light_manager.light_grid.n_x; x_cursor++) {
      glm::vec2 lower_bottom = glm::vec2(x_cursor * ndc_x_tile - 1.0f,
                                         y_cursor* ndc_y_tile - 1.0f);
      glm::vec2 upper_right = glm::vec2((x_cursor + 1) * ndc_x_tile - 1.0f,
                                        (y_cursor + 1) * ndc_y_tile - 1.0f);

      this->tiles.push_back(constructQuad(lower_bottom,
                                          upper_right,
                                          this->z_value));
    }
  }

  // Grid Object
  // -----------
  int n_grid_lines = this->tiled_light_manager.light_grid.n_x +
                     this->tiled_light_manager.light_grid.n_y - 2;
  int n_grid_vertices = 3 * 2 * n_grid_lines;

  GLfloat* grid_vertices = new GLfloat[n_grid_vertices];

  int n_grid_elements = 2 * n_grid_lines;
  GLushort* grid_elements = new GLushort[n_grid_elements];

  GLfloat* grid_vertices_horizontal = grid_vertices;
  GLfloat* grid_vertices_vertical = &grid_vertices[3 * n_grid_lines];

  GLushort* grid_elements_horizontal = grid_elements;
  GLushort* grid_elements_vertical = &grid_elements[n_grid_lines];

  // horizontal lines
  for (unsigned int i = 0; i < this->tiled_light_manager.light_grid.n_y - 1; i++) {
    // left point
    grid_vertices_horizontal[6 * i + 0] = -1.0f;
    grid_vertices_horizontal[6 * i + 1] = (i + 1) * ndc_y_tile - 1;
    grid_vertices_horizontal[6 * i + 2] = -.8f;
    // right point
    grid_vertices_horizontal[6 * i + 3] = 1.0f;
    grid_vertices_horizontal[6 * i + 4] = (i + 1) * ndc_y_tile - 1;
    grid_vertices_horizontal[6 * i + 5] = -.8f;

    // elements
    grid_elements_horizontal[2 * i + 0] = 2 * i + 0;
    grid_elements_horizontal[2 * i + 1] = 2 * i + 1;
  }

  // vertical lines
  for (unsigned int i = 0; i < this->tiled_light_manager.light_grid.n_x - 1; i++) {
    // top point
    grid_vertices_vertical[6 * i + 0] = (i + 1) * ndc_x_tile - 1;
    grid_vertices_vertical[6 * i + 1] = 1.0f;
    grid_vertices_vertical[6 * i + 2] = -.8f;
    // bottom point
    grid_vertices_vertical[6 * i + 3] = (i + 1) * ndc_x_tile - 1;
    grid_vertices_vertical[6 * i + 4] = -1.0f;
    grid_vertices_vertical[6 * i + 5] = -.8f;

    // elements
    grid_elements_vertical[2 * i + 0] = 2 * i + 0 + n_grid_lines;
    grid_elements_vertical[2 * i + 1] = 2 * i + 1 + n_grid_lines;
  }

  GLuint vbo_handles[2];
  glGenBuffers(2, vbo_handles);

  GLuint position_buffer = vbo_handles[0];
  GLuint element_buffer = vbo_handles[1];

  // setup vertex array buffer grid objects
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  // position buffer
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
  glBufferData(GL_ARRAY_BUFFER,
               n_grid_vertices * sizeof(GLfloat),
               grid_vertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  // element buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               n_grid_elements * sizeof(GLushort),
               grid_elements,
               GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_UNSIGNED_SHORT, GL_FALSE, 0, NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // add to this LightTilesDisplay
  this->p_grid = new PipelineObject(vao,
                                    element_buffer,
                                    n_grid_elements,
                                    glm::mat4(1.0f));


}

DebugTileDisplayShader::~DebugTileDisplayShader() {
  delete this->p_grid;

  for (PipelineObject* p_obj : this->tiles) {
    delete p_obj;
  }
}

void DebugTileDisplayShader::render() {
  this->drawTiles();
  this->drawGrid();
}

void DebugTileDisplayShader::drawGrid() {
  glUseProgram(this->grid_shader_program);

  glBindVertexArray(this->p_grid->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
               this->p_grid->element_buffer);
  glDrawElements(GL_LINES,
                 this->p_grid->n_elements,
                 GL_UNSIGNED_SHORT, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}

void DebugTileDisplayShader::drawTiles() {
  this->tiled_light_manager.constructGridFrame();

  glUseProgram(this->tile_shader_program);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  int max_lights = 0;
  int n_light_tile = 0;
  for (unsigned int i = 0; i < this->tiled_light_manager.light_grid.n_tiles; i++) {
    n_light_tile = this->tiled_light_manager.light_grid.grid[i].y;
    if (n_light_tile > max_lights) {
      max_lights = n_light_tile;
    }
  }

  if (max_lights == 0) {
    max_lights = 1;
  }

  int cursor = 0;
  for (PipelineObject* p_obj : this->tiles) {
    GLfloat fraction = (float) this->tiled_light_manager.light_grid.grid[cursor].y / 
      (float) max_lights;

    GLint p_colour_fraction = glGetUniformLocation(this->tile_shader_program,
                                                   "colour_intensity");

    glUniform1f(p_colour_fraction, fraction);

    // calculate colour of the tile
    // bind vertex array object
    glBindVertexArray(p_obj->vao);

    // render quad
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 p_obj->element_buffer);
    glDrawElements(GL_TRIANGLES,
                   p_obj->n_elements,
                   GL_UNSIGNED_SHORT, 0);
    cursor++;
  }
  glBindVertexArray(0);
  glUseProgram(0);
  glDisable(GL_BLEND);
}

} // pipeline
} // nTiled