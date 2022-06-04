#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>

// This state shows how to use the ECS framework and deserialization.
class Playstate: public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);

        //movementSystem.storeCollision(&world);
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        // And finally we use the renderer system to draw the scene
        //renderer.render(&world);
    }

    void onDestroy() override {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
};

/**
 * @brief this class represents the first mode/state of the game 
 * In this mode (mode 1), if the car hitted two barriers then the player loses 
 * 
 */
class GameState: public our::State {

    bool loaded = false;
    our::World world;
    our::ForwardRenderer renderer;
    our::GameCameraController cameraController;
    our::GameMovement movementSystem;

    void onInitialize() override {
        if (loaded){
            return;
        }
        loaded = true;
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);

        // we initialize the collision components
        movementSystem.system.storeCollision(&world);
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        // the update function checks that the player has lost the game or not and updates the position of all entities
        bool lose = movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        // And finally we use the renderer system to draw the scene 
        // if the player has lost then we render the same frame each time
        if (!lose)
            renderer.render(&world);

        // if the  (Q) button is hitted then return to the menu
        if(getApp()->getKeyboard().isPressed(GLFW_KEY_Q)){
            getApp()->changeState("menu");
        };
    }
    ~GameState(){
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();        
    }
};



/**
 * @brief this class represents the second mode/state of the game 
 * In this mode (mode 2), the player deos not lose but we track his score
 * if he hits a barrier then his score decreases by 1
 * if he gets a SpeedUp then his speed will increase and his score will increase   
 * 
 */
class GameState2: public our::State {

    bool loaded = false;
    our::World world;
    our::ForwardRenderer renderer;
    our::GameCameraController cameraController;
    our::GameMovement2 movementSystem;

    void onInitialize() override {
        if (loaded){
            return;
        }
        loaded = true;
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);

        // storing the collision objects
        movementSystem.system.storeCollision(&world);
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        // And finally we use the renderer system to draw the scene
        renderer.render(&world);

        if(getApp()->getKeyboard().isPressed(GLFW_KEY_Q)){
            getApp()->changeState("menu");
        };
    }
    ~GameState2(){
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();        
    }
};