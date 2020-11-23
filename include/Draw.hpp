/**
 *  @file   Draw.hpp
 *  @brief  Drawing actions interface.
 *  @author Mike and Carter Ithier
 *  @date   2020-09-10
 ***********************************************/
#ifndef DRAW_H
#define DRAW_H

// Include our Third-Party SFML header
// #include ...
// Include standard library C++ libraries.
#include <string>
// Project header files
#include "Command.hpp"
#include "App.hpp"
#include <memory>

// Anytime we want to implement a new command in our paint tool,
// we have to inherit from the command class.
// This forces us to implement an 'execute' and 'undo' command.
class Draw : public Command{
	public:
        Draw(const std::string &m_commandDescription, sf::Vector2i coord, const sf::Color &color,
            const sf::Color &background, App &app);
        ~Draw();
    private:
        App& m_app;
        sf::Vector2i m_coords;
        sf::Color m_color;
        sf::Color m_background;
        bool execute();
        bool undo();
        bool compare(const std::shared_ptr<Command> &rhs);
        bool InBounds();

};


#endif
