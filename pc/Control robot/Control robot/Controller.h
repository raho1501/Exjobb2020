#include <windows.h>
#include <Xinput.h>

#pragma once
#pragma comment(lib, "XInput.lib")

//!  The controller class 
/*!
  It uses the windows.h and xinput.h to read user input from the controller.
*/
class Controller
{
public:
	//! The constructor.
	/*! It takes the index of the controller as arg. The index corresponds to the light on the controller logo. 
	* However since the lights on the controller are in the 1-4 range, 
	* it need to be converted to a 0-3 range since that what the library use 
	     \sa Controller()
		\param index the controller index.
	*/
	explicit Controller(int index);
	//! Getter for controller status
	/*! returns XINPUT_STATE of controller
	*/
	XINPUT_STATE get_controller_status();
	//! Check if controller is connected
	/*! function to check if controller is connected.
	*/
	bool is_connected();
	//! Destructor
	~Controller();
private:
	//! XINPUT_STATE variable
	XINPUT_STATE state;
	//! int variable for controller index
	int controller_index;
};

