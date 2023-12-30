#include "pch.h"
#include "console.h"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
int main( int argc, const char *const argv[] ) {
	create_console();
	while (true) std::cout << "hello!\n";
}
