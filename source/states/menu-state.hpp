#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
//#include <systems/free-camera-controller.hpp>
//#include <systems/movement.hpp>
#include <asset-loader.hpp>

// This state shows how to use the ECS framework and deserialization.
class MenuState: public our::State {

    our::World world;
    our::ForwardRenderer renderer;

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("menuworld")){
            world.deserialize(config["menuworld"]);
        }        
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);

        //movementSystem.storeCollision(&world);
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        
        // And finally we use the renderer system to draw the scene
        renderer.render(&world);
        if(getApp()->getKeyboard().isPressed(GLFW_KEY_1)){
            getApp()->changeState("game");
        };
        if(getApp()->getKeyboard().isPressed(GLFW_KEY_2)){
            getApp()->changeState("game2");
        };
        if(getApp()->getKeyboard().isPressed(GLFW_KEY_Q)){
            glfwSetWindowShouldClose(getApp()->getWindow(), 1);
        };
        if(getApp()->getKeyboard().isPressed(GLFW_KEY_R)){
            getApp()->registerState<GameState>("game");
            getApp()->changeState("game");
        };  
    }

    ~MenuState(){
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
};