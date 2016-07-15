#pragma once

#include "DrawMethod.h"

namespace nTiled {

class Controller;


class FrameEvent {
public:
  FrameEvent(unsigned long frame,
             unsigned long index);
  ~FrameEvent() {}

  virtual void execute(Controller* controller) { }
  /*! Frame at which this Event should start taking place*/
  const unsigned long frame;
  const unsigned long index;
};


class FrameEventCompare {
public:
  constexpr bool operator() (FrameEvent* lhs, FrameEvent* rhs) const {
    return ((lhs->frame > rhs->frame) || 
            ((lhs->frame == rhs->frame) && (lhs->index > rhs->index)));
  }
};


class SetDrawMethodEvent : public FrameEvent {
public:
  SetDrawMethodEvent(unsigned long frame,
                     unsigned long index,
                     DrawMethod* draw_method);
  virtual void execute(Controller* controller);

private:
  DrawMethod* draw_method;
};


class ToggleLoggingDataEvent : public FrameEvent {
public:
  ToggleLoggingDataEvent(unsigned long frame,
                         unsigned long index,
                         bool activate);
  virtual void execute(Controller* controller);

private:
  const bool activate;
};


class ExportLoggingDataEvent : public FrameEvent {
public:
  ExportLoggingDataEvent(unsigned long frame,
                         unsigned long index);
  virtual void execute(Controller* controller);
};


class ExitEvent : public FrameEvent {
public:
  ExitEvent(unsigned long frame,
            unsigned long index);
  virtual void execute(Controller* controller);
};



} // nTiled