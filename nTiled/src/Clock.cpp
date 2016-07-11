#include "Clock.h"

namespace nTiled {

// ----------------------------------------------------------------------------
// Constructors
// ----------------------------------------------------------------------------
Clock::Clock(unsigned long frame_i) :
  frame_i(frame_i) { 
}

Clock::Clock() :
  Clock(0) {
}


// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------
unsigned long Clock::getCurrentFrame() const {
  return this->frame_i;
}

void Clock::incrementFrame() {
  this->frame_i++;
}

}