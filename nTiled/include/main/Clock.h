#pragma once

namespace nTiled {

/*!
 Clock object to keep as reference for frames used in nTiled
 */
class Clock {
public:
  // --------------------------------------------------------------------------
  //  Constructors
  // --------------------------------------------------------------------------
  /*! 
   Create a new Clock object with a starting counter of frame_i.

   Param:
     frame_i (unsigned long): the starting counter value of this Clock

   Post:
     | (new this).getCurrentFrame() == frame_i
   */
  Clock(unsigned long frame_i);

  /*!
   Create a new Clock object with starting counter of 0

   Post:
     | (new this).getCurrentFrame() == 0
   */
  Clock();

  // --------------------------------------------------------------------------
  // Methods
  // --------------------------------------------------------------------------
  /*!
   Get the current frame of this Clock object.
   */
  unsigned long getCurrentFrame() const;

  /*!
   Increment this Clock by one.
   */
  void incrementFrame();
  
private:
  /*! Current frame of this Clock object. */
  unsigned long frame_i;
};


}
