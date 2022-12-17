#include<iostream>
#include<vector>
#include<set>
#include<algorithm>

using namespace std;

class Graph {

private:
	uint32_t nbNodes;
	uint32_t nbEdges=-1;
	vector<set<int>> ListNodes;
	vector<set<int>> DegreeNodes;
	vector<vector<int*>>ListPointersNode;
public:

	Graph() { nbNodes = 0;}
	Graph(uint32_t nbV) { nbNodes = nbV; ListNodes.reserve(nbV); DegreeNodes.reserve(nbV); }

	double Density() const { return nbEdges / nbNodes; }

	void AddNode(int u, int v);

	void UpdateDegreeNodes1(int v, int degree);

	void UpdateDeegreeNode(int v, int degree);

	vector<int> GetMinDegreeNode() const;

	uint32_t GetNbNodes() { return nbNodes; }
};


void Graph::AddNode(int u, int v)
{
	ListNodes[u].insert(v);

}

void Graph::UpdateDegreeNodes1(int v, int degree)
{
	vector<int> neighbors (ListNodes[v].begin(),ListNodes[v].end());
	
	int nbNeigh = neighbors.size();
	for (int i = degree; i < nbNodes; i++)
	{
		bool cas = any_of(neighbors.begin(), neighbors.end(), [&](int& u) {return DegreeNodes[i].find(u) != DegreeNodes[i].end(); });
		if (cas)
		{

		}
	}

}

void Graph::UpdateDeegreeNode(int v, int degree)
{
	vector<int*> pointers = ListPointersNode[v];

	int i = 0;
	for ( auto& x : pointers)
	{
		if (x != nullptr)
		{
			DegreeNodes[*x].erase(DegreeNodes[*x].find(i));// возможно здесь find тоже за O(n) тогда все плохо
			DegreeNodes[(*x) - 1].insert(i);
			x--;
		}

	}
	 
	nbNodes = nbNodes - 1;
	nbEdges = nbEdges - pointers.size();
}

vector<int> Graph::GetMinDegreeNode()const  // какая сложность относительно |G| ?
{
	vector<int> result(2);
	if (nbNodes == 0)
		return result;
	int i = 0;
	while (true)
	{
		if (!DegreeNodes[i].empty())
		{
			result[0]=*(DegreeNodes[i].begin());
			result[1] = i;
			return result;
		}
		i++;
	}
	return result;
}


int main()
{


	//тут читаем граф из csv файла за время E + V
	int n;
	cin >> n;
	Graph G = Graph(n);
	Graph H = G;
	// тут из прочитанного графа составляем лист по степеням вершин, это тоже вроде время E + V , те общее время пока что 2|G| + const

	while (G.GetNbNodes() > 0)
	{
		vector<int> v = G.GetMinDegreeNode();
		G.UpdateDegreeNodes1(v[0],v[1]);//здесь тоже вопросик. потому что мы храним и список со степенями и надо попутно знать что удалять.
		if (G.Density() > H.Density())
			H = G;// присваивание должно скорее всего за |G| выполнятся , может 2|G|, но думаю не больше. 
	}

	//возвращаем Η

}

