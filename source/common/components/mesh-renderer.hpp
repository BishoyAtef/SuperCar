#pragma once

#include "../ecs/component.hpp"
#include "../mesh/mesh.hpp"
#include "../material/material.hpp"
#include "../asset-loader.hpp"

namespace our {

    // This component denotes that any renderer should draw the given mesh using the given material at the transformation of the owning entity.
    class MeshRendererComponent : public Component {
    public:
        Mesh* mesh; // The mesh that should be drawn
        Material* material; // The material used to draw the mesh
        // The ID of this component type is "Mesh Renderer"
        static std::string getID() { return "Mesh Renderer"; }

        // Receives the mesh & material from the AssetLoader by the names given in the json object
        void deserialize(const nlohmann::json& data) override;
    };

    class CollisionComponent : public Component {
    
    public:
        float radius;
        bool store;
        std::string function = "";
        // The ID of this component type is "Mesh Renderer"
        static std::string getID() { return "Collision"; }

        // Receives the mesh & material from the AssetLoader by the names given in the json object
        void deserialize(const nlohmann::json& data) override;
    };


    class LightComponent : public Component {
    
    
    public:
        glm::vec3 diffuse = glm::vec3(0, 0, 0);
        glm::vec3 specular = glm::vec3(0, 0, 0);
        glm::vec3 attenuation = glm::vec3(1, 0, 0);
        glm::vec2 coneAngles = glm::vec2(10, 11);
        // 0 -> directional    2 -> spot(coneAngles)  1-> point(attenuation)
        int lightType;
        // The ID of this component type is "Mesh Renderer"
        static std::string getID() { return "Light"; }

        // Receives the mesh & material from the AssetLoader by the names given in the json object
        void deserialize(const nlohmann::json& data) override;
    };

}