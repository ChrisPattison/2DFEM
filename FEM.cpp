#include "FEM.hpp"

namespace FEM
{
	PoissonSolver::PoissonSolver(std::istream& infile) {
		char mshpath[256];
		infile.getline(mshpath, 256);
		std::fstream mesh(mshpath);
		Mesh::ReadGmsh(mesh, this->Nodes, this->Elements, this->Groups);

		try {
			infile >> this->Resid;
			for (;;) { // TODO: make more flexible
				std::string name;
				char type;
				infile >> name;
				if (name == "") break;
				Mesh::group& g = this->Groups[Mesh::SearchGroup(this->Groups, name)];
				infile >> type;
				g.BoundType = static_cast<Mesh::BOUNDARY_TYPE>(type);
				infile >> g.Value;
			}
		}
		catch (std::istream::failure e) {
			if (!infile.eof()) throw e;
		}

		this->T = Eigen::VectorXd(this->Nodes.size());
		this->b = Eigen::VectorXd(this->Nodes.size());
		this->A = Eigen::SparseMatrix<double>(this->Nodes.size(), this->Nodes.size());
		this->T.fill(300);
		this->b.fill(0);
		for (int i = 0; i < this->Elements.size(); ++i) {
			Mesh::element& e = Elements[i];

			switch (e.Type) { // Fill A Matrix
				case(Mesh::ELEMENT_TYPE::TRIANGLE) : {

					Eigen::Matrix<double, 3, 3> E;
					E << this->Nodes[e.Nodes[0]].Coord, 1,
						this->Nodes[e.Nodes[1]].Coord, 1,
						this->Nodes[e.Nodes[2]].Coord, 1;

					double Area = abs(E.determinant())/2;

					Eigen::Matrix<double, 2, 3> B =
						(Eigen::Matrix2d() <<
							E(2, 1) - E(0, 1), E(0, 1) - E(1, 1),
							E(0, 0) - E(2, 0), E(1, 0) - E(0, 0)).finished()
						/
						Area*2
						*
						(Eigen::Matrix<double, 2, 3>() <<
							-1, 1, 0,
							-1, 0, 1).finished();
					Eigen::Matrix3d localA = B.transpose()*B*Area;

					for (int r = 0; r < 3; ++r) {
						for (int c = 0; c < 3; ++c) {
							this->A.coeffRef(e.Nodes[r], e.Nodes[c]) += localA(r, c);
						}
					}
				}
				break;
			}
		}
		for (int i = 0; i < this->Elements.size(); ++i) { // Set BCs
			if (this->Elements[i].Type == Mesh::ELEMENT_TYPE::LINE) {
				for (int ii = 0; ii < this->Elements[i].Nodes.size(); ++ii) {
					int node = this->Elements[i].Nodes[ii];
					this->b(node) = this->Groups[this->Elements[i].Grp].Value;
					//this->A.row(node) = 0;
					this->A.prune([node](int row, int col, double value) -> bool {return row != node; }); // TODO: Replace hack
					this->A.coeffRef(node, node) = 1;
					//TODO: do something about multiple BCs on a node
				}
			}
		}
	}

	double PoissonSolver::Solve() {
		/*Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>> Dsolver;
		Dsolver.compute(this->A);
		double det = Dsolver.determinant();
		std::cout << "Det: " << det << "\n";
		if (det == 0) {
			throw std::exception("Matrix not invertable");
		}
		this->T = Dsolver.solve(this->b);
		return 0.0;*/
		Eigen::BiCGSTAB<Eigen::SparseMatrix<double>> solver;
		solver.compute(this->A);
		solver.setTolerance(this->Resid);
		this->T = solver.solve(this->b);
		int iter = solver.iterations();
		return solver.error();
	}
};