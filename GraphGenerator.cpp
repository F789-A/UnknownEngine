#include "GraphGenerator.h"

Graph::Graph(unsigned int seed, int len, int singleCount, int dualCount, int tripleCount, int quadrupleCount) :
	Length(len),
	Verts(singleCount + dualCount + tripleCount + quadrupleCount),
	VertToLevel(singleCount + dualCount + tripleCount + quadrupleCount, -1),
	LevelsToVert(len),
	debugPos(singleCount + dualCount + tripleCount + quadrupleCount),
	seed_w({seed}),
	random_generator(seed_w)
{
	GenerateGraph(singleCount, dualCount, tripleCount, quadrupleCount);
}

void Graph::AddEdge(int i, int j)
{
	Verts[i].push_back(j);
	Verts[j].push_back(i);

	if (VertToLevel[i] == -1)
	{
		std::swap(i, j);
	}

	if (VertToLevel[j] == -1)
	{
		VertToLevel[j] = VertToLevel[i] + 1;
	}

	if (((int)Verts[i].size() - 2) >= 0)
	{
		auto& prev = LevelsToVert[VertToLevel[i]][Verts[i].size() - 2];
		auto it = std::find(prev.begin(), prev.end(), i);
		prev.erase(it);
	}
	LevelsToVert[VertToLevel[i]][Verts[i].size() - 1].push_back(i);

	if (((int)Verts[j].size() - 2) >= 0)
	{
		auto& prev = LevelsToVert[VertToLevel[j]][Verts[j].size() - 2];
		auto it = std::find(prev.begin(), prev.end(), j);
		if (it != prev.end())
		{
			prev.erase(it);
		}
	}
	LevelsToVert[VertToLevel[j]][Verts[j].size() - 1].push_back(j);
}

int Graph::GetRandomFromLevel(int level, bool CanUse1, bool CanUse2, bool CanUse3, int without)
{
	std::array<bool, 3> arr{ CanUse1, CanUse2, CanUse3 };

	std::vector<int> target;
	for (int i = 0; i < 3; i++) // optimize
	{
		if (arr[i])
		{
			target.insert(target.end(), LevelsToVert[level][i].begin(), LevelsToVert[level][i].end());
		}
	}
	auto it = std::find(target.begin(), target.end(), without);
	if (it != target.end())
	{
		target.erase(it);
	}
	if (target.size() == 0)
	{
		return -1;
	}

	std::uniform_int_distribution<> lvlDistrib(0, target.size() - 1);

	return target[lvlDistrib(random_generator)];
}

void Graph::GenerateGraph(int N1, int N2, int N3, int N4)
{
	int n1 = 0;
	int n2 = 0;
	int n3 = 0;
	int n4 = 0;

	VertToLevel[0] = 0;
	n1 += 1;
	debugPos[0] = std::pair<int, int>(0, 0);

	// строим минимальный путь
	AddEdge(0, 1);
	for (int i = 1; i < Length - 1; ++i)
	{
		AddEdge(i, i + 1);
		n2++;
		debugPos[i] = std::pair<int, int>(0, i);
	}
	n1 += 1;
	debugPos[Length-1] = std::pair<int, int>(0, Length-1);

	//строим ветки
	int k = 1;
	while (n1 + n2 + n3 + n4 != Verts.size())
	{
		int countRemainingVert = (int)Verts.size() - n1 - n2 - n3 - n4;

		bool canUse3 = n4 < N4;
		bool canUse2 = n3 < N3 || canUse3;
		bool canUse1 = (n2 < N2 || canUse2) && (countRemainingVert > (N1 - n1));

		std::uniform_int_distribution<> lvlDistrib(0, Length - 2);
		int level = lvlDistrib(random_generator);

		int vert = GetRandomFromLevel(level, canUse1, canUse2, canUse3);

		std::uniform_int_distribution<> brancDistrib(1, std::min(Length - level - 1, countRemainingVert));
		int branchLen = countRemainingVert <= (N1 - n1) ? 1 : brancDistrib(random_generator); //get random from [1, len-rank);

		if (Verts[vert].size() == 1)
		{
			n1--;
			n2++;
		}
		else if (Verts[vert].size() == 2)
		{
			n2--;
			n3++;
		}
		else if (Verts[vert].size() == 3)
		{
			n3--;
			n4++;
		}
		int last = n1 + n2 + n3 + n4;
		AddEdge(vert, last);
		debugPos[last] = std::pair<int, int>(k, VertToLevel[last]);

		for (int j = last; j < last + branchLen - 1; ++j)
		{
			AddEdge(j, j + 1);
			n2++;
			debugPos[j+1] = std::pair<int, int>(k, VertToLevel[j+1]);
		}
		n1++;

		k++;
	}

	//строим перемычки
	while (n1 != N1 || n2 != N2 || n3 != N3 || n4 != N4)
	{
		//1) заполняем 2 до >= из 1
		//2) расскидываем 1
		//3) заполняем 3 до >= из 2
		//4) расскидываем 2
		//5) заполняем 4 до = из 3

		bool canUse1Src = false;
		bool canUse2Src = false;
		bool canUse3Src = false;

		bool canUse1 = false;
		bool canUse2 = false;
		bool canUse3 = false;

		if (n2 < N2)
		{
			canUse1Src = true;
			canUse1 = true;
		}
		else if (n1 > N1)
		{
			canUse1Src = true;

			canUse3 = n4 < N4;
			canUse2 = (n3 < N3) || canUse3;
			canUse1 = ((n2 < N2) || canUse2) && ((n1-N1) > 1);
		}
		else if (n3 < N3)
		{
			canUse2Src = true;
			canUse2 = true;
		}
		else if (n2 > N2)
		{
			canUse2Src = true;

			canUse3 = n4 < N4;
			canUse2 = (n3 < N3) || canUse3;
		}
		else if (n4 < N4)
		{
			canUse3Src = true;

			canUse3 = true;
		}


		std::uniform_int_distribution<> lvlDistrib(1, Length - 2);

		int vert1Level = lvlDistrib(random_generator); //get random level
		int vert1 = -1;
		int vert2 = -1;
		int i = 0;
		while (vert1 == -1 || vert2 == -1)
		{
			vert1 = GetRandomFromLevel(vert1Level, canUse1Src, canUse2Src, canUse3Src);

			if (vert1 != -1)
			{
				std::uniform_int_distribution<> lvlDistrib2(vert1Level - 1, vert1Level + 1); //get random from [vert1Level - 1, vert1Level + 1]

				int vert2Lvl = lvlDistrib2(random_generator);

				int j = 0;
				while (vert2 == -1 && j != 3)
				{
					vert2 = GetRandomFromLevel(vert2Lvl, canUse1, canUse2, canUse3, vert1);
					int offset = vert1Level - 1;
					vert2Lvl = ((vert2Lvl - offset + 1) % 3) + offset;
					j++;
				}
			}
			int offset = 1;
			vert1Level = ((vert1Level - offset + 1) % (Length - 2)) + offset;

			i++;
			if (i == Length - 2)
			{
				throw "error";
			}
		}

		if (Verts[vert1].size() == 1)
		{
			n1--;
			n2++;
		}
		else if (Verts[vert1].size() == 2)
		{
			n2--;
			n3++;
		}
		else if (Verts[vert1].size() == 3)
		{
			n3--;
			n4++;
		}
		if (Verts[vert2].size() == 1)
		{
			n1--;
			n2++;
		}
		else if (Verts[vert2].size() == 2)
		{
			n2--;
			n3++;
		}
		else if (Verts[vert2].size() == 3)
		{
			n3--;
			n4++;
		}
		AddEdge(vert1, vert2);
	}
}