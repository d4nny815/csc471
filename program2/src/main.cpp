/*
 * Example two meshes and two shaders (could also be used for Program 2)
 * includes modifications to shape and initGeom in preparation to load
 * multi shape objects 
 * CPE 471 Cal Poly Z. Wood + S. Sueda + I. Dunn
 */

#include <iostream>
#include <glad/glad.h>

#include "GLSL.h"
#include "Program.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "WindowManager.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define X_AXIS          (vec3(1, 0, 0))
#define Y_AXIS          (vec3(0, 1, 0))
#define Z_AXIS          (vec3(0, 0, 1))

using namespace std;
using namespace glm;

class Application : public EventCallbacks {

public:

    WindowManager * windowManager = nullptr;

    // Our shader program
    std::shared_ptr<Program> prog;

    //a different mesh
    vector<shared_ptr<Shape>> charizard;
    vector<shared_ptr<Shape>> gojo;
    vector<shared_ptr<Shape>> tree;
    vector<shared_ptr<Shape>> slot_machine;
    shared_ptr<Shape> wolf;
    shared_ptr<Shape> dog;

    //example data that might be useful when trying to compute bounds on multi-shape
    vec3 gMin;

    //animation data
    float gTrans = 0;

    float char_y;
    float gojo_y;

    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        if (key == GLFW_KEY_A && action == GLFW_PRESS) {
            gTrans += 0.2;
        }
        if (key == GLFW_KEY_D && action == GLFW_PRESS) {
            gTrans -= 0.2;
        }
        if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        }
        if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
    }

    void mouseCallback(GLFWwindow *window, int button, int action, int mods)
    {
        double posX, posY;

        if (action == GLFW_PRESS)
        {
             glfwGetCursorPos(window, &posX, &posY);
             cout << "Pos X " << posX <<  " Pos Y " << posY << endl;
        }
    }

    void resizeCallback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void init(const std::string& resourceDirectory)
    {
        GLSL::checkVersion();

        // Set background color.
        glClearColor(.12f, .34f, .56f, 1.0f);
        // Enable z-buffer test.
        glEnable(GL_DEPTH_TEST);

        // Initialize the GLSL program.
        prog = make_shared<Program>();
        prog->setVerbose(true);
        prog->setShaderNames(resourceDirectory + "/simple_vert.glsl", resourceDirectory + "/simple_frag.glsl");
        prog->init();
        prog->addUniform("P");
        prog->addUniform("V");
        prog->addUniform("M");
        prog->addAttribute("vertPos");
        prog->addAttribute("vertNor");

    }

    void initGeom(const std::string& resourceDirectory)
    {
        vector<tinyobj::material_t> objMaterials;
         string errStr;
        bool rc;
        //EXAMPLE set up to read one shape from one obj file - convert to read several
        // Initialize mesh
        // Load geometry
         // Some obj files contain material information.We'll ignore them for this assignment.

        vector<tinyobj::shape_t> shapes_char;
         rc = tinyobj::LoadObj(shapes_char, objMaterials, errStr, (resourceDirectory + "/char2.obj").c_str());
        if (!rc) {
            cerr << errStr << endl;
        } else {
            for (auto shape: shapes_char) {
                auto s = make_shared<Shape>(false);
                s->createShape(shape);
                s->measure();
                s->init();
                charizard.push_back(s);
            }
        }

         vector<tinyobj::shape_t> shape_dog; 		
         rc = tinyobj::LoadObj(shape_dog, objMaterials, errStr, (resourceDirectory + "/dog.obj").c_str());
        if (!rc) {
            cerr << errStr << endl;
        } else {
            dog = make_shared<Shape>(false);
            dog->createShape(shape_dog[0]);
            dog->measure();
            dog->init();
        }

        vector<tinyobj::shape_t> shape_wolf;
         rc = tinyobj::LoadObj(shape_wolf, objMaterials, errStr, (resourceDirectory + "/Wolf_One_obj.obj").c_str());
        if (!rc) {
            cerr << errStr << endl;
        } else {
            wolf = make_shared<Shape>(false);
            wolf->createShape(shape_wolf[0]);
            wolf->measure();
            wolf->init();
        }
        

        vector<tinyobj::shape_t> shapes_gojo;
         rc = tinyobj::LoadObj(shapes_gojo, objMaterials, errStr, (resourceDirectory + "/gojo.obj").c_str());
        if (!rc) {
            cerr << errStr << endl;
        } else {
            for (auto shape: shapes_gojo) {
                auto s = make_shared<Shape>(false);
                s->createShape(shape);
                s->measure();
                s->init();
                gojo.push_back(s);
            }
        }

        vector<tinyobj::shape_t> shapes_tree;
        rc = tinyobj::LoadObj(shapes_tree, objMaterials, errStr, (resourceDirectory + "/tree.obj").c_str());
        if (!rc) {
            cerr << errStr << endl;
        } else {
            for (auto shape: shapes_tree) {
                auto s = make_shared<Shape>(false);
                s->createShape(shape);
                s->measure();
                s->init();
                tree.push_back(s);
            }
        }

        vector<tinyobj::shape_t> shapes_slot;
        rc = tinyobj::LoadObj(shapes_slot, objMaterials, errStr, (resourceDirectory + "/slot_machine.obj").c_str());
        if (!rc) {
            cerr << errStr << endl;
        } else {
            for (auto shape: shapes_slot) {
                auto s = make_shared<Shape>(false);
                s->createShape(shape);
                s->measure();
                s->init();
                slot_machine.push_back(s);
            }
        }
    }

    void draw_multi(shared_ptr<Program> prog, vector<shared_ptr<Shape>> object) {
        for (auto shape : object) {
            shape->draw(prog);
        }
    }

    /* helper for sending top of the matrix strack to GPU */
    void setModel(std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack>M) {
        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
    }

    /* helper function to set model trasnforms */
    void setModel(shared_ptr<Program> curS, vec3 trans, float rotY, float rotX, float sc) {
          mat4 Trans = glm::translate( glm::mat4(1.0f), trans);
          mat4 RotX = glm::rotate( glm::mat4(1.0f), rotX, vec3(1, 0, 0));
          mat4 RotY = glm::rotate( glm::mat4(1.0f), rotY, vec3(0, 1, 0));
          mat4 ScaleS = glm::scale(glm::mat4(1.0f), vec3(sc));
          mat4 ctm = Trans*RotX*RotY*ScaleS;
          glUniformMatrix4fv(curS->getUniform("M"), 1, GL_FALSE, value_ptr(ctm));
      }

    void render() {
        // Get current frame buffer size.
        int width, height;
        glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
        glViewport(0, 0, width, height);

        // Clear framebuffer.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Use the matrix stack for Lab 6
        float aspect = width/(float)height;

        // Create the matrix stacks - please leave these alone for now
        auto Projection = make_shared<MatrixStack>();
        auto View = make_shared<MatrixStack>();
        auto Model = make_shared<MatrixStack>();

        // Apply perspective projection.
        Projection->pushMatrix();
        Projection->perspective(45.0f, aspect, 0.01f, 100.0f);

        // View is global translation along negative z for now
        const float CAM_Z = -50;
        const float CAM_Y = -10;
        View->pushMatrix();
        View->loadIdentity();
        View->translate(vec3(0, CAM_Y, CAM_Z));
        View->rotate(gTrans, Y_AXIS);

        prog->bind();
        glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
        glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));

        // Reference origin
        Model->pushMatrix();
        Model->loadIdentity();


        // draw my king
        Model->pushMatrix(); // gojo push

        Model->rotate(M_PI_2, Y_AXIS);
        Model->translate(vec3(0, 0, -5));
        Model->scale(5);
        setModel(prog, Model);
        for (int i = 3; i < gojo.size(); i++) {
            gojo[i]->draw(prog);
        }

        // gojo orb
        Model->pushMatrix(); // push fighting orb
        gojo_y = sin(7 * glfwGetTime()) / 4 + .25;
        Model->translate(vec3(0, 0, gojo_y));
        setModel(prog, Model);
        gojo[0]->draw(prog);
        Model->popMatrix(); // pop fighting orb

        
        Model->pushMatrix(); // gojo orb push
        Model->translate(vec3(0, -.3, 0));

        float orb_speed = glfwGetTime() * 5;

        Model->pushMatrix(); // push flat ring
        Model->rotate(1.25 * orb_speed, Y_AXIS);
        Model->translate(vec3(0, 0, .8));
        setModel(prog, Model);
        gojo[2]->draw(prog);
        Model->popMatrix();  // pop flat ring

        Model->translate(vec3(0, 1, .5));
        Model->pushMatrix(); // push cool ring
        gojo_y = glfwGetTime() * 12;
        
        const float orb_dist_x = 0;
        const float orb_dist_y = 0;
        
        Model->translate(vec3(orb_dist_x, orb_dist_y, 0));
        Model->rotate(sin(glfwGetTime()), X_AXIS);
        Model->rotate(orb_speed, Y_AXIS);
        Model->rotate(glfwGetTime(), Z_AXIS);
        Model->translate(vec3(-orb_dist_x, -orb_dist_y, 0));
        setModel(prog, Model);
        gojo[2]->draw(prog);
        Model->popMatrix(); // pop cool ring


        Model->popMatrix();  // gojo orbs pop
        Model->popMatrix(); // gojo pop

        // draw charizard
        Model->pushMatrix();

        char_y = sin(glfwGetTime()) / 4;
        Model->rotate(-M_PI_2, Y_AXIS);
        Model->translate(vec3(0, char_y, -8.5));
        Model->scale(.65);
        setModel(prog, Model);
        charizard[0]->draw(prog);

        char_y = sin(10 * glfwGetTime());
        Model->pushMatrix();
        Model->rotate(char_y / 12, Z_AXIS);
        setModel(prog, Model);
        charizard[1]->draw(prog);
        Model->popMatrix();
        
        Model->pushMatrix();
        Model->rotate(-char_y / 12, Z_AXIS);
        setModel(prog, Model);
        charizard[2]->draw(prog);
        Model->popMatrix();

        Model->popMatrix();

        
        // Draw Trees
        Model->pushMatrix();
        const float TREE_DIST = 19;
        Model->translate(vec3(TREE_DIST, 0, 0));
        setModel(prog, Model);
        draw_multi(prog, tree);
        
        for (int i = 0; i < 4; i++) {
            Model->pushMatrix();
            Model->translate(vec3(-TREE_DIST, 0, 0));
            Model->rotate(M_PI_4, vec3(0, 1, 0));
            Model->translate(vec3(TREE_DIST, 0, 0));
            setModel(prog, Model);
            draw_multi(prog, tree);
        }

        for (int i = 0; i < 5; i++) {
            Model->popMatrix();
        }

        // setup for wolf, dog, and slot machine
        // slot is the ref point
        Model->pushMatrix();
        Model->translate(vec3(-10, 0, 8));
        Model->scale(.3);
        
        // draw slot machine
        Model->pushMatrix();
        Model->scale(.2);
        Model->rotate(-M_PI_2, vec3(1, 0, 0));
        setModel(prog, Model);
        draw_multi(prog, slot_machine);
        Model->popMatrix();

        // draw dog
        Model->pushMatrix();
        Model->rotate(5 * M_PI / 4, vec3(0, 1, 0));
        Model->translate(vec3(0, 0, -15));
        Model->rotate(-M_PI / 12, vec3(1, 0, 0));
        setModel(prog, Model);
        dog->draw(prog);
        Model->popMatrix();

        // draw wolf
        Model->pushMatrix();
        Model->rotate(3 * M_PI / 4, vec3(0, 1, 0));
        Model->translate(vec3(0, -3, -10));
        Model->rotate(-M_PI / 12, vec3(1, 0, 0));
        Model->scale(10);
        setModel(prog, Model);
        wolf->draw(prog);
        Model->popMatrix();

        // pop slot ref
        Model->popMatrix();


        // main pop
        Model->popMatrix();
        prog->unbind();

        //animation update example

        // Pop matrix stacks.
        Projection->popMatrix();
        View->popMatrix();
    }
};

int main(int argc, char *argv[])
{
    // Where the resources are loaded from
    std::string resourceDir = "../resources";

    if (argc >= 2)
    {
        resourceDir = argv[1];
    }

    Application *application = new Application();

    // Your main will always include a similar set up to establish your window
    // and GL context, etc.

    WindowManager *windowManager = new WindowManager();
    windowManager->init(640, 480);
    windowManager->setEventCallbacks(application);
    application->windowManager = windowManager;

    // This is the code that will likely change program to program as you
    // may need to initialize or set up different data and state

    application->init(resourceDir);
    application->initGeom(resourceDir);

    // Loop until the user closes the window.
    while (! glfwWindowShouldClose(windowManager->getHandle()))
    {
        // Render scene.
        application->render();

        // Swap front and back buffers.
        glfwSwapBuffers(windowManager->getHandle());
        // Poll for and process events.
        glfwPollEvents();
    }

    // Quit program.
    windowManager->shutdown();
    return 0;
}
