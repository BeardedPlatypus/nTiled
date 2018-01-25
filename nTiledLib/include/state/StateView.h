/*! @file StateView.h
 *  @brief StateView.h contains all definitions related to the state of the 
 *         view. 
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "camera\Camera.h"


namespace nTiled { 
namespace state {

/*! @brief The OutputType of this nTiled execution
 * 
 * Display corresponds with displaying the frames 
 * on the display only.
 * 
 * Memory writes each frame to memory as well as
 * displaying it on the display.
 */
enum class OutputType {
  Display,
  Memory,
};


/*! @brief ViewOutput holds all data regarding the Output of the view.
 */
struct ViewOutput {
  // --------------------------------------------------------------------------
  // Constructors
  // --------------------------------------------------------------------------
  /*! @brief Construct a new ViewOutput with OutputType::Memory and the given
   *         output parameters.
   * 
   * @param image_base_path Path to the directory in which each frame is 
   *                        stored.
   * @param frame_start Frame at which nTiled starts writing frames to storage
   * @param frame_end Frame at which nTiled ceases writing frames to storage
   */
  ViewOutput(const std::string& image_base_path,
             const unsigned int frame_start,
             const unsigned int frame_end);

  /*! @brief Construct a new ViewOutput with OutputType::Display
   */
  ViewOutput();

  // --------------------------------------------------------------------------
  // Member elements
  // --------------------------------------------------------------------------
  /*! @brief The OutputType of this ViewOutput */
  OutputType type;
  /*! @brief Path to the directory in which each frame is stored. */
  const std::string image_base_path;
  /*! Frame at which nTiled starts writing frames to storage. */
  const unsigned int frame_start;
  /*! Frame at which nTiled ceases writing frames to storage. */
  const unsigned int frame_end;
};


/*! @brief View contains all attributes related to the view of the State
 */
struct View {
  // --------------------------------------------------------------------------
  //  Constructor | Destructor
  // --------------------------------------------------------------------------
  /*! @brief Construct a new View with the given parameters. 
   * 
   * @param camera The camera of this new View
   * @param camera_control Pointer to the CameraControl of this new View
   * @param viewport The dimensions of this new View in pixels
   * @param output Pointer to the ViewOutput of this new view
   */
  View(camera::Camera camera,
       camera::CameraControl* camera_control,
       glm::uvec2 viewport,
       ViewOutput* output,
       bool display_light_calculations);

  /*! @brief Destructor of this View. 
   */
	~View();

  // --------------------------------------------------------------------------
	// Member elements
  // --------------------------------------------------------------------------
  /*! @brief Camera of this View. */
	camera::Camera camera;
  /*! @brief Pointer to the CameraControl of this View. */
	camera::CameraControl* camera_control;
  /*! @brief dimensions of the viewport of this View in pixels. */
	glm::uvec2 viewport;

  /*! @brief Pointer to the ViewOutput of this View. */
  const ViewOutput* output;

  bool display_light_calculations;
};


} // state
} // nTiled