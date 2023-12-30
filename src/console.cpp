#include "pch.h"
#include "console.h"
#include <fcntl.h>
#include <io.h>

constexpr WORD MaxConsoleLines = 255;

static bool bind_buffer(const DWORD handle, FILE *nf, FILE *of, bool inbuf = false)
{
	if (GetStdHandle( handle ) != INVALID_HANDLE_VALUE)
	{
		if (freopen_s( &nf, inbuf ? "CONIN$" : "CONOUT$" , inbuf ? "r" : "w", of) == 0)
		{
			setvbuf( of, NULL, _IONBF, 0 );
			return true;
		}
	}
	return false;
}

void create_console2()
{
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;

	BOOL console = AllocConsole();

	GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &coninfo );
	coninfo.dwSize.Y = MaxConsoleLines;
	SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), coninfo.dwSize );

	// redirect stdout
	lStdHandle = (long)GetStdHandle( STD_OUTPUT_HANDLE );
	hConHandle = _open_osfhandle( lStdHandle, _O_TEXT );
	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

	// redirect stdin
	lStdHandle = (long)GetStdHandle( STD_INPUT_HANDLE );
	hConHandle = _open_osfhandle( lStdHandle, _O_TEXT );
	fp = _fdopen( hConHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );

	// redirect stderr
	lStdHandle = (long)GetStdHandle( STD_ERROR_HANDLE );
	hConHandle = _open_osfhandle( lStdHandle, _O_TEXT );
	fp = _fdopen( hConHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );

	std::ios::sync_with_stdio();
}

void create_console()
{
	FILE *fp = nullptr;

	BOOL console = AllocConsole();

	bind_buffer( STD_INPUT_HANDLE, fp, stdin, true);
	bind_buffer( STD_OUTPUT_HANDLE, fp, stdout);
	bind_buffer( STD_ERROR_HANDLE, fp, stderr);

	std::ios::sync_with_stdio();

	std::wcout.clear();
	std::cout.clear();
	std::wcerr.clear();
	std::cerr.clear();
	std::wcin.clear();
	std::cin.clear();
}
