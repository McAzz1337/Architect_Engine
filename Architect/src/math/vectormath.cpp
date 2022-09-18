#include "vectormath.h"

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace archt {

	void print(const glm::mat3& m, const glm::vec3 r, int stage) {

		printf("Stage %i\n", stage);

		for (int i = 0; i < 3; i++) {
			printf("%f\t%f\t%f\t|\t%f\n", m[i][0], m[i][1], m[i][2], r[i]);
		}
		printf("----------------------------------\n");
	}

	glm::vec3 getFaceNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, bool clockWise) {


		glm::mat4 rot(1.0f);
		rot = glm::rotate(rot, (float) M_PI * 0.5f, { 0.0f, 1.0f, 0.0f });
		glm::vec3 a1 = glm::vec3(rot * glm::vec4(a, 1.0f));
		glm::vec3 b1 = glm::vec3(rot * glm::vec4(b, 1.0f));
		glm::vec3 c1 = glm::vec3(rot * glm::vec4(c, 1.0f));

		glm::vec3 dir = c1 - a1;
		glm::vec3 dir1 = b1 - a1;

		glm::vec3 n = glm::cross(dir, dir1);
		
		printf("normal = %f\t%f\t%f\n", n.x, n.y, n.z);

		return n;

	}





}




