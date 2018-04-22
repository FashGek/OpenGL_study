#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Common.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#ifdef __APPLE__
    #include "libs/glm/glm.hpp"
    #include "libs/glm/gtc/matrix_transform.hpp"
#else
    #include "glm/glm.hpp"
    #include "glm/gtc/matrix_transform.hpp"
#endif

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

int main()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow * window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {   
        cout << "glew init error" << endl;
    }

    cout << glGetString(GL_VERSION) << endl;

    {
        // set view port
        GLCall(glViewport(0, 0, 640, 480));

        // line mode or fill mode
#if 0
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   
#endif

        float positions[] = {
            -100.0f, -100.0f,  0.0f,  0.0f,  0.0f,
             100.0f, -100.0f,  0.0f,  1.0f,  0.0f,
             100.0f,  100.0f,  0.0f,  1.0f,  1.0f,
            -100.0f,  100.0f,  0.0f,  0.0f,  1.0f,
        };

        unsigned int index[] = {
            0, 1, 2,
            2, 3, 0,
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray vertex_array;
        VertexBuffer vertex_buffer(positions, 5 * 4 * sizeof(float));
        VertexBufferLayout vertex_buffer_layout;
        vertex_buffer_layout.push<float>(3);
        vertex_buffer_layout.push<float>(2);
        IndexBuffer index_buffer(index, 6);
        
        vertex_array.add_buffer(vertex_buffer,
                                vertex_buffer_layout,
                                index_buffer);

        // projection matrix
        const auto proj = glm::ortho(-320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f);
        // view transform matrix
        const auto view = glm::translate(glm::mat4(1.0f), -glm::vec3(-100.0f, 100.0f, 0.0f));
        // model transform matrix
        const auto model = glm::rotate(glm::mat4(1.0f), 1.57f, glm::vec3(0.0f, 0.0f, 1.0f));

        const auto mvp = proj * view * model;

        Shader shader("res/shaders/texture.shader");
        shader.bind();

        shader.set_uniform_mat4f("u_MVP", mvp);
        //shader.set_uniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
        
        Texture texture("res/textures/hello.png");
        texture.bind();

        // set texture
        shader.set_uniform1i("m_Texture", 0);

        vertex_array.unbind();
        shader.unbind();

        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            process_input(window);

            renderer.clear();

            renderer.draw(vertex_array, shader);

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }

    glfwTerminate();

    return 0;
}

/**
 * frame buffer resize callback 
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    GLCall(glViewport(0, 0, width, height));
}

/**
 * process input
 */
void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}