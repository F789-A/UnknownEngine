#pragma once
#include <vector>
#include <set>
#include <bitset>
#include <array>
#include <random>
#include <algorithm>

class Graph
{
	std::seed_seq seed_w;
	std::mt19937 random_generator;

	std::vector<int> VertToLevel;
	std::vector<std::array<std::vector<int>, 4>> LevelsToVert; //b00st::bimap
	int Length;
	//std::random_device rd;


public:
	std::vector<std::pair<int, int>> debugPos;
	std::vector<std::vector<int>> Verts;

	Graph(unsigned int seed, int len, int singleCount, int dualCount, int tripleCount, int quadrupleCount);

private:
	void AddEdge(int i, int j);

	int GetRandomFromLevel(int level, bool CanUse1, bool CanUse2, bool CanUse3, int without = -1);

	void GenerateGraph(int N1, int N2, int N3, int N4);
};