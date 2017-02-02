/*! @file Clock.h
    @brief Clock.h specifies the Clock class
*/
#pragma once

namespace nTiled {

/*! @class
    @brief Clock object to keep as reference for frames used in nTiled
 */
class Clock {
public:
  // --------------------------------------------------------------------------
  //  Constructors
  // --------------------------------------------------------------------------
  /*! @brief Construct a new Clock object with a starting counter of frame_i.

      @param frame_i: The starting counter value of this Clock
      @post (new this).getCurrentFrame() == frame_i
   */
  Clock(unsigned long frame_i);

  /*! @brief Construct a new Clock object with starting counter of 0

      @post (new this).getCurrentFrame() == 0
   */
  Clock();

  // --------------------------------------------------------------------------
  // Methods
  // --------------------------------------------------------------------------
  /*! @brief Get the current frame of this Clock object.
   */
  unsigned long getCurrentFrame() const;

  /*! @brief Increment this Clock by one.
   */
  void incrementFrame();
  
private:
  /*! @brief Current frame of this Clock object. */
  unsigned long frame_i;
};


}
