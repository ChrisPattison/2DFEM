#include "FEM.hpp"
#include <exception>
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
	try {
		if (argc < 3) {
			throw std::exception("Case path or result path not found.");
		}
		std::fstream casefile(argv[1], std::ios_base::in);
		std::cout << "Setting up solver\n";
		FEM::PoissonSolver solver(casefile);
		std::cout << "Solving...\n";
		double resid = solver.Solve();
		std::cout << "Done with Residual: " << resid << "\n";
		std::cout << solver.T;
		casefile.clear();
		casefile.seekg(0);
		std::string mshfile;
		std::getline(casefile, mshfile);//TODO: read mesh file in one place
		std::fstream msh = std::fstream(mshfile, std::ios_base::in);
		std::fstream output = std::fstream(argv[2], std::ios_base::out);
		Mesh::WriteData(msh, output, solver.Nodes, solver.T);
}
	catch (std::exception e) {
		std::cout << "Exception: " << e.what();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}