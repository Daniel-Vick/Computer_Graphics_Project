#include "assignment4.hpp"
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
    
    eda221::Assignment4::Assignment4()
    {
        Log::View::Init();
        
        window = Window::Create("EDA221: Assignment 4", config::resolution_x,
                                config::resolution_y, config::msaa_rate, false);
        if (window == nullptr) {
            Log::View::Destroy();
            throw std::runtime_error("Failed to get a window: aborting!");
        }
        inputHandler = new InputHandler();
        window->SetInputHandler(inputHandler);
    }
    
    eda221::Assignment4::~Assignment4()
    {
        delete inputHandler;
        inputHandler = nullptr;
        
        Window::Destroy(window);
        window = nullptr;
        
        Log::View::Destroy();
    }
    
    void
    eda221::Assignment4::run()
    {
        auto world = Node();
        // Load the sphere geometry
        auto circle_ring_shape = parametric_shapes::createQuad(50u, 50u);
        //auto phong_shape = parametric_shapes::createSphere(200u, 200u, 1.0f);
        //auto skyboxNodeShape = parametric_shapes::createSphere(200u, 200u, 200.0f);
        if (circle_ring_shape.vao == 0u) {
            LogError("Failed to retrieve the circle ring mesh");
            return;
        }
        
        // Set up the camera
        FPSCameraf mCamera(bonobo::pi / 4.0f,
                           static_cast<float>(config::resolution_x) / static_cast<float>(config::resolution_y),
                           0.01f, 1000.0f);
        mCamera.mWorld.SetTranslate(glm::vec3(0.0f, 0.0f, 6.0f));
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
            normal_shader = eda221::createProgram("normal.vert", "normal.frag");
            if (normal_shader == 0u)
                LogError("Failed to load normal shader");
            
            if (texcoord_shader != 0u)
                glDeleteProgram(texcoord_shader);
            texcoord_shader = eda221::createProgram("bumpMap.vert", "bumpMap.frag");
            if (texcoord_shader == 0u)
                LogError("Failed to load texcoord shader");
            
            if (skybox_shader != 0u)
                glDeleteProgram(skybox_shader);
            skybox_shader = eda221::createProgram("skybox.vert", "skybox.frag");
            if (skybox_shader == 0u)
                LogError("Failed to load skybox shader");
            
            if (phong_shader != 0u)
                glDeleteProgram(phong_shader);
            phong_shader = eda221::createProgram("phong.vert", "phong.frag");
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
        auto const phong_set_uniforms = [&light_position, &camera_position, &ambient, &diffuse, &specular, &shininess](GLuint program) {
            glUniform3fv(glGetUniformLocation(program, "light_position"), 1, glm::value_ptr(light_position));
            glUniform3fv(glGetUniformLocation(program, "camera_position"), 1, glm::value_ptr(camera_position));
            glUniform3fv(glGetUniformLocation(program, "ambient"), 1, glm::value_ptr(ambient));
            glUniform3fv(glGetUniformLocation(program, "diffuse"), 1, glm::value_ptr(diffuse));
            glUniform3fv(glGetUniformLocation(program, "specular"), 1, glm::value_ptr(specular));
            glUniform1f(glGetUniformLocation(program, "shininess"), shininess);
        };
        
        auto polygon_mode = polygon_mode_t::fill;
        //auto sphereTex = loadTexture2D("earth_bump.png");
        //auto earth = loadTexture2D("earth_diffuse.png");
        //auto stoneB = loadTexture2D("stone47_bump.png");
        //auto stoneD = loadTexture2D("stone47_diffuse.png");
        //auto skybox = loadTextureCubeMap("space/posx.png", "space/negx.png", "space/posy.png", "space/negy.png", "space/negz.png", "space/posz.png", true);
        
        
        
        //Earth
        auto circle_ring = Node();
        //circle_ring.add_texture("Sampler", sphereTex);
        //circle_ring.add_texture("diffuse_texture", earth);
        circle_ring.set_geometry(circle_ring_shape);
        circle_ring.set_program(fallback_shader, set_uniforms);
        //world.add_child(&circle_ring);
        //Earth Pivot
        
        //auto earth_pivot = Node();
        //world.add_child(&earth_pivot);
        //earth_pivot.set_geometry(phong_shape);
        
        
        //Skybox
        //auto skyboxNode = Node();
        //world.add_child(&skyboxNode);
        //skyboxNode.add_texture("Sampler", skybox, GL_TEXTURE_CUBE_MAP);
        //skyboxNode.set_geometry(skyboxNodeShape);
        //skyboxNode.set_program(skybox_shader, set_uniforms);
        
        //Moon
        //auto phong = Node();
        
        //phong.set_translation(glm::vec3(8u, 2u, 0u));
        //phong.add_texture("Sampler", stoneB);
        //phong.add_texture("diffuse_texture", stoneD);
        //phong.set_geometry(phong_shape);
        //phong.set_program(texcoord_shader, phong_set_uniforms);
        
        //arth_pivot.add_child(&phong);
        
        
        
        
        
        
        
        
        
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
            
            //phong.rotate_y(.002);
            //phong.set_translation(glm::vec3(5*std::sin(nowTime), 5*std::cos(nowTime), 0.0f));
            //skyboxNode.translate(glm::vec3(1.0,0.0,0.0));
            //circle_ring.rotate_y(.003);
            //phong.translate(glm::vec3(1.0,0.0,0.0));
            
            if (::glfwGetKey(window->GetGLFW_Window(), GLFW_KEY_L) == GLFW_PRESS) {
                circle_ring.rotate_y(.01);
            }
            if (::glfwGetKey(window->GetGLFW_Window(), GLFW_KEY_H) == GLFW_PRESS) {
                circle_ring.rotate_y(-.01);
            }
            if (::glfwGetKey(window->GetGLFW_Window(), GLFW_KEY_Y) == GLFW_PRESS) {
                circle_ring.rotate_z(.01);
            }
            if (::glfwGetKey(window->GetGLFW_Window(), GLFW_KEY_O) == GLFW_PRESS) {
                circle_ring.rotate_z(-.01);
            }
            if (::glfwGetKey(window->GetGLFW_Window(), GLFW_KEY_J) == GLFW_PRESS) {
                circle_ring.translate(glm::vec3(-0.5,0.0,0.0));
            }
            if (::glfwGetKey(window->GetGLFW_Window(), GLFW_KEY_K) == GLFW_PRESS) {
                circle_ring.translate(glm::vec3(0.5,0.0,0.0));
            }
            
            circle_ring.render(mCamera.GetWorldToClipMatrix(), circle_ring.get_transform());
            //skyboxNode.render(mCamera.GetWorldToClipMatrix(), skyboxNode.get_transform());
            //phong.render(mCamera.GetWorldToClipMatrix(), phong.get_transform());
            
            
            
            
            
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
            
            ImGui::Begin("Render Time", &opened, ImVec2(120, 50), -1.0f, 0);
            if (opened)
                ImGui::Text("%.3f ms", ddeltatime);
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
        eda221::Assignment4 assignment4;
        assignment4.run();
    }
    catch (std::runtime_error const& e) {
        LogError(e.what());
    }
    Bonobo::Destroy();
}
