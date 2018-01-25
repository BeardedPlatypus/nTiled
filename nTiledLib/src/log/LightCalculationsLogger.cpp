#include "log\LightCalculationsLogger.h"

#include <vector>
#include <iostream>
#include <fstream>

// Json include
#include <rapidjson\document.h>
#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>


namespace nTiled {
namespace logged {


// ----------------------------------------------------------------------------
//  Constructor | Destructor
// ----------------------------------------------------------------------------
LightCalculationsLogger::LightCalculationsLogger(const Clock& clock,
                                                 std::string output_path,
                                                 unsigned int width,
                                                 unsigned int height) :
  clock(clock),
  output_path(output_path),
  width(width), 
  height(height),
  is_active(false) {

  // Create FBO
  // --------------------------------------------------------------------------
  glGenFramebuffers(1, &(this->p_fbo));
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, p_fbo);

  // Create Calculation Texture
  // --------------------------------------------------------------------------
  glGenTextures(1, &(this->p_calc_texture));
  glGenTextures(1, &(this->p_depth_texture));

  glBindTexture(GL_TEXTURE_2D, this->p_calc_texture);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_R32UI,
               this->width, this->height,
               0,
               GL_RED_INTEGER,
               GL_UNSIGNED_INT,
               NULL);


  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Attach texture the framebuffer object
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,    // target
                          GL_COLOR_ATTACHMENT0,  // attachment
                          GL_TEXTURE_2D,         // textarget
                          this->p_calc_texture,  // texture
                          0);                    // level (mipmap level)
                           

  // Setup Depth buffer
  // ------------------------------------------------------------------------
  glBindTexture(GL_TEXTURE_2D, this->p_depth_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_DEPTH_COMPONENT32F,
               this->width, this->height,
               0,
               GL_DEPTH_COMPONENT,
               GL_FLOAT,
               NULL
               );
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                         GL_DEPTH_ATTACHMENT,
                         GL_TEXTURE_2D,
                         this->p_depth_texture,
                         0
                         );

  // Draw buffers
  // ------------------------------------------------------------------------
  GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, draw_buffers);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
      std::cout << "framebuffer incomplete attachment" << std::endl;
    } else if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
      std::cout << "framebuffer incomplete missing attachment" << std::endl;
    } else if (status == GL_FRAMEBUFFER_UNSUPPORTED) {
      std::cout << "framebuffer unsupported" << std::endl;
    }
    throw std::runtime_error(std::string("FB error, status: 0x%x\n", status));
  }

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}


LightCalculationsLogger::~LightCalculationsLogger() { }


void LightCalculationsLogger::activate() {
  if (this->is_active) {
      throw std::runtime_error(std::string("ExecutionTimeLogger is already active."));
  }
  this->is_active = true;
}


void LightCalculationsLogger::deactivate() {
  if (!this->is_active) {
      throw std::runtime_error(std::string("ExecutionTimeLogger is not yet active."));
  }
  this->is_active = false;
}


// ----------------------------------------------------------------------------
//  Interaction Methods
// ----------------------------------------------------------------------------
void LightCalculationsLogger::preRender() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->p_fbo);
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
}


void LightCalculationsLogger::postRender() {
  glDisable(GL_BLEND);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}


void LightCalculationsLogger::extractCalculations() {
  if (this->is_active) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER,
                      this->p_fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glActiveTexture(GL_TEXTURE0);

    // get data from the texture
    std::vector<GLuint> light_calculations = std::vector<GLuint>(this->width * this->height);
    glBindTexture(GL_TEXTURE_2D, this->p_calc_texture);
    glGetTexImage(GL_TEXTURE_2D,
                  0,
                  GL_RED_INTEGER,
                  GL_UNSIGNED_INT,
                  light_calculations.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    /*
    // export data to disk
    std::string file_path = this->output_path +
      ("/frame_" + std::to_string(clock.getCurrentFrame()) + ".dat");

    std::ofstream frame_file;
    frame_file.open(file_path, std::ios::out | std::ios::trunc);

    for (GLuint val : light_calculations) {
      frame_file << val << ";";
    }

    frame_file.close();
    */
    glBindFramebuffer(GL_READ_FRAMEBUFFER,
                      0);

    unsigned long summed_calculations = 0;
    

    for (unsigned int i = 0; i < this->width * this->height; ++i) {
      // divide by 10 if attenuated
      summed_calculations += light_calculations.at(i);
    }

    this->n_light_calc_data.push_back(std::pair<unsigned long, unsigned long>(this->clock.getCurrentFrame(),
                                                                              summed_calculations));
  }
}


void LightCalculationsLogger::exportLog() {
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);

  writer.StartObject(); // base object
  writer.Key("frames");
  writer.StartArray();
    
    // build frame data
    for (std::pair<unsigned long, unsigned long>& frame : this->n_light_calc_data) {
      writer.StartObject();
      writer.Key("frame");
      writer.Uint64(frame.first);

      writer.Key("n_calc");
      writer.Uint64(frame.second);
      writer.EndObject();
    }

  writer.EndArray();
  writer.EndObject();

  // Write to path
  std::ofstream output_stream;
  output_stream.open(this->output_path);
  output_stream << s.GetString();
  output_stream.close();
}


}
}