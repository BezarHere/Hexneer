#include "pch.h"
#include "app.h"

static void proc( Window *wnd, gart::EventType even_type, const gart::Event * event) {
	
}

App::App()
	: m_window{ L"Hexneer", proc } {

}

void App::mainloop() {
	while (m_window)
	{
		m_window.poll();


		std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
	}
}
