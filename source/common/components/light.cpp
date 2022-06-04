#include "./light.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"


namespace our {
    void LightComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        lightType = data.value("lightType", lightType)
        diffuse = data.value("diffuse", diffuse);
        specular = data.value("specular", specular);
        attenuation = data.value("attenuation", attenuation);
        cone_angles =  glm::radians(data.value("coneAngles", cone_angles));
    }
}