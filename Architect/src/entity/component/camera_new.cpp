#include "camera_new.h"

#include "../../gfx/gui/gui_s.h"
#include "../../entity/component/mesh.h"

namespace archt {


	Camera_new::Camera_new() {

	}

	Camera_new::Camera_new(float fov, float aspect, float near, float far) {
		projection = Transform(glm::perspective(fov, aspect, near, far));
		view = Transform();

		translate({ 0.0f, 0.0f, -1.0f });


		auto lambda = [this](bool* open, GuiWindow_s* handle) {
			ImGui::Begin("Camera");

			ImGui::Text("Matrix:");
			glm::mat4 m = view.getMatrix();

			for (int i = 0; i < 4; i++) {
				ImGui::Text("%f\t%f\t%f\t%f", m[i][0], m[i][1], m[i][2], m[i][3]);
			}
			ImGui::Text("Position:\t%f\t%f\t%f", m[3][0], m[3][1], m[3][2]);
			


			ImGui::End();
		};
		Gui_s::getInstance()->addGuiWindow_void(lambda);

	}

	Camera_new::~Camera_new() {

	}

	void Camera_new::translate(const glm::vec3& t) {
		view.translate(t);
	}

	void Camera_new::rotate(float angle, const glm::vec3& axis) {
		view.rotate(angle, axis);
	}

	void Camera_new::resetMatrix() {
		view.reset();
	}

	

	





}
