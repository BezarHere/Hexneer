#include "pch.h"
#include "app.h"

static App *g_current_app = nullptr;
namespace gdi = Gdiplus;
using gdi::REAL;

struct App::GDIPlusState
{
	HDC hdc;
	gdi::Font *main_font;
	gdi::Graphics *graphics;
	PAINTSTRUCT *paint;
	gdi::Rect window_recti;
	gdi::RectF window_rect;
};

static std::unique_ptr<gdi::FontFamily> query_fonts( std::initializer_list<filepath> fonts, const gdi::FontCollection *collection = nullptr ) {
	std::unique_ptr<gdi::FontFamily> p;

	for (const filepath &f : fonts)
	{
		p.reset( new gdi::FontFamily( f.c_str(), collection ) );
		if (p->GetLastStatus() == gdi::Ok)
			break;
	}

	return p;
}

void App::wnd_proc( Window *wnd, gart::EventType event_type, const gart::Event *event ) {
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

App::App() : m_window{ nullptr }, m_file{} {
	g_current_app = this;

	m_window.allocate( DefaultTitle, App::wnd_proc );
}

void App::mainloop() {
	while (m_window)
	{
		m_window->poll();

		std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
	}
}

void App::draw() const {
	//static gdi::FontFamily *font_family = nullptr;
	//static gdi::Font *font = nullptr;


	//if (!font_family)
	//{
	//	font_family = query_fonts( { L"Lucida Sans Typewriter", L"Lucida Sans", L"Arial" }, nullptr).release();
	//	
	//}


	gdi::FontFamily fm( TEXT( "Lucida Sans Typewriter" ) );
	gdi::Font font( &fm, 14 );

	gdi::Rect rct = m_window->rect();
	rct.X -= 1;
	rct.Width += 2;

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint( m_window->get_hwnd(), &ps );
	
	gdi::Graphics graphics{ hdc };
		
	graphics.SetSmoothingMode( gdi::SmoothingModeHighQuality );
	graphics.SetInterpolationMode( gdi::InterpolationModeHighQuality );

	GDIPlusState state;

	state.graphics = &graphics;
	state.hdc = hdc;
	state.main_font = &font;
	state.paint = &ps;
	state.window_recti = rct;
	state.window_rect = { (REAL)rct.X, (REAL)rct.Y, (REAL)rct.Width, (REAL)rct.Height };

	m_gdi_state = &state;

	//gdi::Pen pen{ gdi::Color( 18, 18, 18 ), 4.f };
	//graphics.FillRectangle( pen.GetBrush(), rct );
	//gdi::SolidBrush solid_brush = { gdi::Color( 255, 255, 255 ) };
	//	
	//graphics.DrawString( DefaultTitle, -1, &font, gdi::PointF( 0, 0 ), &solid_brush );
	

	m_gdi_state = nullptr;

	EndPaint( m_window->get_hwnd(), &ps );
}
