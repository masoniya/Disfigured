#include <iostream>

#include "Engine.h"


int main(int argc, char** argv) {
	Engine engine;

	try {
		engine.start();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		getchar();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
