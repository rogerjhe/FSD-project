/**
 *  @file   Draw.cpp
 *  @brief  Draw implementation, all drawing actions are commands.
 *  @author Mike and Carter Ithier
 *  @date   2020-09-10
 ***********************************************/

// Include our Third-Party SFML header
#include <SFML/Graphics/Color.hpp>
// Include standard library C++ libraries.
// #include ...
// Project header files
#include "App.hpp"
#include "Draw.hpp"
#include <iostream>

/*! \brief Draw constructor which initializes all members
    \param m_commandDescription string of command description "draw" for draw
    \param coord sf::Vector2i location of the pixel being changed
    \param color color the pixel is being changed to
    \param background color of the background
    \param app reference to app object holding the image and actions
*/
Draw::Draw(const std::string &m_commandDescription,
           sf::Vector2i coord, const sf::Color &color, const sf::Color &background, App &app):
            Command(m_commandDescription), m_coords(coord), m_color(color),
            m_background(background), m_app(app){
}

/*! \brief Draw destructor
*/
Draw::~Draw(){
}

/*! \brief Compare two draw objects and check for equality.
    \return boolean of if the two objects are the same
*/
bool Draw::compare(const std::shared_ptr<Command> &c_rhs){
    const auto rhs = std::dynamic_pointer_cast<Draw>(c_rhs);

    if (rhs){
        return (m_commandDescription.compare(rhs->m_commandDescription) == 0
            && m_coords.x == rhs->m_coords.x
            && m_coords.y == rhs->m_coords.y
            && m_color == rhs->m_color
            );
    } else{
        return false;
    }

}

/*! \brief Check if the pixel point in the Draw command is within the bounds of the window.
    \return boolean of if coordinate is in bounds of image window
*/
bool Draw::InBounds(){
    return (m_coords.x >= 0 && m_coords.x < m_app.GetWindowWidth() && m_coords.y >= 0 && m_coords.y < m_app.GetWindowHeight());
}


/*! \brief 	Execute the draw command by drawing a pixel.
    \return boolean of if draw was valid.
*/
bool Draw::execute(){
    if (InBounds()){
        // std::cout << "Drawing at (" << m_coords.x << ", " << m_coords.y << ")" << std::endl;
        m_app.GetImage().setPixel(m_coords.x, m_coords.y, m_color);
        return true;
    }
    else{
        return false;
    }

}

/*! \brief 	Undo this current draw command by making the pixel white again.
*
*/
bool Draw::undo(){
	if (InBounds()){
        m_app.GetImage().setPixel(m_coords.x, m_coords.y, m_background);
        return true;
    }
    else{
        return false;
    }
}
