#include "FEM.hpp"

int main(char** argv, int argc) {
	
}

namespace FEM
{
	PoissonSolver::PoissonSolver(std::istream& infile, std::istream& mesh) {
		/*Mesh::ReadGmsh(mesh, this->Nodes, this->Elements);
		this->T = Eigen::VectorXd(this->Nodes.size());
		T.fill(300);
		for (int i = 0; i < this->Elements.size(); ++i) {
			Mesh::element& e = Elements[i];
			Eigen::Matrix<double, 3, 2> B;
			if (e.Type == Mesh::ELEMENT_TYPE::TRIANGLE) {
				Eigen::Matrix<double, 3, 2> E;
				E <<
					this->Nodes[e.Nodes[0]].Coord,
					this->Nodes[e.Nodes[1]].Coord,
					this->Nodes[e.Nodes[2]].Coord; // TODO: confirm behavior

				Eigen::Matrix<double, 2, 3> B =
					(Eigen::Matrix2d() <<
						E(3, 2) - E(1, 2), E(1, 2) - E(2, 2),
						E(1, 1) - E(3, 1), E(2, 1) - E(1, 1)).finished()
					*
					(Eigen::Matrix<double, 2, 3>() <<
						-1, 1, 0,
						-1, 0, 1).finished();
				Eigen::Matrix3d localA = B*B.transpose();
				
				for (int r = 1; r <= 3; ++r) {
					for (int c = 1; c <= 3; ++c) {
						A.insert(i*3 + r, e.Nodes[c - 1]) = localA(r, c);
					}
				}
			}
		}*/
	}
};