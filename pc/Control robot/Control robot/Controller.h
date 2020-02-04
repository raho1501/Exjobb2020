#include <windows.h>
#include <Xinput.h>

#pragma once
#pragma comment(lib, "XInput.lib")


class Controller
{
public:
	explicit Controller(int index);
	XINPUT_STATE get_controller_status();
	bool is_connected();
	~Controller();
private:
	XINPUT_STATE state;
	int controller_index;
};

