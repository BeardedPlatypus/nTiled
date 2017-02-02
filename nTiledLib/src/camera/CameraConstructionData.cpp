#include "camera\CameraConstructionData.h"

namespace nTiled {
namespace camera {

CameraConstructionData::CameraConstructionData(glm::vec3 camera_eye,
                                               glm::vec3 camera_center,
                                               glm::vec3 camera_up,
                                               float fovy,
                                               float aspect,
                                               float z_near,
                                               float z_far) :
    camera_eye(camera_eye),
    camera_center(camera_center),
    camera_up(camera_up),
    fovy(fovy),
    aspect(aspect),
    z_near(z_near),
    z_far(z_far) { }

}
}