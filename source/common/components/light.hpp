#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {
   class LightComponent : public Component {
    public:
        // 0 -> directional    2 -> spot(coneAngles)  1-> point(attenuation)
        int lightType = 0;
        glm::vec3 diffuse =  glm::vec3(1, 0.2, 0.1);
        glm::vec3 specular =  glm::vec3(1, 0.2, 0.1);
        glm::vec3 attenuation = glm::vec3(1, 0, 0);
        glm::vec2 cone_angles = glm::vec2(10.0f, 11.0f);

        // The ID of this component type is "Light"
        static std::string getID() { return "Light"; }

        // Reads data from the given json object
        void deserialize(const nlohmann::json& data) override;
    };
}