#include<iostream>
#include<vector>
#include<set>
#include<list>
#include<algorithm>
#include<ctime>
#include<chrono>
#include<sstream>
#include<fstream>

#include"CSVReader.h"//pour lire csv

using namespace std;

class Graph {

private:
	int32_t nbNodes;
	int32_t nbEdges=-1;
	int32_t nbOperations = 0;
	int nextDeletedeNodeDegree=0;

	vector<list<int>> ListNodes;
	vector<list<int>::iterator> Pointers;// element - pointer for element in DegreeNodes in list
	vector<int> PointersDegreeTableau; //elements - degree de i-éme sommet
	vector<list<int>> DegreeNodes;// tableau de δ degree de sommets;dans chaque element de vecteur on a la liste de sommets, qui ont le meme degre δ
	//vector<vector<list<int>::iterator>>ListPointersNode;// tableau de sommets; dans chaque elem on a le pointeur sur le place de celle voisoin dans le ListNodes

public:

	Graph() { nbNodes = 0;}
	Graph(int32_t nbV) 
	{
		
		nbNodes = nbV;
		nbEdges = 0;
		nextDeletedeNodeDegree = 0;
		ListNodes.resize(nbNodes+1, list<int> {});

		DegreeNodes.resize(nbNodes + 1, list<int>{});
		Pointers.resize(nbNodes + 1, list<int>::iterator{});
		PointersDegreeTableau.resize(nbNodes + 1, 0);
	}

	double Density() const { return double(nbEdges) / nbNodes; }

	void AddNode(int u, int v);

	void RemplirDegree();

	void UpdateDeegreeNode(int v, int degree);

	vector<int> GetMinDegreeNode() const;

	void SetNextDeletedNodeDegree(int32_t _degree) { nextDeletedeNodeDegree = max(_degree,0); }

	friend ostream& operator<<(ostream& out, Graph& graph)
	{
		out << "Nodes de graph: ";
		for (auto& degreeList : graph.DegreeNodes)
		{
			for (auto& node : degreeList)
			{
				out << node << " ";
			}
		}
		return out;
	}

	uint32_t GetNbNodes() { return nbNodes; }

	Graph& operator =(Graph& C1)
	{
		nbEdges = C1.nbEdges;
		nbNodes = C1.nbNodes;
		//ListNodes = C1.ListNodes;
		PointersDegreeTableau = C1.PointersDegreeTableau;
		return *this;
	}

	void WriteResult(string namefile,int isZero);
};


void Graph::AddNode(int u, int v)
{
	
	ListNodes[u].push_back(v);
	if(u!=v)
		ListNodes[v].push_back(u);
	nbEdges++;
}

void Graph::RemplirDegree()
{
	for (int i = 1; i < nbNodes+1; i++)
	{
		DegreeNodes[ListNodes[i].size()].push_back(i);//tableau de degree; on posse i-éme sommet dans son degree list 

		PointersDegreeTableau[i] = ListNodes[i].size();// tableau avec le degree de chaque node 

		Pointers[i] = --(DegreeNodes[PointersDegreeTableau[i]]).end();// vector indicate sur le element dans le Degreenodes list вектор указывающий на элемент в таблице со степенями, прмя уже в liste

	}

}

void Graph::UpdateDeegreeNode(int v, int degree)
{
	vector<int> voisins(ListNodes[v].begin(),ListNodes[v].end());// list de sommet  les voisins de v

	int nbVoisin = 0;

		for (auto& x : voisins)
		{
			if (x!=0&& PointersDegreeTableau[x]>0 )
			{
				nbVoisin++;
				if (x != v)
				{
					DegreeNodes[PointersDegreeTableau[x]].erase(Pointers[x]);//on supprime le x comme voisin de v dans DegreeNode[δ(v)]

					DegreeNodes[PointersDegreeTableau[x] - 1].push_back(x);//on pose x dans DegreeNodes[δ(v))-1]

					Pointers[x] = --DegreeNodes[PointersDegreeTableau[x] - 1].end();// on rénove  le pointeur sur x 

					--PointersDegreeTableau[x];//diminuer le degree de x, pq on supprimer 
				}
			}
		}
	nbNodes = nbNodes - 1;
	nbEdges = nbEdges - nbVoisin;
 
	ListNodes[v].clear();	
	DegreeNodes[degree].erase(Pointers[v]);
	PointersDegreeTableau[v] = -1;
}

vector<int> Graph::GetMinDegreeNode()const  // O(voisin) = max = O(n)
{
	vector<int> result(2);
	if (nbNodes == 0)
		return result;
	int i = nextDeletedeNodeDegree;
	while (true && i<nbNodes+1)
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

void Graph::WriteResult(string namefile, int isZero)
{
	fstream foutResult;
	string nameFileResult = namefile + "NewResultAlg.csv";
	foutResult.open(nameFileResult, ios::out);
	foutResult << "node1" << "," << "node2" << "\n";

	for (int i = 0; i <ListNodes.size(); i++)
	{
		if (PointersDegreeTableau[i] > 0)
		{
			for (auto& voisin : ListNodes[i])
			{
				if (PointersDegreeTableau[voisin] > 0)
				{
					if(isZero)
						foutResult << i-1 << "," << voisin-1 << "\n";
					else
						foutResult << i << "," << voisin  << "\n";
				}
			}
		}
	}
}

int main()
{
	clock_t curTime;
	curTime = clock();
	int m = 16;
	int n = 0;
	int m1=0;
	
	string nameFile;
	cin >> nameFile;

	auto started1 = std::chrono::high_resolution_clock::now();
	csvstream csvInput(nameFile+".csv");

	map<string, string> row;
	list<pair<int, int>> graph;
	int firstZero = 0;

	while (csvInput >> row)
	{
		int k = 0;
		int firstNode = -1;
		int secondNode = -1;
		for (auto& col : row)
		{
			if (k == 0)
			{
				k++;
				firstNode=stoi((col.second).c_str());
			}
			else
				secondNode = stoi((col.second).c_str());
			
		} //lire line

		if (firstNode > n || secondNode > n)
			n = max(firstNode, secondNode);
		if ((firstNode == 0 || secondNode == 0) && firstZero == 0)
		{
			firstZero = 1;
		}
		graph.push_back(make_pair(firstNode, secondNode));
		m1++;
	}

	if (firstZero == 1)
		n += 1;

	cout << "n = " << n;
	cout << "  m= " << m1<<endl;
	Graph G = Graph(n);

	for (auto& node : graph)
	{
		if(firstZero==1)
			G.AddNode(node.first+1, node.second+1);
		else
			G.AddNode(node.first, node.second);
	}


	auto started = std::chrono::high_resolution_clock::now();
	Graph H = G;
	auto done = std::chrono::high_resolution_clock::now();

	std::cout << "initialization H duration :" << std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count() << endl;

	started = std::chrono::high_resolution_clock::now();
	G.RemplirDegree();
	done = std::chrono::high_resolution_clock::now();
	std::cout << "Remplir touts les structures G :" << std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count() << endl;
	
	vector<int> v(2);

	done = std::chrono::high_resolution_clock::now();

	while (G.GetNbNodes() > 0)
	{
		v = G.GetMinDegreeNode();
		if (v[0] == v[1] && v[0] == 0)
			break;

		G.SetNextDeletedNodeDegree(v[1]-1);
		G.UpdateDeegreeNode(v[0],v[1]);

		if (G.Density() > H.Density() && G.GetNbNodes() > 0)
			H = G;
		
	}

	curTime = clock() - curTime;
	done = std::chrono::high_resolution_clock::now();

	cout << "Input data : nodes = " << n << "  ; edges = " << m1 << endl;
	std::chrono::duration<double, std::nano> duration = done - started1;
	cout << "running time: " << curTime / CLOCKS_PER_SEC << endl;
	std::cout << "millisecond duration :" << std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count() << endl;
	std::cout << "nanosecond duration: " << duration.count() << endl;



	ofstream resultDenst;
	resultDenst.open("densityOutput.txt",ios::app);
	resultDenst<<nameFile<< " Initial density: " << double(1.0 * m1 / n) << " Find density: " << H.Density();

	ofstream resultTime;
	resultTime.open("dataTime.txt", ios::app);
	resultTime << nameFile << ';' << n << ';' << m1 << ';' << double(1.0 * m1 / n) << H.Density() <<';'<< duration.count() << '\n';
	//H.WriteResult(nameFile, firstZero);

	return 0;//return H

}

