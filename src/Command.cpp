/**
 *  @file   Command.cpp
 *  @brief  Implementation of Command.hpp
 *  @author Mike and ????
 *  @date   yyyy-dd-mm
 ***********************************************/

// Include our Third-Party SFML header
// #include ...
// Include standard library C++ libraries.
#include <string>
// Project header files
#include "Command.hpp"

/*! \brief 	N/A
*
*/
Command::Command(const std::string &commandDescription) : m_commandDescription(commandDescription) {
}

/*! \brief 	N/A
*
*/
Command::~Command(){
}
