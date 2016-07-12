#include "main\FrameEvent.h"
#include "main\Controller.h"

namespace nTiled {

FrameEvent::FrameEvent(unsigned long frame ) : 
  frame(frame) {
}


SetDrawMethodEvent::SetDrawMethodEvent(unsigned long frame,
                                       DrawMethod* draw_method) :
  FrameEvent(frame),
  draw_method(draw_method) {
}

void SetDrawMethodEvent::execute(Controller* controller) {
  controller->setDrawMethod(this->draw_method);
}


ExportLoggingDataEvent::ExportLoggingDataEvent(unsigned long frame) :
  FrameEvent(frame) { }

void ExportLoggingDataEvent::execute(Controller* controller) {
  controller->exportLogData();
}


ExitEvent::ExitEvent(unsigned long frame) :
  FrameEvent(frame) { }

void ExitEvent::execute(Controller* controller) {
  controller->exit();
}


} // nTiled