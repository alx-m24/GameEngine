#include "Camera.hpp"

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));  
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::update(GLFWwindow* window, float dt)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        this->move(Camera::FORWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        this->move(Camera::BACKWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        this->move(Camera::LEFT, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        this->move(Camera::RIGHT, dt);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        this->move(Camera::FORWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        this->move(Camera::BACKWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        this->move(Camera::LEFT, dt);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        this->move(Camera::RIGHT, dt);
}

void Camera::move(Movement movement, float dt)
{
    float velocity = MovementSpeed * dt;

    if (movement == FORWARD) Position += Front * velocity;
    if (movement == BACKWARD) Position -= Front * velocity;
    if (movement == LEFT) Position -= Right * velocity;
    if (movement == RIGHT) Position += Right * velocity;
}

void Camera::rotate(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::zoom(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}
