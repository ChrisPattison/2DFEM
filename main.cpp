#include "FEM.hpp"
#include <exception>
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
	try {
		if (argc < 3) {
			throw std::exception("Arguments <Case Path> <Mesh Path> not found.");
		}
		std::fstream casefile(argv[1], std::ios_base::in),
			msh(argv[2], std::ios_base::in);
		FEM::PoissonSolver solver(casefile);
	}
	catch (std::exception e) {
		std::cout << e.what();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}