#pragma once

#include "base/Object.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <vector>

class aiNodeAnim;

namespace coral
{
    struct KeyPosition
    {
        glm::vec3 position;
        double timeStamp;
    };

    struct KeyRotation
    {
        glm::quat orientation;
        double timeStamp;
    };

    struct KeyScale
    {
        glm::vec3 scale;
        double timeStamp;
    };

    class Bone : public Object
    {
    public:
        /**
         * @brief Constructor, read keyframe from assimp node anim
         */
        Bone(const std::string& name, int id, const aiNodeAnim* channel);

        // Interpolates b/w positions,rotations & scaling keys based on the curren time of the animation and prepares the local transformation matrix by combining all keys tranformations
        // animationTime is in seconds
        void update(double animationTime);

        glm::mat4 getLocalTransform();
        std::string getBoneName() const;
        int getBoneID();

        // Gets the current index on mKeyPositions to interpolate to based on the current animation time
        size_t getPositionIndex(double animationTime);

        // Gets the current index on mKeyRotations to interpolate to based on the current animation time
        size_t getRotationIndex(double animationTime);

        // Gets the current index on mKeyScalings to interpolate to based on the current animation time
        size_t getScaleIndex(double animationTime);

    private:
        // Gets normalized value for Lerp & Slerp
        double getScaleFactor(double lastTimeStamp, double nextTimeStamp, double animationTime);

        // figures out which position keys to interpolate b/w and performs the interpolation  and returns the translation matrix
        glm::mat4 interpolatePosition(double animationTime);

        // figures out which rotations keys to interpolate b/w and performs the interpolation  and returns the rotation matrix
        glm::mat4 interpolateRotation(double animationTime);

        // figures out which scaling keys to interpolate b/w and performs the interpolation  and returns the scale matrix
        glm::mat4 interpolateScaling(double animationTime);

    private:
        std::vector<KeyPosition> positions;
        std::vector<KeyRotation> rotations;
        std::vector<KeyScale> scales;
        size_t numPositions;
        size_t numRotations;
        size_t numScalings;

        glm::mat4 localTransform;
        std::string name;
        int id;
    };
}
