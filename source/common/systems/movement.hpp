
//#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include "../systems/collision-system.cpp"

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


    /**
     * @brief this is the movement system for (mode 1)
     * 
     */
    class GameMovement{
    
    public:
        

        our::CollisionSystem system; 

        

        // This should be called every frame to update all entities containing a MovementComponent. 
        bool update(World* world, float deltaTime) {
            
           system.mode1(world);
            // For each entity in the world
            for(auto entity : world->getEntities()){

                // Get the movement component if it exists
                MovementComponent* movement = entity->getComponent<MovementComponent>();
                CollisionComponent* Collisioncomp = entity->getComponent<CollisionComponent>();
                // If the movement component exists
                if(movement){
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    entity->localTransform.position += deltaTime * movement->linearVelocity;
                    if(Collisioncomp && Collisioncomp->store)
                        entity->localTransform.position.z += deltaTime * system.speedUp;

                    entity->localTransform.rotation += deltaTime * movement->angularVelocity;
                }
            }
            return system.checkLose();
            
        }


    };



    class GameMovement2{
    
    public:

        our::CollisionSystem system; 


        // This should be called every frame to update all entities containing a MovementComponent. 
        void update(World* world, float deltaTime) {
            
            system.mode2(world);
            // For each entity in the world
            for(auto entity : world->getEntities()){

                // Get the movement component if it exists
                MovementComponent* movement = entity->getComponent<MovementComponent>();
                CollisionComponent* Collisioncomp = entity->getComponent<CollisionComponent>();
                // If the movement component exists
                if(movement){
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    entity->localTransform.position += deltaTime * movement->linearVelocity;
                     if(Collisioncomp && Collisioncomp->store)
                        entity->localTransform.position.z += deltaTime * system.speedUp;

                    entity->localTransform.rotation += deltaTime * movement->angularVelocity;
                }
            }

            
        }

    };

}
