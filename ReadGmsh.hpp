#include <vector>
#include <istream>
#include <exception>
#include <string>
#include <algorithm>
#include "MshTypes.hpp"

#pragma once

namespace Mesh
{
	void ReadGmsh(std::istream& msh, std::vector<node>& nodes, std::vector<element>& elements);
	std::string NextIdentifier(std::istream& msh);
	int SearchNode(std::vector<node>& nodes, int no);
};