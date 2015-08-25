#include "ReadGmsh.hpp"

namespace Mesh
{
	void ReadGmsh(std::istream& msh, std::vector<node>& nodes, std::vector<element>& elements, std::vector<group>& groups) {
		std::string line;
		msh >> line;
		if (line.compare("$MeshFormat") == 0) {
			msh >> line;
			if (line.compare("2.2") != 0) throw std::exception("Mesh version unsupported");
			msh >> line;
			if (line[0] != '0') throw std::exception("Mesh not in ASCII Format");
			msh >> line;
			if (line[0] != '8') throw std::exception("Unexpected floating point size");
			msh >> line;
			if (line.compare("$EndMeshFormat") != 0) throw std::exception(("Expected $EndMeshFormat identifier got " + line).c_str());
		} else throw std::exception("Mesh format identifier not found");
		try {
			for (;;) {
				line = NextIdentifier(msh);
				if (line.compare("$PhysicalNames") == 0) {
					int no;
					msh >> no;
					groups.resize(no);
					for (int i = 0; i < no; ++i) {
						//msh >> line;
						//if (line == "$EndPhysicalNames" && i < no) throw std::exception("Unexpected end of PhysicalNames section");
						msh >> groups[i].Dim;
						msh >> groups[i].GmshNo;
						msh >> groups[i].Name;
						groups[i].Name = groups[i].Name.substr(1, groups[i].Name.length() - 2);
					}
				}
				if (line.compare("$Nodes") == 0) {
					int no;
					msh >> no;
					nodes.resize(no);
					double tmp;
					for (int i = 0; i < no; ++i) {
						//msh >> line;
						//if (line.compare("$EndNodes") != 0) throw std::exception("Unexpected end of Nodes section");
						msh >> nodes[i].MshNo;
						msh >> tmp;
						nodes[i].Coord(0) = tmp;
						msh >> tmp;
						nodes[i].Coord(1) = tmp;
						msh >> tmp;
					}
				}
				if (line.compare("$Elements") == 0) {
					int no;
					msh >> no;
					elements.resize(no);
					for (int i = 0; i < no; ++i) {
						//msh >> line;
						//if (line.compare("$EndElements") != 0) throw std::exception("Unexpected end of Elements section");
						msh >> elements[i].No;
						int temp, node;
						msh >> temp;
						elements[i].Type = static_cast<ELEMENT_TYPE>(temp);
						msh >> temp;
						if (temp < 1) throw std::exception("Element physical group not found.");
						msh >> elements[i].Grp;
						for (int ii = 1; ii < temp; ++ii ) {
							msh >> node;
						}
						elements[i].Grp = SearchGroup(groups, elements[i].Grp);
						switch (elements[i].Type) {
						case ELEMENT_TYPE::LINE:
							for (int ii = 0; ii < 2; ++ii) {
								msh >> node;
								elements[i].Nodes.push_back(SearchNode(nodes, node));
							}
							break;
						case ELEMENT_TYPE::TRIANGLE:
							for (int ii = 0; ii < 3; ++ii) {
								msh >> node;
								elements[i].Nodes.push_back(SearchNode(nodes, node));
							}
							break;
						}
						elements[i].Nodes.shrink_to_fit();
					}
				}
				if (msh.eof()) break;
			}
		}
		catch (std::istream::failure e) {
			if (!msh.eof()) throw e;
		}
	}

	std::string NextIdentifier(std::istream& msh) {
		std::string line;
		for (;;) {
			msh >> line;
			if (line[0] == '$') return line;
			if (msh.eof()) return std::string("");
		}
	}

	int SearchNode(std::vector<node>& nodes, int no) {
		for (int i = 0; i < nodes.size(); ++i) {
			if (nodes[i].MshNo == no) {
				return i;
			}
		}
		throw std::exception(("Couldn't find node: " + std::to_string(no)).c_str());
	}

	int SearchGroup(std::vector<group>& groups, int no) {
		for (int i = 0; i < groups.size(); ++i) {
			if (groups[i].GmshNo == no) {
				return i;
			}
		}
		throw std::exception(("Couldn't find group: " + std::to_string(no)).c_str());
	}

	int SearchGroup(std::vector<group>& groups, std::string name) {
		for (int i = 0; i < groups.size(); ++i) {
			if (groups[i].Name == name) {
				return i;
			}
		}
		throw std::exception(("Couldn't find group: " + name).c_str());
	}
};