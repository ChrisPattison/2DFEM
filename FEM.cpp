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
		this->q = Eigen::VectorXd(this->Nodes.size());
		this->A = Eigen::SparseMatrix<double>(this->Nodes.size(), this->Nodes.size());
		this->T.fill(300);
		this->q.fill(0);
		for (int i = 0; i < this->Elements.size(); ++i) {
			Mesh::element& e = Elements[i];

			switch (e.Type) {

				case(Mesh::ELEMENT_TYPE::TRIANGLE) : {
					Eigen::Matrix<double, 3, 2> E;
					E.row(0) = this->Nodes[e.Nodes[0]].Coord;
					E.row(1) = this->Nodes[e.Nodes[1]].Coord;
					E.row(2) = this->Nodes[e.Nodes[2]].Coord;

					Eigen::Matrix<double, 2, 3> B =
						(Eigen::Matrix2d() <<
							E(2, 1) - E(0, 1), E(0, 1) - E(1, 1),
							E(0, 0) - E(2, 0), E(1, 0) - E(0, 0)).finished()
						*
						(Eigen::Matrix<double, 2, 3>() <<
							-1, 1, 0,
							-1, 0, 1).finished();
					Eigen::Matrix3d localA = B.transpose()*B;

					for (int r = 0; r < 3; ++r) {
						for (int c = 0; c < 3; ++c) {
							this->A.coeffRef(e.Nodes[r], e.Nodes[c]) += localA(r, c);
						}
					}
				}
				break;

				case(Mesh::ELEMENT_TYPE::LINE) : {
					for (int n : e.Nodes) {
						this->q(n) = -this->Groups[e.Grp].Value;
					}
				}
				break;
			}
		}
		for (int i = 0; i < this->Elements.size(); ++i) {
			if (this->Elements[i].Type == Mesh::ELEMENT_TYPE::LINE) {
				for (int ii = 0; i < this->Elements[i].Nodes.size(); ++i) {
					this->q(this->Elements[i].Nodes[ii]) -= this->Groups[this->Elements[i].Grp].Value;
					//TODO: do something about multiple BCs on a node
				}
			}
		}
	}

	double PoissonSolver::Solve() {
		Eigen::BiCGSTAB<Eigen::SparseMatrix<double>> solver;
		solver.compute(this->A);
		solver.setTolerance(this->Resid);
		this->T = solver.solve(this->q);
		int iter = solver.iterations();
		return solver.error();
	}
};