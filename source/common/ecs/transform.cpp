#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 2) Write this function
        //Builds a translation 4 * 4 matrix created from a vector of 3 components representing the translation matrix in the homogenous coordinates
        glm::mat4 translation = glm::translate(
            glm::mat4(1.0f),
            position
        );
        //Builds a translation 4 * 4 matrix created from a vector of 3 components representing the scaling matrix in the homogenous coordinates        
        glm::mat4 scaling = glm::scale(
            glm::mat4(1.0f),
            scale
        );
        
        //creates a 3D 4*4 homogeneous rotation matrix from euler angles (y * x * z). 
        glm::mat4 rotate = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        //multiply the three matrices in this order so that scaling is first multiplied to the object
        return translation * rotate * scaling;
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}