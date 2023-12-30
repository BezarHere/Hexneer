#pragma once
#include "gh.h"
#include "edited_file.h"


class App
{
public:
	App();

	void mainloop();

private:
	static void wnd_proc( Window *wnd, gart::EventType event_type, const gart::Event *event );
	void draw() const;

private:
	struct GDIPlusState;

	stackptr<Window> m_window;
	EditedFile m_file;
	mutable GDIPlusState *m_gdi_state;
};
