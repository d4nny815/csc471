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

using namespace std;
using namespace glm;

class Application : public EventCallbacks {

public:

    WindowManager * windowManager = nullptr;

    // Our shader program
    std::shared_ptr<Program> prog;

    // Our shader program
    // std::shared_ptr<Program> solidColorProg;
    
    //a different mesh
    vector<shared_ptr<Shape>> charizard;
    vector<shared_ptr<Shape>> gojo;
    shared_ptr<Shape> wolf;
    shared_ptr<Shape> dog;
    shared_ptr<Shape> tree;

    //example data that might be useful when trying to compute bounds on multi-shape
    vec3 gMin;

    //animation data
    float sTheta = 0;
    float gTrans = 0;

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
            //for now all our shapes will not have textures - change in later labs
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
            //for now all our shapes will not have textures - change in later labs
            wolf = make_shared<Shape>(false);
            wolf->createShape(shape_wolf[0]);
            wolf->measure();
            wolf->init();
        }
        

        // TODO: also multishape
        vector<tinyobj::shape_t> shapes_gojo;
         rc = tinyobj::LoadObj(shapes_gojo, objMaterials, errStr, (resourceDirectory + "/gojo.obj").c_str());
        if (!rc) {
            cerr << errStr << endl;
        } else {
            //for now all our shapes will not have textures - change in later labs
            // FIXME: maybe push first?
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
            //for now all our shapes will not have textures - change in later labs
            tree = make_shared<Shape>(false);
            tree->createShape(shapes_tree[0]);
            tree->measure();
            tree->init();
        }

        // ? what to do with these?
        //read out information stored in the shape about its size - something like this...
        //then do something with that information.....
        // gMin.x = mesh->min.x;
        // gMin.y = mesh->min.y;
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

    // TODO: add rotateZ
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
        View->pushMatrix();
        View->loadIdentity();
        View->translate(vec3(gTrans, -8, -30));

        prog->bind();
        glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
        glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));

        Model->pushMatrix();
        Model->loadIdentity();

        // draw my king
        Model->pushMatrix();
        Model->scale(10);
        setModel(prog, Model);
        draw_multi(prog, gojo);
        
        Model->scale(.05);
        setModel(prog, Model);

        draw_multi(prog, charizard);
        
        Model->popMatrix();

        
        // Draw Trees
        Model->pushMatrix();
        Model->translate(vec3(13, 0, 0));
        setModel(prog, Model);
        tree->draw(prog);
        
        for (int i = 0; i < 4; i++) {
            Model->pushMatrix();
            Model->translate(vec3(-13, 0, 0));
            Model->rotate(M_PI_4, vec3(0, 1, 0));
            Model->translate(vec3(13, 0, 0));
            setModel(prog, Model);
            tree->draw(prog);
        }

        // pop trees
        for (int i = 0; i < 5; i++) {
            Model->popMatrix();
        }

        // setup for wolf, dog, and slot machine
        // slot (dog rn) is the ref point
        Model->pushMatrix();
        Model->translate(vec3(-15, -2, 3));
        Model->scale(.3);
        setModel(prog, Model);
        dog->draw(prog);
        
        // finish dog
        Model->pushMatrix();
        Model->rotate(5 * M_PI / 4, vec3(0, 1, 0));
        Model->translate(vec3(0, 0, -15));
        Model->rotate(-M_PI / 12, vec3(1, 0, 0));
        setModel(prog, Model);
        dog->draw(prog);
        Model->popMatrix();

        // finish wolf
        Model->pushMatrix();
        Model->rotate(3 * M_PI / 4, vec3(0, 1, 0));
        Model->translate(vec3(0, -3, -10));
        Model->rotate(-M_PI / 12, vec3(1, 0, 0));
        Model->scale(10);
        setModel(prog, Model);
        wolf->draw(prog);
        Model->popMatrix();

        
        // pop dog and wolf
        Model->popMatrix();


        // draw gojo and charizard


        // main pop
        Model->popMatrix();


        //use helper function that uses glm to create some transform matrices
        // setModel(prog, vec3(-1, .5, 0), 0, -3.14/8, .2);
        // dog->draw(prog);

        // Model->pushMatrix();
        // Model->loadIdentity();
        // setModel(prog, Model);
        // wolf->draw(prog);

        // Model->pushMatrix();
        // Model->translate(vec3(.5, 0, 3));
        // Model->scale(vec3(0.5, 0.5, 0.5));
        // Model->rotate(sTheta, vec3(0, 1, 0));
        // setModel(prog, Model);
        // wolf->draw(prog);
        // Model->popMatrix();

        prog->unbind();

        // // Draw base Hierarchical person
        // prog->bind();
        // glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
        // glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));

        // //use helper function that uses glm to create some transform matrices
        // // setModel(prog, vec3(1.7, -1.7, 0), 0, 0, 0.5);
        // // bunny->draw(prog);


        // // draw hierarchical mesh using matrix stack
        // Model->pushMatrix();
        // Model->loadIdentity();
        // Model->translate(vec3(gTrans, 0, 0));
        
        // /* draw top cube - aka head */
        // Model->pushMatrix();
        // Model->translate(vec3(0, 1.4, 0));
        // Model->scale(vec3(0.5, 0.5, 0.5));
        // setModel(prog, Model);
        // mesh->draw(prog);
        // Model->popMatrix();
        
        // //draw the torso with these transforms
        // Model->pushMatrix();
        // Model->scale(vec3(1.25, 1.35, 1.25));
        // setModel(prog, Model);
        // mesh->draw(prog);
        // Model->popMatrix();
        
        // // draw the upper 'arm' - relative 
        // //note you must change this to include 3 components!
        // Model->pushMatrix();
        // //place at shoulder
        // Model->translate(vec3(0.8, 0.8, 0));
        // //rotate shoulder joint
        // Model->rotate(sTheta, vec3(0, 0, 1));
        // //move to shoulder joint
        // Model->translate(vec3(0.8, 0, 0));
    
        // //now draw lower arm - this is INCOMPLETE and you will add a 3rd component
        // //right now this is in the SAME place as the upper arm
        // Model->pushMatrix();
        // Model->scale(vec3(0.8, 0.25, 0.25));
        // setModel(prog, Model);
        // mesh->draw(prog);
        // Model->popMatrix();

        // //Do final scale ONLY to upper arm then draw
        // //non-uniform scale
        // Model->scale(vec3(0.8, 0.25, 0.25));
        // setModel(prog, Model);
        // mesh->draw(prog);
        // Model->popMatrix();
        
        // Model->popMatrix();

        // prog->unbind();

        //animation update example
        sTheta = sin(glfwGetTime());

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
