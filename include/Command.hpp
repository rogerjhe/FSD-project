
/**
 *  @file   Command.hpp
 *  @brief  Represents an actionable command by the user.
 *  @author Mike and Carter Ithier
 *  @date   2020-09-10
 ***********************************************/
#ifndef COMMAND_HPP
#define COMMAND_HPP

// Include our Third-Party SFML header
// #include ...
// Include standard library C++ libraries.
#include <string>
#include <memory>
// Project header files
// #include ...

// The command class
class Command{
protected:
	std::string m_commandDescription;
public:
    /*! \brief Command constructor that initializes command description.
    */
	Command(const std::string &commandDescription);
	// Destructor for a command
	/*! \brief Virtual Command destructor.
	*/
	virtual ~Command();

	// Returns true or false if the command was able to succssfully
	// execute.
	// If it executes successfully, then that command is added to the
	// undo stack.
	// Each parameters also takes in a string name. While this may not be optimal,
	// it allows us to easily debug what each command is doing in a textual form.
	/*! \brief Purely virtual function for executing command.
	*/
	virtual bool execute() = 0;
	/*! \brief Purely virtual function for undoing command.
	*/
	virtual bool undo() = 0;
	/*! \brief Purely virtual function for comparing two commands.
	*/
	virtual bool compare(const std::shared_ptr<Command> &c_rhs) = 0;
};



#endif
