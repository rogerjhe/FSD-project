/**
 *  @file   main.cpp
 *  @brief  Entry point into the program.
 *  @author Mike and Carter Ithier
 *  @date   2020-09-10
 ***********************************************/

// g++ -std=c++17 *.cpp -o App -lsfml-graphics -lsfml-window -lsfml-system
//
// Note:	If your compiler does not support -std=c++17,
//		then try -std=c++14 then -std=c++11.
//
// HOW TO RUN
//
// ./App

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
// Include standard library C++ libraries.
#include <iostream>
#include <string>
// Project header files
#include "App.hpp"
#include "Command.hpp"
#include "Draw.hpp"
#include "ClearCanvas.hpp"
#include <memory>
#include "GUI.hpp"

// Preset color index needed to link app and gui together
static int preset = 1;

/*! \brief 	Call any initailization functions here.
*		This might be for example setting up any
*		global variables, allocating memory,
*		dynamically loading any libraries, or
*		doing nothing at all.
*
*/
void initialization(void){
	//std::cout << "Starting the App" << std::endl;
}


/*! \brief 	The update function presented can be simplified.
*		I have demonstrated two ways you can handle events,
*		if for example we want to add in an event loop.
*
*/
void update(App* &&app){
	// Handle key presses
	sf::Event event;
	while(app->GetWindow().pollEvent(event)){
        if (event.type == sf::Event::KeyPressed){
            sf::Keyboard::Key code = event.key.code;
            switch(code){
                case sf::Keyboard::Z:
                    app->Undo();
                    break;
                case sf::Keyboard::Y:
                    app->Redo();
                    break;
                case sf::Keyboard::Num1:
                    app->ChangeColor(sf::Color::Black);
                    preset = 1;
                    break;
                case sf::Keyboard::Num2:
                    app->ChangeColor(sf::Color::White);
                    preset = 2;
                    break;
                case sf::Keyboard::Num3:
                    app->ChangeColor(sf::Color::Red);
                    preset = 3;
                    break;
                case sf::Keyboard::Num4:
                    app->ChangeColor(sf::Color::Green);
                    preset = 4;
                    break;
                case sf::Keyboard::Num5:
                    app->ChangeColor(sf::Color::Blue);
                    preset = 5;
                    break;
                case sf::Keyboard::Num6:
                    app->ChangeColor(sf::Color::Yellow);
                    preset = 6;
                    break;
                case sf::Keyboard::Num7:
                    app->ChangeColor(sf::Color::Magenta);
                    preset = 7;
                    break;
                case sf::Keyboard::Num8:
                    app->ChangeColor(sf::Color::Cyan);
                    preset = 8;
                    break;
                case sf::Keyboard::Space:
                {
                    std::shared_ptr<Command> clear_command = std::make_shared<ClearCanvas>("clear",
                        app->GetCurrentColor(), app->GetBackgroundColor(), *app);
                    app->AddCommand(clear_command);
                    app->ExecuteCommand();
                }
                    break;
                default:
                    break;
            }
        }
	}

	// We can otherwise handle events normally
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		sf::Vector2i coordinate = sf::Mouse::getPosition(app->GetWindow());
		// Add command to queue if it is unique
		std::shared_ptr<Command> command = std::make_shared<Draw>("draw", coordinate,
                app->GetCurrentColor(), app->GetBackgroundColor(), *app);

        app->AddCommand(command);
        app->ExecuteCommand();

	}

	// Capture any keys that are released
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
		exit(EXIT_SUCCESS);
	}

}


/*! \brief 	The draw call
*
*/
void draw(App *&&app){
	// Static variable
	static int refreshRate = 0;
	++refreshRate;	// Increment


	// We load into our texture the modified pixels
	// But we only do so every 10 draw calls to reduce latency of transfer
	// between the GPU and CPU.
	// Ask yourself: Could we do better with sf::Clock and refresh once
	// 	 	 every 'x' frames?
	if(refreshRate > 10){
		app->GetTexture().loadFromImage(app->GetImage());
		refreshRate = 0;
	}
}


/*! \brief 	The entry point into our program.
*
*/
int main(){
	// Call any setup function
	// Passing a function pointer into the 'init' function.
	// of our application.

	App *app = new App;
    GUI *gui = new GUI(app);
    
	app->Init(&initialization);
	// Setup your keyboard
	app->UpdateCallback(&update);
	// Setup the Draw Function
	app->DrawCallback(&draw);
	// Call the main loop function
    while (app->GetWindow().isOpen() && gui->getWindow()->isOpen()) {
        app->Loop();
        gui->setPresetIndex(preset);
        gui->loop();
        preset = gui->getPreset();
    }

	// Destroy our app
	app->Destroy();

	delete app;

	return 0;
}
