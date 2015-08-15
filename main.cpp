#include "FEM.hpp"
#include <exception>
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
	try {
		if (argc < 2) {
			throw std::exception("Case path argument not found.");
		}
		std::fstream casefile(argv[1], std::ios_base::in);
		FEM::PoissonSolver solver(casefile);
	}
	catch (std::exception e) {
		std::cout << e.what();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}