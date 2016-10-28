#include "assignment3.hpp"
#include "interpolation.hpp"
#include "node.hpp"
#include "parametric_shapes.hpp"

#include "config.hpp"
#include "external/glad/glad.h"
#include "core/Bonobo.h"
#include "core/FPSCamera.h"
#include "core/InputHandler.h"
#include "core/Log.h"
#include "core/LogView.h"
#include "core/Misc.h"
#include "core/utils.h"
#include "core/Window.h"
#include <imgui.h>
#include "external/imgui_impl_glfw_gl3.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "external/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdlib>
#include <stdexcept>

enum class polygon_mode_t : unsigned int {
	fill = 0u,
	line,
	point
};

static polygon_mode_t get_next_mode(polygon_mode_t mode)
{
	return static_cast<polygon_mode_t>((static_cast<unsigned int>(mode) + 1u) % 3u);
}

eda221::Assignment3::Assignment3()
{
	Log::View::Init();

	window = Window::Create("EDA221: Assignment 3", config::resolution_x,
		config::resolution_y, config::msaa_rate, false);
	if (window == nullptr) {
		Log::View::Destroy();
		throw std::runtime_error("Failed to get a window: aborting!");
	}
	inputHandler = new InputHandler();
	window->SetInputHandler(inputHandler);
}

eda221::Assignment3::~Assignment3()
{
	delete inputHandler;
	inputHandler = nullptr;

	Window::Destroy(window);
	window = nullptr;

	Log::View::Destroy();
}
    


void
eda221::Assignment3::run()
{
    float camX = -5.0;
    float camZ = 0.0;
    float x = 0.0;
    float z = 0.0;
    float theta = 0.0;
    float A1 = 4.0;
    float f1 = 0.1;
    float p1 = 0.5;
    float k1 = 3.0;
    glm::vec2 d1 = glm::vec2(-1.0,0.0);
    
    float A2 = 2.5;
    float f2 = 0.1;
    float p2 = 0.3;
    float k2 = 2.0;
    glm::vec2 d2 = glm::vec2(-0.7,0.7);
    
    
    //Assimp
    
    
    
    
    
    auto world = Node();
	// Load the sphere geometry
    auto const spaceship_shape = eda221::loadObjects("spaceship.obj");
    auto const longship_shape = eda221::loadObjects("boat.obj");
	auto circle_ring_shape = parametric_shapes::createQuad(1000u,1000u);
	auto phong_shape = parametric_shapes::createSphere(200u, 200u, 2.0f);
	auto skyboxNodeShape = parametric_shapes::createSphere(200u, 200u, 500.0f);
    auto gateShape = parametric_shapes::createCircleRing(200, 200, 5, 8);
    auto gateSphereShape = parametric_shapes::createSphere(200u, 200u, 2.0f);
	if (circle_ring_shape.vao == 0u) {
		LogError("Failed to retrieve the circle ring mesh");
		return;
	}

	// Set up the camera
	FPSCameraf mCamera(bonobo::pi / 4.0f,
		static_cast<float>(config::resolution_x) / static_cast<float>(config::resolution_y),
		0.01f, 1000.0f);
	mCamera.mWorld.SetTranslate(glm::vec3(0.0f, 0.0f, 0.0f));
    mCamera.mWorld.SetTranslate(glm::vec3(-8*sinf(theta)+x, 3, -8*cosf(theta)+z));
    mCamera.mWorld.LookAt(glm::vec3(0,1,0));
	mCamera.mMouseSensitivity = 0.003f;
	mCamera.mMovementSpeed = 0.025;
	window->SetCamera(&mCamera);

	// Create the shader programs
	auto fallback_shader = eda221::createProgram("fallback.vert", "fallback.frag");
	if (fallback_shader == 0u) {
		LogError("Failed to load fallback shader");
		return;
	}
	GLuint diffuse_shader = 0u, normal_shader = 0u, texcoord_shader = 0u, skybox_shader = 0u, phong_shader = 0u;
	auto const reload_shaders = [&diffuse_shader, &normal_shader, &texcoord_shader, &skybox_shader, &phong_shader]() {
		if (diffuse_shader != 0u)
			glDeleteProgram(diffuse_shader);
		diffuse_shader = eda221::createProgram("diffuse.vert", "diffuse.frag");
		if (diffuse_shader == 0u)
			LogError("Failed to load diffuse shader");

		if (normal_shader != 0u)
			glDeleteProgram(normal_shader);
		normal_shader = eda221::createProgram("gateNext.vert", "gateNext.frag");
		if (normal_shader == 0u)
			LogError("Failed to load normal shader");

		if (texcoord_shader != 0u)
			glDeleteProgram(texcoord_shader);
		texcoord_shader = eda221::createProgram("texcoord.vert", "texcoord.frag");
		if (texcoord_shader == 0u)
			LogError("Failed to load texcoord shader");

		if (skybox_shader != 0u)
			glDeleteProgram(skybox_shader);
		skybox_shader = eda221::createProgram("skybox.vert", "skybox.frag");
		if (skybox_shader == 0u)
			LogError("Failed to load skybox shader");

		if (phong_shader != 0u)
			glDeleteProgram(phong_shader);
		phong_shader = eda221::createProgram("water.vert", "water.frag");
		if (phong_shader == 0u)
			LogError("Failed to load phong shader");
	};
	reload_shaders();

	auto light_position = glm::vec3(-2.0f, 4.0f, 2.0f);
	auto const set_uniforms = [&light_position](GLuint program) {
		glUniform3fv(glGetUniformLocation(program, "light_position"), 1, glm::value_ptr(light_position));
	};

	auto camera_position = mCamera.mWorld.GetTranslation();
	auto ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	auto diffuse = glm::vec3(0.7f, 0.0f, 0.0f);
	auto specular = glm::vec3(1.0f, 1.0f, 1.0f);
	auto shininess = 100.0f;
    auto time = 0.0f;
	auto const phong_set_uniforms = [&light_position, &camera_position, &ambient, &diffuse, &specular, &shininess, &time](GLuint program) {
		glUniform3fv(glGetUniformLocation(program, "light_position"), 1, glm::value_ptr(light_position));
		glUniform3fv(glGetUniformLocation(program, "camera_position"), 1, glm::value_ptr(camera_position));
		glUniform3fv(glGetUniformLocation(program, "ambient"), 1, glm::value_ptr(ambient));
		glUniform3fv(glGetUniformLocation(program, "diffuse"), 1, glm::value_ptr(diffuse));
		glUniform3fv(glGetUniformLocation(program, "specular"), 1, glm::value_ptr(specular));
        glUniform1f(glGetUniformLocation(program, "shininess"), shininess);
        glUniform1f(glGetUniformLocation(program, "time"), time);
 
	};

	auto polygon_mode = polygon_mode_t::fill;
	auto bumpTex = loadTexture2D("waves.png");
    auto stoneD = loadTexture2D("stone47_diffuse.png");
	auto skybox = loadTextureCubeMap("cloudyhills/posx.png", "cloudyhills/negx.png", "cloudyhills/posy.png", "cloudyhills/negy.png", "cloudyhills/negz.png", "cloudyhills/posz.png", true);

    
    
    //Earth
	auto circle_ring = Node();
    circle_ring.add_texture("Sampler", skybox, GL_TEXTURE_CUBE_MAP);
    circle_ring.add_texture("bumpSampler", bumpTex);
    circle_ring.set_geometry(circle_ring_shape);
    circle_ring.set_program(phong_shader, phong_set_uniforms);

    
    auto earth_pivot = Node();
    world.add_child(&earth_pivot);
 
    
    
    //Skybox
	auto skyboxNode = Node();
    world.add_child(&skyboxNode);
    skyboxNode.add_texture("Sampler", skybox, GL_TEXTURE_CUBE_MAP);
    skyboxNode.set_geometry(skyboxNodeShape);
    skyboxNode.set_program(skybox_shader, set_uniforms);
    
    //Moon
	auto phong = Node();

    phong.set_geometry(phong_shape);


    phong.add_texture("diffuse_texture", stoneD);

	phong.set_program(texcoord_shader, phong_set_uniforms);
    
    int currentGate = 0;
    
    //Gates
    auto gatePositions = std::vector<glm::vec4>(10);
    for (int i = 0; i < 10; i++) {
        auto theta = i*(bonobo::pi/4);
        gatePositions[i] = glm::vec4(cos(theta)*i*30,0,sin(theta)*i*30,theta);
    }


    
    auto gateThrough = std::vector<bool>(10);
    
    
    auto gates = std::vector<Node>(10);
    auto gateSpheres = std::vector<Node>(10);

    for (int i = 0; i < 10; i++) {
        auto currGate = Node();
        auto currSphere = Node();
        world.add_child(&currGate);
        world.add_child(&currSphere);
        
        currGate.set_geometry(gateShape);
        currGate.set_program(fallback_shader, set_uniforms);
        currGate.translate(glm::vec3(gatePositions[i].x, gatePositions[i].y, gatePositions[i].z));
        currGate.rotate_y(gatePositions[i].w);
        currSphere.set_geometry(gateSphereShape);
        currSphere.set_program(fallback_shader, phong_set_uniforms);
        currSphere.translate(glm::vec3(gatePositions[i].x, gatePositions[i].y, gatePositions[i].z));
 
        gateThrough[i] = false;
        gates[i] = currGate;
        gateSpheres[i] = currSphere;
    }
    gates[0].set_program(normal_shader, set_uniforms);
    
    
    float timer = 0;
    bool timeRunning = false;


    circle_ring.translate(glm::vec3(-500,0,-500));
    
 
    
    
    

	
	
	


	glEnable(GL_DEPTH_TEST);

	// Enable face culling to improve performance:
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glCullFace(GL_BACK);
    
    
    


	f64 ddeltatime;
	size_t fpsSamples = 0;
	double nowTime, lastTime = GetTimeMilliseconds();
	double fpsNextTick = lastTime + 1000.0;

	while (!glfwWindowShouldClose(window->GetGLFW_Window())) {
		nowTime = GetTimeMilliseconds();
		ddeltatime = nowTime - lastTime;
		if (nowTime > fpsNextTick) {
			fpsNextTick += 1000.0;
			fpsSamples = 0;
		}
		fpsSamples++;
        
        time += (ddeltatime / 1000);
        //printf("%f\n", time);
        
        //GLint loc = glGetUniformLocation(phong_shader, "time");
        //glUniform1f(loc, time);

        
        
    

		glfwPollEvents();
		inputHandler->Advance();
		mCamera.Update(ddeltatime, *inputHandler);

		ImGui_ImplGlfwGL3_NewFrame();

		if (inputHandler->GetKeycodeState(GLFW_KEY_1) & JUST_PRESSED) {
			circle_ring.set_program(fallback_shader, set_uniforms);
		}
		if (inputHandler->GetKeycodeState(GLFW_KEY_2) & JUST_PRESSED) {
			circle_ring.set_program(diffuse_shader, set_uniforms);
		}
		if (inputHandler->GetKeycodeState(GLFW_KEY_3) & JUST_PRESSED) {
			circle_ring.set_program(normal_shader, set_uniforms);
		}
		if (inputHandler->GetKeycodeState(GLFW_KEY_4) & JUST_PRESSED) {
			circle_ring.set_program(texcoord_shader, phong_set_uniforms);
		}
		if (inputHandler->GetKeycodeState(GLFW_KEY_5) & JUST_PRESSED) {
			circle_ring.set_program(phong_shader, phong_set_uniforms);
		}
		if (inputHandler->GetKeycodeState(GLFW_KEY_Z) & JUST_PRESSED) {
			polygon_mode = get_next_mode(polygon_mode);
		}
		if (inputHandler->GetKeycodeState(GLFW_KEY_R) & JUST_PRESSED) {
			reload_shaders();
		}

		switch (polygon_mode) {
		case polygon_mode_t::fill:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case polygon_mode_t::line:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case polygon_mode_t::point:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		}

		camera_position = mCamera.mWorld.GetTranslation();

		auto const window_size = window->GetDimensions();
		glViewport(0, 0, window_size.x, window_size.y);
		glClearDepthf(1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        
        


        
        if (::glfwGetKey(window->GetGLFW_Window(), GLFW_KEY_J) == GLFW_PRESS) {
            theta += 0.05;
            phong.rotate_y(0.05);
            mCamera.mWorld.RotateY(0.05);
        }
        if (::glfwGetKey(window->GetGLFW_Window(), GLFW_KEY_L) == GLFW_PRESS) {
            theta += -0.05;
            phong.rotate_y(-0.05);
            mCamera.mWorld.RotateY(-0.05);
        }
        if (::glfwGetKey(window->GetGLFW_Window(), GLFW_KEY_I) == GLFW_PRESS) {
            
            x+= (0.5)*sinf(theta);
            z+= (0.5)*cosf(theta);
        }
        if (::glfwGetKey(window->GetGLFW_Window(), GLFW_KEY_K) == GLFW_PRESS) {
            x+= -(0.5)*sinf(theta);
            z+= -(0.5)*cosf(theta);
        }
        
        float newY1 = A1*pow((sin((d1.x*x + d1.y*z)*f1 + time*p1)*0.5 + 0.5),k1);
        float newY2 = A2*pow((sin((d2.x*x + d2.y*z)*f2 + time*p2)*0.5 + 0.5),k2);
        phong.set_translation(glm::vec3(x,newY1+newY2,z));
        mCamera.mWorld.SetTranslate(glm::vec3(-12*sinf(theta)+x, 8, -12*cosf(theta)+z));
        
        
        
        
        

        //Render Objects
		circle_ring.render(mCamera.GetWorldToClipMatrix(), circle_ring.get_transform());
		skyboxNode.render(mCamera.GetWorldToClipMatrix(), skyboxNode.get_transform());
		phong.render(mCamera.GetWorldToClipMatrix(), phong.get_transform());
        for (int i = 0; i < 10; i++) {
            float gateY1 = A1*pow((sin((d1.x*gatePositions[i].x+ d1.y*gatePositions[i].z)*f1 + time*p1)*0.5 + 0.5),k1);
            float gateY2 = A2*pow((sin((d2.x*gatePositions[i].x + d2.y*gatePositions[i].z)*f2 + time*p2)*0.5 + 0.5),k2);
            gates[i].set_translation(glm::vec3(gatePositions[i].x,gateY1+gateY2,gatePositions[i].z));
            gateSpheres[i].set_translation(glm::vec3(gatePositions[i].x,gateY1+gateY2,gatePositions[i].z));
            
            
            //Hit Detection

            auto distanceX = pow((gatePositions[i].x-x),2);
            auto distanceY = pow(((newY1+newY2)-(gateY1+gateY2)),2);
            auto distanceZ = pow((gatePositions[i].z-z),2);
            auto dist = sqrt(distanceX+distanceY+distanceZ);
            
            if (dist < (7) && (i == currentGate)) {
                gateThrough[i] = true;
                if ( currentGate == 1) {
                    timeRunning = true;
                }
        
                currentGate++;
                
                if (currentGate < 10) {
                    gates[currentGate].set_program(normal_shader, set_uniforms);
                } else {
                    timeRunning = false;
                }
            }
            
            if (!gateThrough[i]) {
                gates[i].render(mCamera.GetWorldToClipMatrix(), gates[i].get_transform());
            }

        }
        
        
        if (timeRunning) {
            timer += (ddeltatime / 1000);
        }

        
        
        

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		Log::View::Render();

		bool opened = ImGui::Begin("Scene Control", &opened, ImVec2(300, 100), -1.0f, 0);
		if (opened) {
			ImGui::ColorEdit3("Ambient", glm::value_ptr(ambient));
			ImGui::ColorEdit3("Diffuse", glm::value_ptr(diffuse));
			ImGui::ColorEdit3("Specular", glm::value_ptr(specular));
			ImGui::SliderFloat("Shininess", &shininess, 0.0f, 1000.0f);
			ImGui::SliderFloat3("Light Position", glm::value_ptr(light_position), -20.0f, 20.0f);
			//			ImGui::SliderInt("Faces Nb", &faces_nb, 1u, 16u);
		}
		ImGui::End();

		ImGui::Begin("Timer", &opened, ImVec2(120, 50), -1.0f, 0);
			ImGui::Text("Time: %f", timer);
		ImGui::End();

		ImGui::Render();

		window->Swap();
		lastTime = nowTime;
	}

	glDeleteProgram(phong_shader);
	phong_shader = 0u;
	glDeleteProgram(skybox_shader);
	skybox_shader = 0u;
	glDeleteProgram(texcoord_shader);
	texcoord_shader = 0u;
	glDeleteProgram(normal_shader);
	normal_shader = 0u;
	glDeleteProgram(diffuse_shader);
	diffuse_shader = 0u;
	glDeleteProgram(fallback_shader);
	diffuse_shader = 0u;
}
    


int main()
{
	Bonobo::Init();
	try {
		eda221::Assignment3 assignment3;
		assignment3.run();
	}
	catch (std::runtime_error const& e) {
		LogError(e.what());
	}
	Bonobo::Destroy();
}
