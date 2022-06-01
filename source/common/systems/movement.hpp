#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/movement.hpp"
    class MovementSystem {
    public:

        // This should be called every frame to update all entities containing a MovementComponent. 
        void update(World* world, float deltaTime) {
            // For each entity in the world
            for(auto entity : world->getEntities()){
                // Get the movement component if it exists
                MovementComponent* movement = entity->getComponent<MovementComponent>();
                // If the movement component exists
                if(movement){
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    entity->localTransform.position += deltaTime * movement->linearVelocity;
                    entity->localTransform.rotation += deltaTime * movement->angularVelocity;
                }
            }
        }

    };


    class GameMovement : public MovementSystem {
    
    public:

        std::vector<CollisionComponent*> collisionObjects;
        std::vector<glm::vec3> collisionPositions;

        glm::vec3 carPosition;
        float carRadius = 0.0;
        void storeCollision(World* world) {
            std::cout<<"storing the collisions"<<std::endl<<std::endl;
            for(auto entity : world->getEntities()){
                CollisionComponent* collision = entity->getComponent<CollisionComponent>();
                if(!(collision->store)) {
                
                    carRadius = collision->radius;
                    carPosition = entity->localTransform.position;
                    
                }else {
                    // If the movement component exists
                    collisionObjects.push_back(collision);
                    collisionPositions.push_back(entity->localTransform.position);
                }
            }
        }

        bool checkCollision() {
            for(int i=0; i<collisionObjects.size(); i++) {
                glm::vec3 dist = collisionPositions[i] - carPosition;
                float temp1 = glm::dot(dist, dist);
                if(glm::sqrt(temp1) <= collisionObjects[i]->radius + carRadius) {
                    std::cout<<"there is a collision"<<std::endl<<std::endl;
                    return true;
                }
                std::cout<<"no collision for object "<<i<<std::endl<<std::endl;
            }
            return false;
        }


        // This should be called every frame to update all entities containing a MovementComponent. 
        void update(World* world, float deltaTime) {
            bool collide = checkCollision();
            if(collide) {
                std::cout<<"there is a collision"<<std::endl<<std::endl;
            }

            // For each entity in the world
            for(auto entity : world->getEntities()){

                // Get the movement component if it exists
                MovementComponent* movement = entity->getComponent<MovementComponent>();
                // If the movement component exists
                if(movement){
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    entity->localTransform.position += deltaTime * movement->linearVelocity;
                    entity->localTransform.rotation += deltaTime * movement->angularVelocity;
                }
            }
        }

    };

}
