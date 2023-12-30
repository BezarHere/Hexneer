#include "pch.h"
#include "console.h"
#include "app.h"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")


int main( int argc, const char *const argv[] ) {
	create_console();
	App app;

	app.mainloop();

}
