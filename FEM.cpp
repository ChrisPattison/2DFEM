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
		T.fill(300);
		q.fill(0);
		for (int i = 0; i < this->Elements.size(); ++i) {
			Mesh::element& e = Elements[i];

			switch (e.Type) {

				case(Mesh::ELEMENT_TYPE::TRIANGLE) : {
					Eigen::Matrix<double, 3, 2> E;
					E <<
						this->Nodes[e.Nodes[0]].Coord,
						this->Nodes[e.Nodes[1]].Coord,
						this->Nodes[e.Nodes[2]].Coord; // TODO: confirm behavior

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
							A.insert(e.Nodes[r], e.Nodes[c]) += localA(r, c);
						}
					}
				}
				break;

				case(Mesh::ELEMENT_TYPE::LINE) : {
					for (int n : e.Nodes) {
						q(n) = -this->Groups[e.Grp].Value;
					}
				}
				break;
			}
		}
	}
};