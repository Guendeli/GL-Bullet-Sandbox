#pragma once
#include "Camera.h"
#include "LightRenderer.h"

#include <GL/glew.h>

#include "Dependencies/glm/glm/glm.hpp"
#include "Dependencies/glm/glm/gtc/matrix_transform.hpp"
#include "Dependencies/glm/glm/gtc/type_ptr.hpp"
#include <btBulletDynamicsCommon.h>

class MeshRenderer
{
private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    glm::mat4 modelMatrix;

    Camera* camera;
    btRigidBody* rigidBody;
    glm::vec3 position,scale;
    GLuint vao, vbo, ebo, program, texture;
public:
    MeshRenderer(MeshType meshType,std::string _name, Camera* _camera, btRigidBody* _rb);
    ~MeshRenderer();
    std::string name = "";
    void draw();
    void setPosition(glm::vec3 _position);
    void setScale(glm::vec3 _scale);
    void setProgram(GLuint program);
    void setTexture(GLuint texture2D);
    
};
