#include "log\Logger.h"

// ----------------------------------------------------------------------------
//  System Libraries
// ----------------------------------------------------------------------------
// File handling
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Json include
#include <rapidjson\document.h>
#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>


namespace nTiled {
namespace log {

// ----------------------------------------------------------------------------
//  Constructor
// ----------------------------------------------------------------------------
ExecutionTimeLogger::ExecutionTimeLogger() :
  is_running(false),
  time_data(std::vector<std::map<std::string, double>>()) {
  // add zero frame
  this->incrementFrame();
}


// ----------------------------------------------------------------------------
//  Member functions
// ----------------------------------------------------------------------------
void ExecutionTimeLogger::startLog(std::string function_id) {
  if (this->is_running) {
    throw std::runtime_error(std::string("ExecutionTimeLogger is already running."));
  }
  if (this->time_data.back().find(function_id) != this->time_data.back().end()) {
    throw std::runtime_error(std::string("ExecutionTimeLogger has already tracked this function_id this frame."));
  }

  this->is_running = true;
  this->current_function_id = function_id;
  
  QueryPerformanceFrequency(&(this->frequency));
  QueryPerformanceCounter(&(this->start_time));
}

void ExecutionTimeLogger::endLog() {
  QueryPerformanceCounter(&(this->end_time));

  if (!this->is_running) {
    throw std::runtime_error(std::string("ExecutionTimeLogger was ended without running."));
  }
  this->is_running = false;

  double execution_time = 
    (this->end_time.QuadPart - this->start_time.QuadPart) * 1000.0 / this->frequency.QuadPart;  // in ms
  this->time_data.back().insert(std::pair<std::string, double>(this->current_function_id,
                                                             execution_time));
}

void ExecutionTimeLogger::incrementFrame() {
  std::map<std::string, double> empty_frame = 
    std::map<std::string, double>();

  this->time_data.push_back(empty_frame);
}

void ExecutionTimeLogger::exportLog(const std::string& path) {
  /* JSON layout: 
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
   */
  // Construct RapidJSON object
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);

  writer.StartObject(); // base object
  writer.Key("frames");
  writer.StartArray();

  for (std::map<std::string, double> frame : this->time_data) {
    writer.StartObject(); // start frame object
    for (auto& iter : frame) {
      writer.Key(iter.first.c_str());
      writer.Double(iter.second);
    }
    writer.EndObject(); // end frame object
  }
  
  writer.EndArray();
  writer.EndObject();

  // Write to path
  std::ofstream output_stream;
  output_stream.open(path);
  output_stream << s.GetString();
  output_stream.close();
}

} // log
} // nTiled