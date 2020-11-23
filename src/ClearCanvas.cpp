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
#include "ClearCanvas.hpp"
#include <iostream>

/*! \brief ClearCommand constructor which initializes all members
    \param m_commandDescription string of action should be "clear"
    \param curr_color color of that the canvas should be wiped to
    \param app reference to object that stores the image and actions
*/
ClearCanvas::ClearCanvas(const std::string &m_commandDescription, const sf::Color &curr_color,
    const sf::Color &prev_color, App &app):
 Command(m_commandDescription), m_color(curr_color), m_app(app),
 m_prev_color(prev_color), m_prev_img(std::make_shared<sf::Image>()){
    m_prev_img->create(m_app.GetWindowWidth(), m_app.GetWindowHeight());
    m_prev_img->copy(m_app.GetImage(), 0, 0);
}

/*! \brief ClearCanvas destructor
*/
ClearCanvas::~ClearCanvas(){
}

/*! \brief Compare two command objects and check for equality.
    \param c_rhs command object to be compared to
    \return boolean of if the object is the same as this one
*/
bool ClearCanvas::compare(const std::shared_ptr<Command> &c_rhs){
    const auto rhs = std::dynamic_pointer_cast<ClearCanvas>(c_rhs);

    if (rhs){
        for (unsigned int i = 0; i < m_app.GetWindowHeight(); i++){
            for (unsigned int j = 0; j < m_app.GetWindowWidth(); j++){
                if (m_app.GetImage().getPixel(i, j) != rhs->m_prev_img->getPixel(i, j))
                    std::cout << "original images not equal" << std::endl;
                    return false;
            }
        }

        return (m_commandDescription.compare(rhs->m_commandDescription) == 0
            && m_color == rhs->m_color
            );
    } else{
        return false;
    }

}


/*! \brief 	Execute the clear command by replacing every pixel with the new color.
    \return boolean of if execute was completed
*
*/
bool ClearCanvas::execute(){
    m_app.GetImage().create(m_app.GetWindowWidth(), m_app.GetWindowHeight(), m_color);
    m_app.SetBackgroundColor(m_color);
    return true;

}

/*! \brief 	Undo this current clear command by making every pixel the original pixel.
    \return boolean of if undo was completed
*/
bool ClearCanvas::undo(){
    m_app.GetImage().copy(*m_prev_img, 0, 0);
    m_app.SetBackgroundColor(m_prev_color);
    return true;

}


