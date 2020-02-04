#include "Controller.h"

Controller::Controller(int index):controller_index(index)
{}

XINPUT_STATE Controller::get_controller_status()
{
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	XInputGetState(controller_index, &state);
	return state;
}

bool Controller::is_connected()
{
    ZeroMemory(&state, sizeof(XINPUT_STATE));
    DWORD Result = XInputGetState(controller_index, &state);
    if (Result == ERROR_SUCCESS)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Controller::~Controller()
{
    ZeroMemory(&state, sizeof(XINPUT_STATE));
}
