#include "state\State.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
// File handling
#include <fstream>
#include <string>
#include <vector>

// Json include
#include <rapidjson\document.h>

#include <glm/gtc/matrix_transform.hpp>

#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

void nTiled::state::parseLights(const std::string& path,
                               nTiled::world::LightConstructor& constructor) {
#ifdef DEBUG
	std::cout << "      file: " << path << std::endl;
	std::cout << "          loading ... ";
#endif // DEBUG
	std::ifstream ifs(path);
	std::string lights_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	// parse json
#ifdef DEBUG
	std::cout << "DONE" << std::endl;
#endif // DEBUG

#ifdef DEBUG
	std::cout << "          parsing ... ";
#endif // DEBUG
	rapidjson::Document lights;
	lights.Parse(lights_file.c_str());
#ifdef DEBUG
	std::cout << "DONE" << std::endl;
#endif // DEBUG

#ifdef DEBUG
	std::cout << "          constructing lights ... ";
#endif // DEBUG

	unsigned int i = 0;

	auto& lights_array_json = lights["lights"];
	for (rapidjson::Value::ConstValueIterator itr = lights_array_json.Begin();
	     itr != lights_array_json.End();
		 ++itr) {

        // Read position
		auto& position_json = (*itr)["position"];
		glm::vec3 position = glm::vec3(position_json["x"].GetFloat(),
		                               position_json["y"].GetFloat(),
		                               position_json["z"].GetFloat());

		// Read colour
		auto& intensity_json = (*itr)["intensity"];
		glm::vec3 intensity = glm::vec3(intensity_json["r"].GetFloat(),
		                                intensity_json["g"].GetFloat(),
		                                intensity_json["b"].GetFloat());

		// Read radius
		float radius = (*itr)["radius"].GetFloat();

		constructor.add("point_light" + std::to_string(i),
                    glm::vec4(position, 1.0f),
                    intensity, 
                    radius, 
                    true);
		i++;
	}
#ifdef DEBUG
	std::cout << i << " constructed" << std::endl;
#endif // DEBUG
}