/*! @file FrameEvent.h
 *  @brief FrameEvent.h contains all the definitions of FrameEvents used in 
 *        nTiled.
 */
#pragma once

#include "DrawMethod.h"

namespace nTiled {

class Controller;


/*! @brief Base class for all FrameEvents, 
 *         it contains a frame at which the frame event should be executed 
 *         as well as an index in case frames are executed in the same frame.
 */
class FrameEvent {
public:
  /*! @brief Construct a new FrameEvent with the given frame and index
   * 
   * @param frame The frame at which this FrameEvent should be executed
   * @param index The index of this FrameEVent
   */
  FrameEvent(unsigned long frame,
             unsigned long index);

  /*! @brief Destruct this FrameEvent. 
   */
  ~FrameEvent() {}

  /*! @brief Execute this FrameEvent by modifying the specified Controller.
   * 
   * @param controller Pointer to the controller on which this FrameEvent 
   *                   operates.
   */
  virtual void execute(Controller* controller) { }

  /*! @brief Frame at which this FrameEvent should start taking place. */
  const unsigned long frame;
  /*! @brief Index of this FrameEvent. */
  const unsigned long index;
};


/*! @brief FrameEventCompare is the Comparator responsible for comparing 
 *         which FrameEvent should occur first.
 */
class FrameEventCompare {
public:
  constexpr bool operator() (FrameEvent* lhs, FrameEvent* rhs) const {
    return ((lhs->frame > rhs->frame) || 
            ((lhs->frame == rhs->frame) && (lhs->index > rhs->index)));
  }
};


/*! @brief The SetDrawMethodEvent sets the DrawMethod of the controller
 *         specified during execution to the DrawMethod with which it
 *         was initiated.
 */
class SetDrawMethodEvent : public FrameEvent {
public:
  /*! @brief Construct a new SetDrawMethodEvent with the given parameters. 
   * 
   * @param frame The frame at which this FrameEvent should be executed
   * @param index The index of this FrameEVent
   * @param draw_method Pointer to the DrawMethod this FrameEvent should
   *                    set the DrawMethod in the Controller to.
   */
  SetDrawMethodEvent(unsigned long frame,
                     unsigned long index,
                     DrawMethod* draw_method);

  /*! @brief Execute this FrameEvent by setting the DrawMethod 
   *         of the specified Controller to draw_method.
   *
   * @param controller Pointer to the controller on which this FrameEvent 
   *                   operates.
   */
  virtual void execute(Controller* controller);

private:
  /*! @brief Pointer to the DrawMethod that this FrameEvent sets the 
   *         Controller to. */
  DrawMethod* draw_method;
};


/*! @brief ToggleLoggingDataEvent is responsible for turning 
 *         the tracking of execution time within the Controller on and off.
 */
class ToggleLoggingDataEvent : public FrameEvent {
public:
  /*! @brief Construct a new ToggleLoggingDataEvent with the specified 
   *         parameters
   * 
   * @param frame The frame at which this FrameEvent should be executed
   * @param index The index of this FrameEVent
   * @param activate Whether this ToggleLoggingDataEvent should turn logging
   *                 on or off.
   */
  ToggleLoggingDataEvent(unsigned long frame,
                         unsigned long index,
                         bool activate);

  /*! @brief Execute this FrameEvent by setting the  tracking of data
   *         of the specified Controller to activate.
   *
   * @param controller Pointer to the controller on which this FrameEvent 
   *                   operates.
   */
  virtual void execute(Controller* controller);

private:
   /*! @brief Whether this ToggleLoggingDataEvent should turn logging on or off. */
  const bool activate;
};


/*! @brief ExportLoggingDataEvent is responsible for signalling the Controller
 *         at the specified frame to export all the tracked data within an 
 *         execution.
 */
class ExportLoggingDataEvent : public FrameEvent {
public:
  ExportLoggingDataEvent(unsigned long frame,
                         unsigned long index);

  /*! @brief Execute this FrameEvent by signalling the Controller to export 
   *         all tracked data.
   *
   * @param controller Pointer to the controller on which this FrameEvent 
   *                   operates.
   */
  virtual void execute(Controller* controller);
};


/*! @brief ExitEvent is responsible for signalling the Controller to end 
 *         the execution of nTiled at the specified frame.
 */
class ExitEvent : public FrameEvent {
public:
  ExitEvent(unsigned long frame,
            unsigned long index);

  /*! @brief Execute this FrameEvent by signalling the Controller to end
   *         the execution of nTiled.
   *
   * @param controller Pointer to the controller on which this FrameEvent 
   *                   operates.
   */
  virtual void execute(Controller* controller);
};

} // nTiled