/*! @file Logger.h
 *  @brief Logger.h contains the definition of ExecutionTimeLogger used to 
 *         track the execution time of functions.
 */

#pragma once

// ----------------------------------------------------------------------------
//  System Headers
// ----------------------------------------------------------------------------
#include <string>
#include <map>
#include <vector>

// TODO windows dependency, add alternative for UNIX 
#include <Windows.h>

#include "main/Clock.h"

namespace nTiled {
namespace logged {

/*! @brief Global Logger to be created by the main function to keep track of the 
           different functions execution times.

    It supports the starting and ending of logs.
    The data can be exported to a json file, and will have the following 
    structure:

    @code{.js}
    { frames: [ { "<function_id 1>" : <function_id 1 timing> 
                , "<function_id 2>" : <function_id 2 timing>
                , ...
                }
             , { "<function_id 1>" : <function_id 1 timing> 
               , "<function_id 2>" : <function_id 2 timing>
               , ...
               }
             , ...
             ]
    }
    @endcode
 */
class ExecutionTimeLogger {
public:
  // --------------------------------------------------------------------------
  //  Constructor || Destructor
  // --------------------------------------------------------------------------
  /*! @brief Construct a new ExecutionTimeLogger with the given parameters
   *  
   *  @param clock The clock object used to track the execution time with.
   *  @param frame_start The first frame in which the time of functions should 
   *                     be tracked
   *  @param frame_end The last frame in which the time of functions should be 
   *                   tracked.
   */
  ExecutionTimeLogger(const Clock& clock,
                      unsigned int frame_start,
                      unsigned int frame_end);

  // --------------------------------------------------------------------------
  //  Member functions
  // --------------------------------------------------------------------------
  /*! @brief Start a logging time attributed to the given function_id
   * 
   *  @param function_id The id of the function to be logged
   */
  void startLog(std::string function_id);

  /*! @brief End timing the current function.
   */
  void endLog();

  /*! @brief Increment the current frame of this ExecutionTimeLogger
   */
  void incrementFrame();

  /*! @brief Export the collected data to the json file specified with path
   *
   * @param path Reference to the path of the json file
   */
  void exportLog(const std::string& path);

  /*! @brief Activate this ExecutionTimeLogger. */
  void activate();
  /*! @brief Deactivate this ExecutionTimeLogger. */
  void deactivate();

private:
  // --------------------------------------------------------------------------
  //  Data Members
  // --------------------------------------------------------------------------
  /*! @brief Collected Data Per frame.*/
  std::vector<std::pair<unsigned long,
                        std::map<std::string, double>>> time_data;

  // --------------------------------------------------------------------------
  //  Measurement Members
  // --------------------------------------------------------------------------
  /*! @brief Whether this ExecutionTimeLogger is logging a function. */
  bool is_running;
  /*! @brief Whether this ExecutionTimeLogger is active*/
  bool is_active;
  /*! @brief Function_id of function that is currently being tracked*/
  std::string current_function_id;
  /*! @brief Frequency at start of the measurement */
  LARGE_INTEGER frequency;
  /*! @brief Time at start of the measurement */
  LARGE_INTEGER start_time;
  /*! @brief Time at end of the measurement */
  LARGE_INTEGER end_time;

  /*! @brief The first frame that is logged. */
  unsigned int frame_start;
  /*! @brief The frame at which logging is stopped. */
  unsigned int frame_end;

  /*! @brief Clock object of this ExecutionTimeLogger. */
  const Clock& clock;
};

} // log
} // nTiled