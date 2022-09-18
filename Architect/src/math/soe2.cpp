#include "soe2.h"
#include <cmath>



namespace archt {

	SOE2::SOE2() {}

	SOE2::SOE2(const glm::vec2& a1, const glm::vec2& b1, const glm::vec2& a2, const glm::vec2& b2)
		: a1(a1), a2(a2), b1(b1), b2(b2) {

	}

	SOE2::~SOE2() {}

	bool SOE2::solve(bool print) {

		float div1 = b1.x - a1.x;
		float m1 = 0.0f;

		if (div1 != 0.0f) 
			m1 = (b1.y - a1.y) / div1;


		float div2 = b2.x - a2.x;
		float m2 = 0.0f;
		if (div2 != 0)
			m2 = (b2.y - a2.y) / div2;

		float q1 = a1.y - m1 * a1.x;
		float q2 = a2.y - m1 * a2.x;



		if (m1 == 0.0f && m2 == 0.0f) {
			if ((a1.y >= std::min(a2.y, b2.y) ||
				a1.y <= std::max(a2.y, b2.y)) &&
				a1.x == a2.x) {
				printf("a1 = %f %f \nb1 = %f %f\n", a1.x, a1.y, b1.x, b1.y);
				printf("a2 = %f %f \nb2 = %f %f\n", a2.x, a2.y, b2.x, b2.y);

				printf("m1 = %f\nq1 = %f\nm2 = %f\nq2 = %f\n-------\n", m1, q1, m2, q2); 
				return true;
			}
		}

		float x = (q1 - q2) / (m2 - m1);
		

		if (print) {
			printf("a1 = %f %f \nb1 = %f %f\n", a1.x, a1.y, b1.x, b1.y);
			printf("a2 = %f %f \nb2 = %f %f\n", a2.x, a2.y, b2.x, b2.y);

			printf("m1 = %f\nq1 = %f\nm2 = %f\nq2 = %f\n-------\n", m1, q1, m2, q2);
		}


		if (m1 == m2) {
			if (a1 == a2 && b1 == b2) {
				return true;
			}
			else {
				return false;
			}
		}

		if (m1 * x + q1 == m2 * x + q2) {
		
			return true;
		}

		return false;
	}
}