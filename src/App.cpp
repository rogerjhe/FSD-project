/**
 *  @file   App.cpp
 *  @brief  Main class for program
 *  @author Mike and Carter Ithier
 *  @date   2020-09-10
 ***********************************************/

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
// Include standard library C++ libraries.
#include <cassert>
// Project header files
#include "App.hpp"
#include "Draw.hpp"
#include <iostream>


/*! \brief Constructor for App class
	\param m_initFunc(nullptr) function pointer to initialization in main.cpp
*/
App::App(): m_window(nullptr), m_image(new sf::Image), m_sprite(new sf::Sprite),
m_texture(new sf::Texture), m_initFunc(nullptr), m_updateFunc(nullptr), m_drawFunc(nullptr),
windowWidth(600), windowHeight(400), m_numUndos(10), m_currentColor(sf::Color::Black),
 m_backgroundColor(sf::Color::White)
{

}


/*! \brief Change the current color for Drawing.
	\param color sf::Color name of color to be changed to
*/
void App::ChangeColor(sf::Color color){
    m_currentColor = color;
}

/*! \brief Get the current drawing color
*/
sf::Color App::GetCurrentColor(){
    return m_currentColor;
}

/*! \brief Get background color
*/
sf::Color App::GetBackgroundColor(){
    return m_backgroundColor;
}

/*! \brief Sets background color
	\param color sf::Color being changed to
*/
void App::SetBackgroundColor(sf::Color color){
    m_backgroundColor = color;
}

/*! \brief See if current command is equal to the most recent command that was
 * put in the m_commands in App
*/
bool App::commandExists(std::shared_ptr<Command> command){
 const std::shared_ptr<Command> lastcommand = GetLastCommand();
    if (lastcommand){
        return command->compare(lastcommand);
    } else{
        return false;
    }
}

/*! \brief Get the width of the window in our App class.
	\return Pixel count of the width
*/
int App::GetWindowWidth(){
    return windowWidth;
}

/*! \brief Get the height of the window in our App class.
	\return Pixel count of the height
*/
int App::GetWindowHeight(){
    return windowHeight;
}


/*! \brief 	Add a command to a data structure to later be executed.
	\param c command object to be added to deque holding all the commands
*/
void App::AddCommand(std::shared_ptr<Command> c){
    // add command if it doesn't already exist
    if (!commandExists(c)){
        // add to command deque
        m_commands.push_back(c);
    }
}

/*! \brief Add command to deque of commands that can be undone.
	\param c command object to be added to undo deque
*/
void App::AddUndo(std::shared_ptr<Command> c){
    // add to possible actions that can be undone
    m_undo.push_front(c);
    if (m_undo.size() > 10){
        m_undo.pop_back();
    }
}

/*! \brief Undo the most recent command
*/
void App::Undo(){
    if (m_undo.size() > 0){
        std::cout << "Called undo" << std::endl;
        std::shared_ptr<Command> command = m_undo.front();
        m_undo.pop_front();
        command->undo();
        m_redo.push(command);
    }
}

/*! \brief Redo the last command that was undone.
*/
void App::Redo(){
    if (m_redo.size() > 0){
        std::cout << "Called redo" << std::endl;
        std::shared_ptr<Command> command = m_redo.top();
        m_redo.pop();
        command->execute();
        m_undo.push_front(command);
    }
}


/*! \brief 	Execute the oldest commands in a data structure of commands.
*
*/
void App::ExecuteCommand(){
    if (m_commands.size() > 0){
        std::shared_ptr<Command> command = m_commands.front();
        bool success = command->execute();
        if (success){
            m_lastcommand = command;
            // clear redo stack because another command executed
            while (m_redo.size() > 0){
                m_redo.pop();
            }
            AddUndo(command);
        }
        m_commands.pop_front();

    }
}

/*! \brief Return the most recent command put in the queue.
	\return the command that was last added
*/
const std::shared_ptr<Command> App::GetLastCommand(){
    return m_lastcommand;
}

/*! \brief 	Return a reference to our m_image, so that
*		we do not have to publicly expose it.
	\return Pointer to image
*/
sf::Image& App::GetImage(){
	return *m_image;
}

/*! \brief 	Return a reference to our m_Texture so that
*		we do not have to publicly expose it.
	\return Pointer to texture
*/
sf::Texture& App::GetTexture(){
	return *m_texture;
}

/*! \brief 	Return a reference to our m_window so that we
*		do not have to publicly expose it.
	\return Pointer to window
*/
sf::RenderWindow& App::GetWindow(){
	return *m_window;
}

/*! \brief 	Destroy we manually call at end of our program.
*
*/
void App::Destroy(){
	delete m_image;
	delete m_sprite;
	delete m_texture;

}

/*! \brief 	Initializes the App and sets up the main
*		rendering window(i.e. our canvas.)
	\param (*initFunction)(void) function pointer to our initilization function in main.cpp
*/
void App::Init(void (*initFunction)(void)){
	// Create our window
	m_window = new sf::RenderWindow(sf::VideoMode(App::windowWidth, App::windowHeight),"Mini-Paint alpha 0.0.2",sf::Style::Titlebar);
	m_window->setVerticalSyncEnabled(true);
	// Create an image which stores the pixels we will update
	m_image->create(App::windowWidth, App::windowHeight,sf::Color::White);
	assert(m_image != nullptr && "m_image != nullptr");
	// Create a texture which lives in the GPU and will render our image
	m_texture->loadFromImage(*m_image);
	assert(m_texture != nullptr && "m_texture != nullptr");
	// Create a sprite which is the entity that can be textured
	m_sprite->setTexture(*m_texture);
	assert(m_sprite != nullptr && "m_sprite != nullptr");
	// Set our initialization function to perform any user
	// initialization
	m_initFunc = initFunction;
}

/*! \brief 	Set a callback function which will be called
		each iteration of the main loop before drawing.
*
*/
void App::UpdateCallback(void (*updateFunction)(App *&&app)){
	m_updateFunc = updateFunction;
}

/*! \brief 	Set a callback function which will be called
		each iteration of the main loop after update.
*
*/
void App::DrawCallback(void (*drawFunction)(App *&&app)){
	m_drawFunc = drawFunction;
}

/*! \brief 	The main loop function which handles initialization
		and will be executed until the main window is closed.
		Within the loop function the update and draw callback
		functions will be called.
*
*/
void App::Loop(){
	// Call the init function
	m_initFunc();

	// Start the main rendering loop
	// Clear the window
	m_window->clear();
	// Updates specified by the user
	m_updateFunc(this);
	// Additional drawing specified by user
	m_drawFunc(this);
	// Update the texture
	// Note: This can be done in the 'draw call'
	// Draw to the canvas
	m_window->draw(*m_sprite);
	// Display the canvas
	m_window->display();
}


