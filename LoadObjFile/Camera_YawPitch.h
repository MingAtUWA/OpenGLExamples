#ifndef __Camera_Yaw_Pitch_h__
#define __Camera_Yaw_Pitch_h__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera_YawPitch
{
public:
    enum Movement
    {
        forward,
        backward,
        left,
        right
    };

protected:
    glm::vec3 view_pos;
    glm::vec3 view_dir;
    glm::vec3 view_up;
    glm::vec3 view_right;
    glm::vec3 world_up;

    // Euler angles
    float yaw, pitch;

    float speed;
    float sensitivity;

public:
    Camera_YawPitch(glm::vec3 &cam_pos,
           glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f),
           float _pitch = 0.0f, float _yaw = -90.0f) :
        view_pos(cam_pos),
        view_dir(glm::vec3(0.0f, 0.0f, -1.0f)),
        world_up(0.0f, 1.0f, 0.0f),
        pitch(_pitch), yaw(_yaw),
        speed(2.5f), sensitivity(0.1f)
    {
        update_camera_vectors();
    }

    inline glm::mat4 get_view_mat()
    {
        return glm::lookAt(view_pos, view_pos + view_dir, view_up);
    }

    inline void move_cam_pos(Movement direction, float dtime)
    {
        float disp = speed * dtime;
        switch (direction)
        {
        case forward:
            view_pos += view_dir * disp;
            break;
        case backward:
            view_pos -= view_dir * disp;
            break;
        case left:
            view_pos -= view_right * disp;
            break;
        case right:
            view_pos += view_right * disp;
            break;
        default:
            break;
        }
    }

    inline void move_cam_dir(float xoffset, float yoffset)
    {
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        update_camera_vectors();
    }

protected:
    inline void update_camera_vectors()
    {
        float yaw_rad = glm::radians(yaw);
        float pitch_rad = glm::radians(pitch);
        float cos_pitch = cos(pitch_rad);
        view_dir = glm::normalize(glm::vec3(cos_pitch * cos(yaw_rad), sin(pitch_rad), cos_pitch * sin(yaw_rad)));
        view_right = glm::normalize(glm::cross(view_dir, world_up));
        view_up = glm::normalize(glm::cross(view_right, view_dir));
    }
};

#endif