#include <vector>
#include <istream>
#include <fstream>
#include <exception>
#include <string>
#include <algorithm>
#include "MshTypes.hpp"

#pragma once

namespace Mesh
{
	void ReadGmsh(std::istream& msh, std::vector<node>& nodes, std::vector<element>& elements, std::vector<group>& groups);
	void WriteData(std::fstream& msh, std::fstream& output, std::vector<node>& nodes, Eigen::VectorXd T);
	std::string NextIdentifier(std::istream& msh);
	int SearchNode(std::vector<node>& nodes, int no);
	int SearchGroup(std::vector<group>& groups, int no);
	int SearchGroup(std::vector<group>& groups, std::string name);
};