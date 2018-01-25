#include "main\FrameEvent.h"
#include "main\Controller.h"

namespace nTiled {

// ----------------------------------------------------------------------------
// FrameEvent
FrameEvent::FrameEvent(unsigned long frame,
                       unsigned long index) : 
  frame(frame),
  index(index) {
}


// ----------------------------------------------------------------------------
// SetDrawMethodEvent
SetDrawMethodEvent::SetDrawMethodEvent(unsigned long frame,
                                       unsigned long index,
                                       DrawMethod* draw_method) :
  FrameEvent(frame, index),
  draw_method(draw_method) {
}

void SetDrawMethodEvent::execute(Controller* controller) {
  controller->setDrawMethod(this->draw_method);
}


// ----------------------------------------------------------------------------
//  ToggleLoggingDataEvent
ToggleLoggingDataEvent::ToggleLoggingDataEvent(unsigned long frame,
                                               unsigned long index,
                                               bool activate) :
  FrameEvent(frame, index),
  activate(activate) {
}


void ToggleLoggingDataEvent::execute(Controller* controller) {
  controller->toggleLoggingData(this->activate);
}


// ----------------------------------------------------------------------------
//  ExportLoggingDataEvent
ExportLoggingDataEvent::ExportLoggingDataEvent(unsigned long frame,
                                               unsigned long index) :
  FrameEvent(frame, index) { }

void ExportLoggingDataEvent::execute(Controller* controller) {
  controller->exportLogData();
}


// ----------------------------------------------------------------------------
//  ToggleLoggingCalculationsEvent
ToggleLoggingLightCalculationsEvent::ToggleLoggingLightCalculationsEvent(
    unsigned long frame,
    unsigned long index,
    bool activate) :
  FrameEvent(frame, index),
  activate(activate) {
}


void ToggleLoggingLightCalculationsEvent::execute(Controller* controller) {
  controller->toggleLoggingLightCalculations(this->activate);
}


// ----------------------------------------------------------------------------
//  ExportLoggingDataEvent
ExportLoggingLightCalculationsDataEvent::ExportLoggingLightCalculationsDataEvent(
    unsigned long frame,
    unsigned long index) :
  FrameEvent(frame, index) { }

void ExportLoggingLightCalculationsDataEvent::execute(Controller* controller) {
  controller->exportLightCalculationsLogData();
}


// ----------------------------------------------------------------------------
//  ExitEvent
ExitEvent::ExitEvent(unsigned long frame, unsigned long index) :
  FrameEvent(frame, index) { }

void ExitEvent::execute(Controller* controller) {
  controller->exit();
}


} // nTiled