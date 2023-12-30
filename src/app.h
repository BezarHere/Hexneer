#pragma once
#include "gh.h"


class App
{
public:
	App();

	void mainloop();

private:
	static void wnd_proc( Window *wnd, gart::EventType event_type, const gart::Event *event );
	void draw() const;
	

private:
	std::array<u8, sizeof( Window )> m_mem_window = { 0 };
	Window *m_window;
};
