#include <fstream>
#include <iostream>
#include <exception>
#include <vector>
#include "ReadGmsh.hpp"
#include "Eigen/Sparse"
#include "Eigen/Dense"

#pragma once

namespace FEM
{
	class PoissonSolver
	{
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		PoissonSolver(std::istream& infile);
		double Solve();
		Eigen::VectorXd T;
		std::vector<Mesh::node> Nodes;
	private:
		std::vector<Mesh::group> Groups;
		std::vector<Mesh::element> Elements;
		Eigen::VectorXd b;
		Eigen::SparseMatrix<double> A;
		double Resid;
	};
};