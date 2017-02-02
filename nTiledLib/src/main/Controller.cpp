#include "main\Controller.h"

#include "pipeline\forward\ForwardPipeline.h"
#include "pipeline\forward\ForwardPipelineLogged.h"

#include "pipeline\deferred\DeferredPipeline.h"
#include "pipeline\deferred\DeferredPipelineLogged.h"

#include "pipeline\debug-view\DebugPipeline.h"

namespace nTiled {

void key_callback(GLFWwindow* window, 
                  int key, 
                  int scancode, 
                  int action, 
                  int mode);


// ----------------------------------------------------------------------------
//  constructor
// ----------------------------------------------------------------------------
Controller::Controller(const std::string& scene_path) : 
    scene_path(scene_path),
    draw_methods(std::vector<DrawMethod*>()),
    event_queue(std::priority_queue<FrameEvent*,
                                    std::vector<FrameEvent*>,
                                    FrameEventCompare>()),
    should_close(false) {
  this->draw_methods.push_back(new DrawToView());
  this->current_draw_method = this->draw_methods[0];
}


Controller::~Controller() {
  for (DrawMethod* dm : this->draw_methods) {
    delete dm;
  }

  delete p_logger;
  delete p_gui_manager;
  delete p_pipeline;
  delete p_state;
}

// ----------------------------------------------------------------------------
//  Initialisation
// ----------------------------------------------------------------------------
void Controller::initialise() {
  this->initialiseState();
  this->initialiseOpenGL();
  this->initialiseNTiledComponents();
  this->initialiseFrameEvents();
}


void Controller::initialiseState() {
  this->p_state = state::constructStateFromJson(this->scene_path);
}


void Controller::initialiseOpenGL() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  this->p_window = glfwCreateWindow(this->p_state->view.viewport.x, // width
                                    this->p_state->view.viewport.y, // height
                                    "nTiled - openGL viewer",    // name
                                     NULL,                        // p_monitor
                                     NULL);                       // p_share
  glfwMakeContextCurrent(this->p_window);

  if (this->p_window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    this->exit();
  }

  // Set Key callback
  // ----------------
  glfwSetKeyCallback(this->p_window, key_callback);

  // load glad
  // ---------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize OpenGL context" << std::endl;
    this->exit();
  }

  glViewport(0, 0, this->p_state->view.viewport.x, this->p_state->view.viewport.y);
}



void Controller::initialiseNTiledComponents() {
  // Logging attributes
  // --------------------------------------------------------------------------
  this->clock = Clock();
  this->p_logger = new logged::ExecutionTimeLogger(this->clock, 
                                                   this->p_state->log.frame_start,
                                                   this->p_state->log.frame_end);

  nTiled::pipeline::Pipeline* p_render_pipeline;
  if (this->p_state->shading.pipeline_type == nTiled::pipeline::PipelineType::Forward) {
    if (!(this->p_state->log.is_logging_data)) {
      p_render_pipeline = new pipeline::ForwardPipeline(*(this->p_state));
    } else {
      p_render_pipeline = new nTiled::pipeline::ForwardPipelineLogged(*(this->p_state), 
                                                                      *(this->p_logger));
    }
  } else {
    if (!this->p_state->log.is_logging_data) {
      p_render_pipeline = new nTiled::pipeline::DeferredPipeline(*(this->p_state));
    } else {
      p_render_pipeline = new nTiled::pipeline::DeferredPipelineLogged(*(p_state), 
                                                                       *(p_logger));
    }
  }

  p_render_pipeline->initialiseShaders();

  if (this->p_state->shading.is_debug) {
    this->p_pipeline = new pipeline::DebugPipeline(p_render_pipeline,
                                                   *(this->p_state));
  } else {
    this->p_pipeline = p_render_pipeline;
  }


  this->p_gui_manager = new gui::GuiManager(*(this->p_state));
  this->p_gui_manager->init(*(this->p_window));
}


void Controller::initialiseFrameEvents() {
  unsigned long index = 0;
  // --------------------------------------------------------------------------
  // Change DisplayType
  if (this->p_state->view.output->type == state::OutputType::Memory) {
    // construct new DrawMethod
    DrawMethod* draw_to_memory =new DrawToMemory(this->p_state->view.output->image_base_path,
                                                 this->p_state->view);
    this->draw_methods.push_back(draw_to_memory);

    // push both switch events to the event queue
    this->event_queue.push(new SetDrawMethodEvent(this->p_state->view.output->frame_start,
                                                  index,
                                                  draw_to_memory));
    index += 1;
    this->event_queue.push(new SetDrawMethodEvent(this->p_state->view.output->frame_end,
                                                  index,
                                                  this->draw_methods[0]));
    index += 1;
  }

  // --------------------------------------------------------------------------
  // Export logging
  if (this->p_state->log.is_logging_data) {
    this->event_queue.push(new ToggleLoggingDataEvent(this->p_state->log.frame_start,
                                                      index,
                                                      true));
    index += 1;
    this->event_queue.push(new ToggleLoggingDataEvent(this->p_state->log.frame_end,
                                                      index,
                                                      false));
    index += 1;
    this->event_queue.push(new ExportLoggingDataEvent(this->p_state->log.frame_end,
                                                      index));
    index += 1;
  }

  // --------------------------------------------------------------------------
  // Exit after done
  if (this->p_state->log.exit_after_done) {
    this->event_queue.push(new ExitEvent(this->p_state->log.exit_frame,
                                         index));
    index += 1;
  }
}

// ----------------------------------------------------------------------------
//  RenderLoop
// ----------------------------------------------------------------------------
void Controller::render() {
  this->renderLoopEventQueue();
  this->renderLoopDefault();
}


void Controller::renderLoopDefault() {
  while(!glfwWindowShouldClose(this->p_window) && !this->should_close ) {
    this->current_draw_method->draw(this->p_window,
                                    this->p_pipeline,
                                    this->p_gui_manager,
                                    this->p_state->view,
                                    this->clock);
    this->clock.incrementFrame();
    this->p_logger->incrementFrame();
  }

  glfwTerminate();
}


void Controller::renderLoopEventQueue() {
  while (!glfwWindowShouldClose(this->p_window) &&
         !this->should_close &&
         !this->event_queue.empty()) {
    // get the next event
    FrameEvent* frame_event = this->event_queue.top();

    // execute until next event should be executed
    while ((this->clock.getCurrentFrame() < frame_event->frame) && 
           !glfwWindowShouldClose(this->p_window) &&
           !this->should_close) {
      this->current_draw_method->draw(this->p_window,
                                      this->p_pipeline,
                                      this->p_gui_manager,
                                      this->p_state->view,
                                      this->clock);
      // increment frames
      this->clock.incrementFrame();
      this->p_logger->incrementFrame();
    }

    // execute nexte event
    frame_event->execute(this);

    this->event_queue.pop();
    delete frame_event;
  }
}


// ----------------------------------------------------------------------------
//  Execution Methods
// ----------------------------------------------------------------------------
void Controller::setDrawMethod(DrawMethod* draw_method) {
  this->current_draw_method = draw_method;
}


void Controller::toggleLoggingData(bool activate) {
  if (activate) {
    this->p_logger->activate();
  } else {
    this->p_logger->deactivate();
  }
}


void Controller::exportLogData() {
  this->p_logger->exportLog(this->p_state->log.path);
}


void Controller::exit() {
  this->should_close = true;
}


void key_callback(GLFWwindow* window, 
                  int key, 
                  int scancode, 
                  int action, 
                  int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}
} // nTiled