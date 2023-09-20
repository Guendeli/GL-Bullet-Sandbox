#include "MeshRenderer.h"

#include <ctime>

MeshRenderer::MeshRenderer(MeshType meshType,std::string _name, Camera* _camera, btRigidBody* _rb)
{
    name = _name;
    camera = _camera;
    rigidBody = _rb;
    scale = glm::vec3(1.0f,1.0f,1.0f);
    position = glm::vec3(0.0f,0.0f,0.0f);
    switch(meshType)
    {
    case kTriangle: Mesh::setTriData(vertices, indices);
        break;
    case kQuad: Mesh::setQuadData(vertices, indices);
        break;
    case kCube: Mesh::setCubeData(vertices, indices);
        break;
    case kSphere: Mesh::setSphereData(vertices, indices);
        break;
    }

    // Array bindings
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Vertex Attributes
    glEnableVertexAttribArray(0); // 0 - POSITON
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    
    glEnableVertexAttribArray(1); // 1 - TEXTURE
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::texCoords)));

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
}


void MeshRenderer::draw()
{
    // glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    //
    // modelMatrix = glm::mat4(1.0f);
    // modelMatrix = translationMatrix * scaleMatrix;

    // Welcome physics
    btTransform t;
    rigidBody->getMotionState()->getWorldTransform(t);

    btQuaternion rotation = t.getRotation();
    btVector3 translation = t.getOrigin();
    

    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation.getAngle(), glm::vec3(rotation.getAxis().getX(),rotation.getAxis().getY(),rotation.getAxis().getZ()));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(translation.getX(),translation.getY(), translation.getZ()));
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

    modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    
    glm::mat4 vp = camera->getProjectionMatrix() * camera->getViewMatrix();
    glUseProgram(this->program);
    GLint vpLoc = glGetUniformLocation(program, "vp");
    glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(vp));

    GLint modelLoc = glGetUniformLocation(program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void MeshRenderer::setTexture(GLuint texture2D)
{
    texture = texture2D;
}

void MeshRenderer::setScale(glm::vec3 _scale)
{
    this->scale = _scale;
}

void MeshRenderer::setPosition(glm::vec3 _position)
{
    this->position = _position;
}

void MeshRenderer::setProgram(GLuint program)
{
    this->program = program;
}



