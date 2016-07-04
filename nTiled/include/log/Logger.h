#pragma once

// ----------------------------------------------------------------------------
//  System Headers
// ----------------------------------------------------------------------------
#include <string>
#include <map>
#include <vector>

// TODO windows dependency, add alternative for UNIX 
#include <Windows.h>

namespace nTiled {
namespace log {

/*!
 Global Logger to be created by the main function to keep track of the 
 different functions

 It supports the starting and ending of logs
 */
class ExecutionTimeLogger {
public:
  // --------------------------------------------------------------------------
  //  Constructor || Destructor
  // --------------------------------------------------------------------------
  ExecutionTimeLogger();

  ~ExecutionTimeLogger();

  // --------------------------------------------------------------------------
  //  Member functions
  // --------------------------------------------------------------------------
  /*! 
   start a logging time attributed to the given function_id

   Param:
     function_id (std::string): the id of the function to be logged
   */
  void startLog(std::string function_id);

  /*! 
   End timing the current function.
   */
  void endLog();

  /*!
   Increment the current frame of this ExecutionTimeLogger
   */
  void incrementFrame();

  /*! 
   Export the collected data to the json file specified with path

   Param:
     path (const std::string&): reference to the path of the json file
   */
  void exportLog(const std::string& path);

private:
  /*! Whether this ExecutionTimeLogger is logging a function. */
  bool is_running;

  /*! Collected Data Per frame.*/
  std::vector<std::map<std::string, LARGE_INTEGER>> time_data;

};

} // log
} // nTiled