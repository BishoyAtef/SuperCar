
//#pragma once

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


    class GameMovement{
    
    public:

        std::vector<CollisionComponent*> collisionObjects;
        std::vector<glm::vec3> collisionPositions;
        std::vector<Entity*> collisionEntities;
        std::vector<bool> collideBefore;

        glm::vec3 carPosition;
        float carRadius = 0.0;
        Entity* carEntity;

        float roadPosition_x = 15.0;

        float speedUp = 0.0;

        int numOfCollisions = 0;

        void storeCollision(World* world) {
            std::cout<<"storing the collisions"<<std::endl<<std::endl;
            for(auto entity : world->getEntities()){
                CollisionComponent* collision = entity->getComponent<CollisionComponent>();
                if(collision) {
                    if(!(collision->store)) {
                        std::cout<<"storing the car radius"<<std::endl<<std::endl;
                        carRadius = collision->radius;
                        //carPosition = entity->localTransform.position;
                        carEntity = entity;
                        
                    }else {
                        // If the movement component exists
                        std::cout<<"storing a possible collision"<<std::endl<<std::endl;
                        collisionObjects.push_back(collision);
                        collisionEntities.push_back(entity);
                        collideBefore.push_back(false);
                    }
                }
            }
            numOfCollisions = collisionObjects.size();
            std::cout<<"number of possible collisions is "<<numOfCollisions<<std::endl<<std::endl;
        }

        void getPositions(World* world) {
            collisionPositions.clear();
            for(auto entity : world->getEntities()){
                CollisionComponent* collision = entity->getComponent<CollisionComponent>();
                if(collision) {
                    if(!(collision->store)) {
                    
                        carPosition = entity->getLocalToWorldMatrix()*glm::vec4(0,0,0,1);
                        
                    }else {
                        collisionPositions.push_back(entity->getLocalToWorldMatrix()*glm::vec4(0,0,0,1));
                    }
                }
            }
        }

        CollisionComponent* checkCollision() {
            for(int i=0; i<collisionObjects.size(); i++) {
                if(glm::distance(collisionPositions[i], carPosition) <= (collisionObjects[i]->radius + carRadius)) {
                    //std::cout<<"there is a collision"<<std::endl<<std::endl;
                    
                    std::cout<<"collision at radius "<<collisionObjects[i]->radius<<std::endl<<std::endl;
                    CollisionComponent* comp1 = checkSpeedUp(i);
                    if(! comp1)
                        return checkBarrier(i);
                    return comp1;
                }
                //std::cout<<"radius of the collision is "<<collisionObjects[i]->radius<<std::endl<<std::endl;
            }
            return nullptr;
        }

        CollisionComponent* checkSpeedUp(int i) {
            if(collisionObjects[i]->function == "speedup" && !collideBefore[i]){
                std::cout<<"collision with a speedup"<<std::endl<<std::endl;
                speedUp +=5;
                collideBefore[i] = true;
                return collisionObjects[i];
            }
            collisionObjects[i] = nullptr;
            return nullptr;
        }

        CollisionComponent* checkBarrier(int i) {
            if(collisionObjects[i]->function == "barrier" && !collideBefore[i]){
                std::cout<<"collision with a barrier"<<std::endl<<std::endl;
                collideBefore[i] = true;
                return collisionObjects[i];
            }
            collisionObjects[i] = nullptr;
            return nullptr;

        }
        // This should be called every frame to update all entities containing a MovementComponent. 
        void update(World* world, float deltaTime) {
            
            getPositions(world);

            // For each entity in the world
            for(auto entity : world->getEntities()){

                // Get the movement component if it exists
                MovementComponent* movement = entity->getComponent<MovementComponent>();
                // If the movement component exists
                if(movement){
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    entity->localTransform.position += deltaTime * movement->linearVelocity;
                    entity->localTransform.position.z += deltaTime * speedUp;

                    entity->localTransform.rotation += deltaTime * movement->angularVelocity;
                }
            }

            //return checkCollision();
        }

    };

}
