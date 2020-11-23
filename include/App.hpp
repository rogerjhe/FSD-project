/**
 *  @file   App.hpp
 *  @brief  App class interface
 *  @author Mike and Carter Ithier
 *  @date   2020-09-10
 ***********************************************/
#ifndef APP_HPP
#define APP_HPP

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
// Include standard library C++ libraries.
#include <queue>
#include <stack>
#include "Command.hpp"
#include <memory>
// Project header files
// #include ...

// Singleton for our Application called 'App'.
class App{
private:
    // member variables
    sf::Color m_backgroundColor;
    sf::Color m_currentColor;
    int windowWidth;
	int windowHeight;
    sf::RenderWindow* m_window;
	// Deque stores the next command to do.
	std::deque<std::shared_ptr<Command>> m_commands;
	// Deque that stores the last action to occur.
	std::deque<std::shared_ptr<Command>> m_undo;
	// Stack that stores operations that can be redone
    std::stack<std::shared_ptr<Command>> m_redo;
	// Main image
	sf::Image* m_image;
	// Create a sprite that we overaly
	// on top of the texture.
	sf::Sprite* m_sprite;
	// Texture sent to the GPU for rendering
	sf::Texture* m_texture;
	// amount of undos allowed
    int m_numUndos;
	// hold the last command that was added to m_commands
	std::shared_ptr<Command> m_lastcommand;

    // Member functions
	// Store the address of our funcion pointer
	// for each of the callback functions.
	void (*m_initFunc)(void);
	void (*m_updateFunc)(App *&&app);
	void (*m_drawFunc)(App *&&app);
	bool commandExists(std::shared_ptr<Command> command);
	App(const App&);

public:
    App();
    // Member functions
    void ChangeColor(sf::Color color);
    sf::Color GetCurrentColor();
    sf::Color GetBackgroundColor();
    void SetBackgroundColor(sf::Color color);
    void 	AddCommand(std::shared_ptr<Command> c);
	void 	ExecuteCommand();
	sf::Image& GetImage();
	sf::Texture& GetTexture();
	sf::RenderWindow& GetWindow();
	void Undo();
	void Redo();
	const std::shared_ptr<Command> GetLastCommand();
	int GetWindowWidth();
	int GetWindowHeight();

	void Destroy();
	void Init(void (*initFunction)(void));
	void UpdateCallback(void (*updateFunction)(App *&&app));
	void DrawCallback(void (*drawFunction)(App *&&app));
	void Loop();
	void AddUndo(std::shared_ptr<Command> c);


};


#endif
