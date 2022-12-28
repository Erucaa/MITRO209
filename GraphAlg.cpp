#include<iostream>
#include<vector>
#include<set>
#include<list>
#include<algorithm>
#include<ctime>
#include<sstream>
#include<fstream>

using namespace std;

class Graph {

private:
	uint32_t nbNodes;
	uint32_t nbEdges=-1;
	vector<list<int>> ListNodes;

	vector<list<int>::iterator> Pointers;// element - pointer for element in DegreeNodes in list
	vector<int> PointersDegreeTableau; //elements - degree de i-éme sommet

	vector<list<int>> DegreeNodes;// tableau de δ degree de sommets;dans chaque element de vecteur on a la liste de sommets, qui ont le meme degre δ
	vector<vector<list<int>::iterator>>ListPointersNode;// tableau de sommets; dans chaque elem on a le pointeur sur le place de celle voisoin dans le ListNodes
public:

	Graph() { nbNodes = 0;}
	Graph(uint32_t nbV) 
	{
		nbNodes = nbV;
		ListNodes.reserve(nbV);

		DegreeNodes.reserve(nbV);
		ListPointersNode.reserve(nbV);

		Pointers.reserve(nbNodes);
		PointersDegreeTableau.reserve(nbNodes);

	}

	double Density() const { return nbEdges / nbNodes; }

	void AddNode(int u, int v);

	void RemplirDegree();

	void UpdateDeegreeNode(int v, int degree);

	vector<int> GetMinDegreeNode() const;

	uint32_t GetNbNodes() { return nbNodes; }
};


void Graph::AddNode(int u, int v)
{
	ListNodes[u].push_back(v);

}

void Graph::RemplirDegree()
{
	for (int i = 0; i < nbNodes; i++)
	{
		DegreeNodes[ListNodes[i].size()].push_back(i);//tableau de degree; on posse i-éme sommet dans son degree list 

		PointersDegreeTableau[i] = ListNodes[i].size();// таблица с  степенью каждой вершины 

		Pointers[i] = (DegreeNodes[PointersDegreeTableau[i]]).end();// вектор указывающий на элемент в таблице со степенями, прмя уже в liste

	}

}

void Graph::UpdateDeegreeNode(int v, int degree)
{
	vector<int> voisins(ListNodes[v].begin(),ListNodes[v].end());// list de sommet  les voisins de v

	if (PointersDegreeTableau[v] != 1)
	{
		for (auto& x : voisins)
		{
			DegreeNodes[PointersDegreeTableau[x]].erase(Pointers[x]);//on supprime le x comme voisin de v dans DegreeNode[δ(v)]

			DegreeNodes[PointersDegreeTableau[x] - 1].push_back(x);//on pose x dans DegreeNodes[δ(v))-1]

			Pointers[x] = DegreeNodes[PointersDegreeTableau[x] - 1].end();// on rénove  le pointeur sur x 

			PointersDegreeTableau[x]--;//diminuer le degree de x, pq on supprimer 

			ListNodes[x].erase(ListPointersNode[x][v]);//suprime le v du list de voisins de x

			// Возможно нужен еще один контейнер, который для каждой вершину v содержит лист указателей на их соседей в листе ListNodes? 
			// либо как блять их удалить
			// le cas quand on a δ(v) == 1 => soit nbNodes == 1 ; soit nbNodes == 2
		}
	}
	
	nbNodes = nbNodes - 1;
	nbEdges = nbEdges - voisins.size();
	//supprimer v de toute les lists

	ListNodes[v].clear();
	DegreeNodes[degree].erase(Pointers[v]);//suprimer v de la liste de voisins+degree
	PointersDegreeTableau[v] = 0;
	

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
	return result;// в принципе невозможно
}


int main()
{
	clock_t curTime;
	curTime = clock();


	//тут читаем граф из csv файла за время E + V
	int n;
	//cin >> n;
	n = 7624;
	int m=27860;//число ребе
	Graph G = Graph(n);

	ifstream fin("lastfm_asia_edges.csv");

	char delimiter = ',';
	string line,node1, node2;
	getline(fin, line);
	
	for (int i = 0; i < m; i++)
	{
		getline(fin, line);
		stringstream ss(line);
		getline(ss, node1, delimiter);
		getline(ss, node2, delimiter);
		G.AddNode(stoi(node1), stoi(node2));
	}

	Graph H = G;
	// тут из прочитанного графа составляем лист по степеням вершин, это тоже вроде время E + V , те общее время пока что 2|G| + const
	
	G.RemplirDegree();

	vector<int> v(2);

	while (G.GetNbNodes() > 0)
	{
		v = G.GetMinDegreeNode();
		G.UpdateDeegreeNode(v[0],v[1]);//здесь тоже вопросик. потому что мы храним и список со степенями и надо попутно знать что удалять.
		if (G.Density() > H.Density())
			H = G;// присваивание должно скорее всего за |G| выполнятся , может 2|G|, но думаю не больше. 
	}



	curTime = clock() - curTime;
	cout << "Input data : nodes = " << n << "  ; edges = " << m << endl;;
	cout << "running time: " << curTime/CLOCKS_PER_SEC << endl;

	return 0;//возвращаем Η

}

