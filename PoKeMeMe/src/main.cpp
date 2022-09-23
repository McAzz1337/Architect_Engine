#include <iostream>


#include <core.h>



//#define SPLASH_SCREEN

int main() {
	using namespace archt;

#ifdef SPLASH_SCREEN
	Window* hWin = new Window();
#endif

	GLWindow* window = GLRenderAPI::init();
	Gui::init(window);
	Console console;
	console.createGuiWindow();
	GLRenderAPI::createGuiInfoWindow();
	system_info::createSysteminfoWindow();

	auto task = []() {
		printf("scheduled task invoked::void\n");
	};

	Scheduler::getInstance().submitTask(10000.0f, task);

	Input::init();

	Scene scene;

	ptr<Camera_new> cam;
	ptr<Camera_new> cam1;

	Entity_s camEnt = scene.createEntity();
	camEnt.addComponent<Transform_s>();
	{
		glm::ivec2 windowSize = window->getSize();
		cam = make_ptr<Camera_new>(M_PI / 3.0f, windowSize.x / windowSize.y, 0.01f, 100.0f);
		cam1 = make_ptr<Camera_new>(M_PI / 3.0f, windowSize.x / windowSize.y, 0.01f, 100.0f);
		cam1->translate({ -0.8f, 0.0f, -1.0f });
		cam1->rotate(M_PI / 12.0f, { 0.0f, 1.0f, 0.0f });
	}

#pragma region ENTITY_SETUP
	Entity_s entity = scene.createEntity();
	entity.addComponent<Transform_s>();

	{
		float u = 1.0f / 1711.0f;
		float v = 18.0f / 5609.0f;
		float sizeX = 56.0f / 1711.0f;
		float sizeY = 56.0f / 5609.0f;

		uint32_t vSize = 4;
		Vertex* verteces = new Vertex[vSize]{
			Vertex({ -0.5f,  0.5f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f}, 0.0f, 0.0f),
			Vertex({  0.5f,  0.5f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 1.0f, 0.0f}, 0.0f, 0.0f),
			Vertex({  0.5f, -0.5f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 1.0f, 1.0f}, 0.0f, 0.0f),
			Vertex({ -0.5f, -0.5f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f, 1.0f}, 0.0f, 0.0f)
		};




		uint32_t iSize = 6;
		uint32_t* indeces = new uint32_t[iSize]{
			0, 1, 2,
			0, 2, 3
		};

		entity.addComponent<Mesh_s>(verteces, vSize, indeces, iSize);

		entity.addComponent<Material_s>(std::string("src/assets/img/item.png"),
										std::string(""),
										std::string("src/assets/shaders/fastshader/fastshader"));
		Material_s& mat = entity.getComponent<Material_s>();
		Uniformbuffer* uniformBuffer = new Uniformbuffer("matrices", nullptr, sizeof(glm::mat4) * 1000);
		mat.shader->registerUniformBuffer(uniformBuffer);

		camEnt.addComponent<Mesh_s>(verteces, vSize, indeces, iSize);
		camEnt.addComponent<Material_s>(std::string("src/assets/img/camera.png"),
										std::string(""),
										std::string("src/assets/shaders/fastshader/fastshader"));
		Material_s& camMat = camEnt.getComponent<Material_s>();
		camMat.shader->registerUniformBuffer(uniformBuffer);
	}



#pragma endregion ENTITY_SETUP


	SceneRenderer::createInstance();
	SceneRenderer* renderer = SceneRenderer::getInstance();
	renderer->setRenderSettings();

	

	{
		
		auto lambda = [&entity, cam](const char* name, bool* open, GuiWindow* handle) {

			ImGui::Begin(name, open);

			if (entity.hasComponent<Transform_s>()) {

				glm::mat4& m = entity.getComponent<Transform_s>();
				ImGui::Text("Transform");
				for (int i = 0; i < 4; i++) {
					ImGui::Text("%f\t%f\t%f\t%f", m[i][0], m[i][1], m[i][2], m[i][3]);
				}

				ImGui::Separator();

				ImGui::Text("MVP");
				glm::mat4 n = cam->getProjectionView().getMatrix() * entity.getComponent<Transform_s>().transform;
				for (int i = 0; i < 4; i++) {
					ImGui::Text("%f\t%f\t%f\t%f", n[i][0], n[i][1], n[i][2], n[i][3]);
				}

				ImGui::Separator();
				
			}
			if (entity.hasComponent<Mesh_s>()) {

				Mesh_s& mesh = entity.getComponent<Mesh_s>();

				VBO* vbo = mesh.vbo;
				Vertex* verteces = vbo->getData();

				ImGui::Text("Vertex buffer");
				for (int i = 0; i < vbo->getSize(); i++) {
					Vertex& v = verteces[i];
					ImGui::Text("%f\t%f\t%f", v.pos.x, v.pos.y, v.pos.z);
				}

				IBO* ibo = mesh.ibo;
				uint32_t* indeces = ibo->getData();

				ImGui::Text("Index buffer");
				for (int i = 0; i < ibo->getSize(); i++) {
					ImGui::Text("%i", indeces[i]);
				}

				ImGui::Separator();
			}
			if (entity.hasComponent<Material_s>()) {

				ImGui::Text("Material");

				Material_s& mat = entity.getComponent<Material_s>();
				glm::vec2 s = mat.tex->getSize();
				ImVec2 size = { s.x, s.y };
				ImGui::Image((ImTextureID) mat.tex->getId(), { 256, 256 });

				ImGui::Separator();
			}
			ImGui::End();

		};
		Gui::getInstance()->addGuiWindow_void("Entity", lambda);
	}


	Framebuffer fb(window->getSize());
	fb.installViewPortCallback(window);

	Framebuffer fb1(window->getSize());
	fb1.installViewPortCallback(window);
	//window->toggleFullscreen();

	int frames = 0;
	int highestFps = 0;
	delta::archt_time last = delta::getTimePoint();
	delta::archt_time now;
	float deltaTime = 0.0f;
	float secondsTimer = 0.0f;
	float renderTimer = 0.0f;
	int targetFps = 500;
	float targetDelta = 1.0f / targetFps;
	{ 
		renderTimer += deltaTime;
		auto lambda = [&renderTimer, &targetFps, &targetDelta, &deltaTime, &frames, &highestFps]
											(const char* name, bool* open, GuiWindow* handle) {
			ImGui::Begin(name, open);
			std::string fileName = "";
			extractFileName(__FILE__, fileName, '\\');
			ImGui::Text("File: %s", fileName.c_str());

			ImGui::Text("Frame count: %i", frames);
			ImGui::Text("highestFps count: %i", highestFps);
			ImGui::Text("delta time: %f\seconds", deltaTime);
			ImGui::Text("render time: %f\seconds", renderTimer);

			if (ImGui::SliderInt("Target FPS: ", &targetFps, 30, 1000)) {
				targetDelta = 1.0f / targetFps;
				highestFps = 0;
			}

			ImGui::End();
		};
		Gui::getInstance()->addGuiWindow_void("Frames", lambda);
	}

#ifdef SPLASH_SCREEN
	{
		delta::archt_time timer = delta::getTimePoint();
		while (delta::getDelta<delta::seconds>(timer, false) < 5.0f) {
			// just a artificial delay
		}
	}
	delete hWin;
	hWin = nullptr;
#endif

	window->show();

	BezierreCurve curve({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f });
	curve.addAnchor({ 0.33333f, 0.5f, 0.0f });
	curve.addAnchor({ 0.66666f, -0.5f, 0.0f });
	Curve crv = curve.getCurve();


	while (true) {

		deltaTime = delta::getDelta<delta::seconds>(last);
		secondsTimer += deltaTime;
		renderTimer += deltaTime;

		if (secondsTimer > 1.0f) {
			if (frames > highestFps) {
				highestFps = frames;
			}
			frames = 0;
			secondsTimer -= 1.0f;
		}
		
		window->pollEvents();
		
		if (window->shouldClose()) {
			break;
		}
#pragma region CAMERA_CONTROLS
		
		ptr<Camera_new> controlledCamera = nullptr;
		if (fb.getGuiWindow() == Gui::getInstance()->getFocusedWindow()) {
			controlledCamera = cam;
		}
		else if (fb1.getGuiWindow() == Gui::getInstance()->getFocusedWindow()) {
			controlledCamera = cam1;
		}

		if (controlledCamera) {
			float cameraSpeed = 0.5f;
			if (Input::isPress(GLFW_KEY_LEFT_CONTROL) || Input::isHeld(GLFW_KEY_LEFT_CONTROL)) {
				cameraSpeed *= 0.5f;
			}
			else if (Input::isRelease(GLFW_KEY_LEFT_CONTROL)) {
				cameraSpeed *= 2.0f;
			}

			if (Input::isPress(GLFW_KEY_LEFT_SHIFT) || Input::isHeld(GLFW_KEY_LEFT_SHIFT)) {
				cameraSpeed *= 2.0f;
			}
			else if (Input::isRelease(GLFW_KEY_LEFT_SHIFT)) {
				cameraSpeed *= 0.5f;
			}

			cameraSpeed *= deltaTime;

			if (Input::isPress(GLFW_KEY_W) || Input::isHeld(GLFW_KEY_W)) {
				controlledCamera->translate({ 0.0f, 0.0f, cameraSpeed });
			}
			else if (Input::isPress(GLFW_KEY_S) || Input::isHeld(GLFW_KEY_S)) {
				controlledCamera->translate({ 0.0f, 0.0f, -cameraSpeed });
			}
			if (Input::isPress(GLFW_KEY_A) || Input::isHeld(GLFW_KEY_A)) {
				controlledCamera->translate({ -cameraSpeed, 0.0f, 0.0f });
			}
			else if (Input::isPress(GLFW_KEY_D) || Input::isHeld(GLFW_KEY_D)) {
				controlledCamera->translate({ cameraSpeed, 0.0f, 0.0f });
			}
		}
#pragma endregion CAMERA_CONTROLS

		{
			static delta::archt_time timer = delta::getTimePoint();
			Transform_s& transform = entity.getComponent<Transform_s>();
			glm::vec3 pos = transform.getPosition();
			glm::vec3 dst = crv.getPosition(delta::getDelta<delta::seconds>(timer, false));
			glm::vec3 translation = dst - pos;
			transform.translate(translation);
		}


		
		if (renderTimer >= targetDelta) {
			renderer->setRendertarget(&fb);
			
			renderer->clear();
			renderer->beginScene(&scene, cam);
			
			renderer->submit(entity);
			
			renderer->render();
			renderer->endScene();
			renderer->flush();


			camEnt.getComponent<Transform_s>() = cam->getView().inverse().getMatrix();

			float angle = getAngle(cam->getPosition(), cam->getPosition() + glm::vec3(0.0f, 0.0f, 0.1f), cam1->getPosition());
			camEnt.getComponent<Transform_s>().rotate(-angle, { 0.0f, 1.0f, 0.0f });


			renderer->setRendertarget(&fb1);
			
			renderer->clear();
			renderer->beginScene(&scene, cam1);
			
			renderer->submit(entity);
			renderer->submit(camEnt);
			
			renderer->render();
			renderer->endScene();
			renderer->flush();



			Gui::getInstance()->render();


			window->swapBuffer();
			frames++;
			renderTimer -= targetDelta;
			
			
		}
		
		Scheduler::getInstance().dispatchTasks();


	}



	delete window;
	Gui::terminate();

	SceneRenderer::deleteInstance();
	Input::terminate();
	
	GLRenderAPI::terminate();

	return 0;
}




