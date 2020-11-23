/**
 *  @file   Draw.hpp
 *  @brief  Drawing actions interface.
 *  @author Mike and Carter Ithier
 *  @date   2020-09-10
 ***********************************************/
#ifndef CLEAR_H
#define CLEAR_H

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
class ClearCanvas : public Command{
	public:
        ClearCanvas(const std::string &m_commandDescription, const sf::Color &color,
            const sf::Color &prev_color, App &app);
        ~ClearCanvas();
    private:
        App& m_app;
        sf::Color m_color;
        sf::Color m_prev_color;
        std::shared_ptr<sf::Image> m_prev_img;
        bool execute();
        bool undo();
        bool compare(const std::shared_ptr<Command> &rhs);

};


#endif
