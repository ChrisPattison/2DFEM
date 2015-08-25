#include <vector>
#include "Eigen/Dense"

#pragma once

namespace Mesh
{
	struct node
	{
		Eigen::Vector2d Coord;
		int MshNo;
	};
	
	enum ELEMENT_TYPE
	{
		LINE = 1,
		TRIANGLE = 2
	};

	enum BOUNDARY_TYPE
	{
		DIRICHLET = 'd',
		NEUMANN = 'n'
	};

	struct element
	{
		std::vector<int> Nodes;
		int No;
		int Grp;
		ELEMENT_TYPE Type;
	};

	struct group
	{
		int Dim;
		int GmshNo;
		std::string Name;
		BOUNDARY_TYPE BoundType;
		double Value;
	};
};