#pragma once

#include "DrawMethod.h"

namespace nTiled {

class Controller;


class FrameEvent {
public:
  FrameEvent(unsigned long frame);
  ~FrameEvent() {}

  virtual void execute(Controller* controller) { }
  /*! Frame at which this Event should start taking place*/
  const unsigned long frame;
};


class FrameEventCompare {
public:
  constexpr bool operator() (FrameEvent* lhs, FrameEvent* rhs) const {
    return lhs->frame <= rhs->frame;
  }
};


class SetDrawMethodEvent : public FrameEvent {
public:
  SetDrawMethodEvent(unsigned long frame,
                     DrawMethod* draw_method);
  virtual void execute(Controller* controller);

private:
  DrawMethod* draw_method;
};


class ExportLoggingDataEvent : public FrameEvent {
public:
  ExportLoggingDataEvent(unsigned long frame);
  virtual void execute(Controller* controller);
};


class ExitEvent : public FrameEvent {
public:
  ExitEvent(unsigned long frame);
  virtual void execute(Controller* controller);
};



} // nTiled