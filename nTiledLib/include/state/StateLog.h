/*! @file StateLog.h
 *  @brief StateLog contains the Log struct used to store the state data of 
 *         Logging within a run of nTiled
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <string>


namespace nTiled {
namespace state {

/*! @brief Log holds all data related to logging which is passed on to the 
 *         relevant logging components
 */
struct Log {

  /*! @brief Construct a new Log object with the provided parameters
   * 
   * @param is_logging_data Whether this run of nTiled is logging data
   * @param path The path to where the logged data should be written
   * @param frame_start The frame at which logging should start
   * @param frame_end The frame at which logging should cease
   * @param exit_after_done Whether nTiled should exit after logging data
   * @param exit_frame The frame at which nTiled should exit if exit_after_done
   */
  Log(bool is_logging_data,
      std::string path,
      unsigned int frame_start,
      unsigned int frame_end,
      bool exit_after_done,
      unsigned int exit_frame);

  /*! @brief Construct a new Log object which will log data from start to end 
   *         does not exit after completing logging.
   * 
   * @param path The path to where the logged data should be written
   * @param frame_start The frame at which logging should start
   * @param frame_end The frame at which logging should cease
   */
  Log(std::string path,
      unsigned int frame_start,
      unsigned int frame_end);

  /*! @brief Construct a new Log object which instructs nTiled not to log data
   */
  Log();

  /*! @brief Whether this run of nTiled is logging data. */
  bool is_logging_data;
  /*! @brief The path to where the logged data should be written. */
  std::string path;

  /*! @brief The frame at which logging should start. */
  unsigned int frame_start;
  /*! @brief The frame at which logging should cease. */
  unsigned int frame_end;

  /*! @brief Whether the nTiled execution should exit at exit_frame. */
  bool exit_after_done;
  /*! @brief The frame at which the nTiled execution should exit 
   *         if this.exit_after_done. 
   */
  unsigned int exit_frame;
};

} // state
} // nTiled
