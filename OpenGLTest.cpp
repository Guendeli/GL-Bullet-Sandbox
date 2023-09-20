#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderLoader.h"
#include "Camera.h"
#include "LightRenderer.h"
#include "TextureLoader.h"
#include "MeshRenderer.h"
#include <btBulletDynamicsCommon.h>
#include <chrono>

GLuint textureShaderProgram,sphereTexture,groundTexture;
btDiscreteDynamicsWorld* dynamicsWorld;
Camera* camera;
LightRenderer* light;
MeshRenderer* sphereRenderer;
MeshRenderer* ground;

void renderScene();
void initGame();
void addRigidBodies();

int main(int argc, char* argv[])
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800,600, "GLFW", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();

    initGame();
    auto previousTime = std::chrono::high_resolution_clock::now();
    
    while(glfwWindowShouldClose(window) == false)
    {
        // update
        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - previousTime).count();
        dynamicsWorld->stepSimulation(dt);
        //render
        renderScene();
        // Swap buffer
        glfwSwapBuffers(window);
        glfwPollEvents();
        previousTime = currentTime;
    }
    glfwTerminate();

    // delete camera;
    // delete light;
    
    return 0;
}

void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0,0.0,1.0,1.0);
    sphereRenderer->draw();
    ground->draw();
}

void initGame()
{
    glEnable(GL_DEPTH_TEST);
    camera = new Camera(45.0f, 800, 600, 0.1f, 100.0f, glm::vec3(0.0f,4.0f, 20.0f));

    // Loading
    ShaderLoader shader;
    TextureLoader tLoader;
    textureShaderProgram = shader.CreateProgram("Assets/Shaders/TexturedModel.vs", "Assets/Shaders/TexturedModel.fs");
    sphereTexture = tLoader.getTexture("Assets/Textures/tennisball.jpg");
    groundTexture = tLoader.getTexture("Assets/Textures/ground.jpg");

    // Init Physics
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration* collisionConfig = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* collisionDispatcher = new btCollisionDispatcher(collisionConfig);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

    dynamicsWorld = new btDiscreteDynamicsWorld(collisionDispatcher, broadphase, solver,collisionConfig);
    dynamicsWorld->setGravity(btVector3(0,-9.8f,0));

    addRigidBodies();
}

void addRigidBodies()
{
    // Adding a rigid body
    btCollisionShape* sphereShape = new btSphereShape(1.0f);
    btDefaultMotionState* sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,10.0f,0)));
    btScalar mass = 13.0;
    btVector3 sphereInertia(0,0,0);
    sphereShape->calculateLocalInertia(mass,sphereInertia);

    btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(mass, sphereMotionState, sphereShape, sphereInertia);
    btRigidBody* sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
    sphereRigidBody->setRestitution(0.0f);
    sphereRigidBody->setFriction(1.0f);
    sphereRigidBody->setActivationState(DISABLE_DEACTIVATION);
    
    dynamicsWorld->addRigidBody(sphereRigidBody);

    // Sphere visual
    sphereRenderer = new MeshRenderer(MeshType::kSphere,"ball", camera, sphereRigidBody);
    sphereRenderer->setProgram(textureShaderProgram);
    sphereRenderer->setTexture(sphereTexture);
    sphereRenderer->setScale(glm::vec3(1.0f));
    sphereRigidBody->setUserPointer(sphereRenderer);
    // Ground body
    btCollisionShape* groundShape = new btBoxShape(btVector3(4.0f,0.5f, 4.0f));
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1.0f, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0.0f, groundMotionState, groundShape, btVector3(0, 0, 0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

    groundRigidBody->setFriction(1.0f);
    groundRigidBody->setRestitution(0.0f);
    groundRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);

    dynamicsWorld->addRigidBody(groundRigidBody);

    // gruond visual
    ground = new MeshRenderer(kCube,"", camera,groundRigidBody);
    ground->setProgram(textureShaderProgram);
    ground->setTexture(groundTexture);
    ground->setScale(glm::vec3(4.0f,0.5f,4.0f));
    groundRigidBody->setUserPointer(ground);
}


