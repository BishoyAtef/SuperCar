
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


    /**
     * @brief this is the movement system for (mode 1)
     * 
     */
    class GameMovement{
    
    public:
        // these vectors store the collision components, collision positions, collision entities
        // and collideBefore List to indicate that a collision object has collided with the car before 
        std::vector<CollisionComponent*> collisionObjects;
        std::vector<glm::vec3> collisionPositions;
        std::vector<Entity*> collisionEntities;
        std::vector<bool> collideBefore;

        // storing the carPosition, carRadius and carEntity 
        glm::vec3 carPosition;
        float carRadius = 0.0;
        Entity* carEntity;

        // the speedup of the car
        float speedUp = 0.0;

        // storing the number of collisionsObjects to track that the game has ended
        int numOfCollisions = 0;

        // the number of actual hits and number of hits
        // if the number of hits = the number of actual hits
        // then the player loses
        const int numberOfHits = 2;
        int actualNumberOfHits = 0;

        /**
         * @brief this function stores some informmation about the collision objects like 
         * their CollisionComponents and Entities
         * 
         * @param world the world of entities 
         */
        void storeCollision(World* world) {
            std::cout<<"storing the collisions"<<std::endl<<std::endl;
            for(auto entity : world->getEntities()){
                CollisionComponent* collision = entity->getComponent<CollisionComponent>();
                // if it is a collision object
                if(collision) {
                    // check that this object is a car to store the radius and the car Entity
                    if(!(collision->store)) {
                        std::cout<<"storing the car radius"<<std::endl<<std::endl;
                        carRadius = collision->radius;
                        carEntity = entity;
                        
                    }
                    // the collisionObject is a barrier or a speedup
                    else {
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

        /**
         * @brief Get the Positions of objects (car, barriers and speedup)
         * 
         * @param world the world of entities
         */
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

        /**
         * @brief checks that there is a collision between the car and any other object
         * 
         */
        void checkCollision() {
            for(int i=0; i<collisionObjects.size(); i++) {
                if(glm::distance(collisionPositions[i], carPosition) <= (collisionObjects[i]->radius + carRadius)) {
                    //std::cout<<"there is a collision"<<std::endl<<std::endl;
                    
                    std::cout<<"collision at radius "<<collisionObjects[i]->radius<<std::endl<<std::endl;
                    // checking that the collision is with a speedup object
                    checkSpeedUp(i);
                    // checking that the collision is with a barrier object
                    checkBarrier(i);
                  
                }
            }
        }

        /**
         * @brief this function checks the collision with a speedup 
         * 
         * @param i index represents the position of the object in the collision and entity lists
         */
        void checkSpeedUp(int i) {
            if(collisionObjects[i]->function == "speedup" && !collideBefore[i]){
                std::cout<<"collision with a speedup"<<std::endl<<std::endl;
                speedUp +=5;
                collideBefore[i] = true;
                collisionEntities[i]->drawMesh = false;
                numOfCollisions --;
            }
        }

         /**
         * @brief this function checks the collision with a barrier
         * 
         * @param i index represents the position of the object in the collision and entity lists
         */
        void checkBarrier(int i) {
            if(collisionObjects[i]->function == "barrier" && !collideBefore[i]){
                std::cout<<"collision with a barrier"<<std::endl<<std::endl;
                collideBefore[i] = true;
                collisionEntities[i]->drawMesh = false;
                numOfCollisions --;
                actualNumberOfHits ++;
            }

        }


         /**
         * @brief this function checks that a car has passed by a barrier in order to track the number of remaining barriers
         * 
         */
        void checkPassByBarrier() {
             for(int i=0; i<collisionObjects.size(); i++) {
               if((collisionPositions[i].z >= carPosition.z)  && !collideBefore[i] ) {
                   // decreasing the number of remaining barriers
                   numOfCollisions --;
                   // indicating that this collision object has been passedby
                   // in order to not to check again
                   collideBefore[i] = true;
                   std::cout<<"Passed by a Barrier"<<std::endl<<std::endl;
               }

            }
        }

        /**
         * @brief checks the end of the game if there are no remaining objects 
         * 
         */
        void checkEnd() {
            std::cout<<"Number of collisions left is "<<numOfCollisions<<std::endl<<std::endl;
            if(numOfCollisions == 0) {
                std::cout<<"the game has ended"<<std::endl<<std::endl;
            }
        }

        /**
         * @brief checks that player loses if hitting 2 barriers
         * 
         * @return true if the player hitted to barriers
         * @return false if the player did not hit 2 barriers
         */
        bool checkLose() {
            std::cout<<"Number of hits is "<<actualNumberOfHits<<std::endl<<std::endl;
            if(actualNumberOfHits >= numberOfHits) {
                std::cout<<"YOU Lose , You hitted 2 barriers"<<std::endl<<std::endl;
                return true;
            }
            return false;
        }

        // This should be called every frame to update all entities containing a MovementComponent. 
        bool update(World* world, float deltaTime) {
            
            // first we get the positions of all entities
            getPositions(world);
            // second we check that the car has passed by a barrier
            checkPassByBarrier();
            checkEnd();
            checkCollision();
            
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
            return checkLose();
            
        }


    };



    class GameMovement2{
    
    public:

        std::vector<CollisionComponent*> collisionObjects;
        std::vector<glm::vec3> collisionPositions;
        std::vector<Entity*> collisionEntities;
        std::vector<bool> collideBefore;

        glm::vec3 carPosition;
        float carRadius = 0.0;
        Entity* carEntity;

        float speedUp = 0.0;

        int numOfCollisions = 0;

        int score = 0;

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

        void checkCollision() {
            for(int i=0; i<collisionObjects.size(); i++) {
                if(glm::distance(collisionPositions[i], carPosition) <= (collisionObjects[i]->radius + carRadius)) {
                    //std::cout<<"there is a collision"<<std::endl<<std::endl;
                    
                    std::cout<<"collision at radius "<<collisionObjects[i]->radius<<std::endl<<std::endl;
                    checkSpeedUp(i);
                    checkBarrier(i);
                  
                }
                //std::cout<<"radius of the collision is "<<collisionObjects[i]->radius<<std::endl<<std::endl;
            }
        }

        void checkSpeedUp(int i) {
            if(collisionObjects[i]->function == "speedup" && !collideBefore[i]){
                std::cout<<"collision with a speedup"<<std::endl<<std::endl;
                speedUp +=5;
                score += 10;
                collideBefore[i] = true;
                collisionEntities[i]->drawMesh = false;
                numOfCollisions --;
            }
        }

        void checkBarrier(int i) {
            if(collisionObjects[i]->function == "barrier" && !collideBefore[i]){
                std::cout<<"collision with a barrier"<<std::endl<<std::endl;
                collideBefore[i] = true;
                collisionEntities[i]->drawMesh = false;
                numOfCollisions --;
                score = (score<=0) ? 0 : score - 1;
            }

        }

        void checkPassByBarrier() {
             for(int i=0; i<collisionObjects.size(); i++) {
               if((collisionPositions[i].z >= carPosition.z)  && !collideBefore[i] ) {
                   numOfCollisions --;
                   collideBefore[i] = true;
                   std::cout<<"Passed by a Barrier"<<std::endl<<std::endl;
                   score +=1 ;
               }

            }
        }

        void checkWin() {
            std::cout<<"Number of collisions left is "<<numOfCollisions<<std::endl<<std::endl;
            if(numOfCollisions == 0) {
                std::cout<<"YOU Ended the run with score = "<<score<<std::endl<<std::endl;
            }
        }

        // This should be called every frame to update all entities containing a MovementComponent. 
        void update(World* world, float deltaTime) {
            
            getPositions(world);
            checkPassByBarrier();
            checkWin();
            checkCollision();
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

            
        }

    };

}
