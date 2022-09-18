#pragma once

#include <glm/vec3.hpp>


namespace archt {



	glm::vec3 getFaceNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, bool clockWise);


}