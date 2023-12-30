#include "pch.h"
#include "app.h"

static App *g_current_app = nullptr;
namespace gdi = Gdiplus;

void App::wnd_proc( Window *wnd, gart::EventType event_type, const gart::Event * event) {
	switch (event_type)
	{
	case gart::EventType::Paint:
		{
			gdi::GdiplusStartupInput gdiplusStartupInput;
			ULONG_PTR gdiplusToken;
			GdiplusStartup( &gdiplusToken, &gdiplusStartupInput, NULL );

			g_current_app->draw();

			gdi::GdiplusShutdown( gdiplusToken );
		}
		break;
	default:
		break;
	}
}

App::App() : m_window{ nullptr } {
	g_current_app = this;

	m_window = reinterpret_cast<Window *>(m_mem_window.data());
	(void)new(m_window) Window( DefaultTitle, App::wnd_proc );
}

void App::mainloop() {
	while (m_window)
	{
		m_window->poll();

		std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
	}
}

void App::draw() const {

	gdi::Rect rct = m_window->rect();
	rct.X -= 1;
	rct.Width += 2;

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint( m_window->get_hwnd(), &ps );
	gdi::Graphics graphics{ hdc };

	graphics.SetSmoothingMode( gdi::SmoothingModeHighQuality );
	graphics.SetInterpolationMode( gdi::InterpolationModeHighQuality );

	gdi::Pen pen{ gdi::Color( 18, 18, 18 ), 4.f };
	graphics.FillRectangle( pen.GetBrush(), rct );

	EndPaint( m_window->get_hwnd(), &ps );
}
