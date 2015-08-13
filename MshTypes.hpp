#include <vector>
#include "Eigen/Dense"

#pragma once

namespace Mesh
{
	struct node;
	
	enum ELEMENT_TYPE
	{
		LINE = 1,
		TRIANGLE = 2
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
		std::vector<int> Members;
		int GmshNo;
		std::string Name;
		double Value;
	};

	struct node
	{
		Eigen::Vector2d Coord;
		int MshNo;
		std::vector<int> Elements;
	};
};