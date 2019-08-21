#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <windows.h>
#include <stack>
#include <algorithm>
#include <list>
#include <set>
#include <queue>
using namespace std;

string getexepath(){
    char result[ MAX_PATH ];
    return std::string( result, GetModuleFileName( NULL, result, MAX_PATH ) );
}

class Graph {
public:
    int vertices;
    vector<vector<int>> AdjList;

    Graph(){};

    Graph(int size){
        this->vertices = size;
        this->AdjList.resize(size);
    }


    void addEdge(int from, int to){
        AdjList[from].push_back(to);
    }

    void transposeGraph(){
        vector<vector<int>> inv;
        inv.resize(this->vertices);
        for (int i = 0; i < this->vertices; i++){
            for (int j = 0; j < this->AdjList[i].size(); j++){
                inv[AdjList[i][j]].push_back(i);
            }
        }
        this->AdjList = inv;
    }

    void initEdges(string filename){
        ifstream file;
        file.open(filename);
        if (!file){
            cerr << "File does not exist. \n";
            return;
        }

        int from, to;

        if (file.is_open()){
            while (!file.eof()){
                file >> from >> to;
                this->addEdge(from, to);
            }
            file.close();
        }
    }

    void printGraph(){
        for (int i = 0; i < this->vertices; i++){
            cout << i+1 << " --> ";
            for (int v : this->AdjList[i])
                cout << v+1 << " ";
            cout << endl;
        }
    }
};

class StrongestComponents{
private:
    int size;
    Graph graph;
    int totalComponents;
    bool isFinished;
    stack <int> Stack;
    vector <int> color;
    vector<vector<int>> components;
    vector<bool> visited;

public:
    StrongestComponents(){

    }
    StrongestComponents(Graph graph){
        this->graph = graph;
        this->size = graph.vertices;
        this->totalComponents = 0;
        this->isFinished = false;
        this->components.resize(size);
        vector<int> tmpInt2(this->size, -1);
        vector<bool> tmpBool(this->size, false);
        this->color = tmpInt2;
        this->visited = tmpBool;
    }

    void dfs(int u) {
        this->visited[u] = true;
        if (u < graph.vertices){
            for (int i = 0; i < graph.AdjList[u].size(); ++i)
            {
                if(!this->visited[graph.AdjList[u][i]]){
                    dfs(graph.AdjList[u][i]);
                }
            }
        }
        this->Stack.push(u);
        if(this->isFinished) {
            this->components[totalComponents].push_back(u);
        }
    }

    void DFS(){
        for(int i=0; i < this->size; ++i){
            if(!this->visited[i]){
                dfs(i);
            }
        }
        this->isFinished = true;
    }


    void findComponents(){

        if(!this->isFinished)
            DFS();

        this->isFinished = true;

        this->graph.transposeGraph();
        vector<int> tmpInt2(this->size, -1);
        this->color = tmpInt2;
        vector<bool> tmpBool(this->size, false);
        this->visited = tmpBool;

        while(!this->Stack.empty()){
            int ind = (int) this->Stack.top();
            this->Stack.pop();
            if(this->color[ind]==-1 && !this->visited[ind]) {
                dfs(ind);
                this->totalComponents++;
            }
        }
    }

    int longestComponentLength(){
        int max = 0;
        int index = 0;
        for (int i=0; i<this->totalComponents; i++){
            if (this->components[i].size() > max){
                max = this->components[i].size();
                index = i;
            }
        }
        return max;
    }

    vector<int> getLongestComponent(){
        int max = 0;
        int index = 0;
        for (int i=0; i<this->totalComponents; i++){
            if (this->components[i].size() > max){
                max = this->components[i].size();
                index = i;
            }
        }
        return this->components[index];
    }



    int getNumberOfComponents(){
        return this->totalComponents;
    }


};

class WeakComponents{
private:
    Graph graph;
    vector<vector<int>> components;
    vector<bool> visited;
    int totalComponents;




public:
    WeakComponents(){}

    WeakComponents(Graph g){
        this->graph = g;
        this->components.resize((g.vertices));
        this->totalComponents = 0;
        vector<bool> tmpBool(this->graph.vertices, false);
        this->visited = tmpBool;
    }

    void convertToDirected(){
        for(int i=0; i<this->graph.AdjList.size(); ++i){
            for(int j=0; j<this->graph.AdjList[i].size(); j++){
                if(std::find(this->graph.AdjList[this->graph.AdjList[i][j]].begin(), this->graph.AdjList[this->graph.AdjList[i][j]].end(),i) == this->graph.AdjList[this->graph.AdjList[i][j]].end()) {
                    this->graph.AdjList[this->graph.AdjList[i][j]].push_back(i);
                }
            }
        }
    }

    void findComponents(){
        convertToDirected();
        for(int i=0; i<this->graph.AdjList.size(); i++){
            if(visited[i] == false){
                BFS(i);
                this->totalComponents++;
            }
        }
    }

    int longestComponentLength(){
        int max = 0;
        for (int i=0; i<this->totalComponents; i++){
            if (this->components[i].size() > max) {
                max = this->components[i].size();
            }
        }
        return max;
    }

    vector<int> getLongestComponent(){
        int max = 0;
        int index = 0;
        for (int i=0; i<this->totalComponents; i++){
            if (this->components[i].size() > max) {
                max = this->components[i].size();
                index = i;
            }
        }
        return this->components[index];
    }



    void BFS(int u){
        list<int> queue;
        visited[u] = true;
        queue.push_back(u);

        while(!queue.empty()){
            u = queue.front();
            this->components[this->totalComponents].push_back(u);
            queue.pop_front();
            for(int i = 0; i<this->graph.AdjList[u].size(); i++){
                if(!visited[this->graph.AdjList[u][i]]){
                    queue.push_back(graph.AdjList[u][i]);
                    visited[this->graph.AdjList[u][i]] = true;
                }
            }
        }
    }

    int getNumberOfComponents(){
        return this->totalComponents;
    }
};



class BowTie{
private:
    Graph graph;
    StrongestComponents scc;
    WeakComponents wcc;
    vector<bool> visited;
    vector<int> lc;
    vector<int> tmp;
    int out;
    int in;
    int tendrilsAndTubes;
    int nodesReached;

public:

    BowTie(Graph g, StrongestComponents s, WeakComponents w){
        this->graph = g;
        this->scc = s;
        this->wcc = w;
        this->nodesReached = 0;
        this->lc = s.getLongestComponent();
        vector<bool> tmpBool(this->graph.vertices, false);
        this->visited = tmpBool;
    }


    vector<int> BFS(int u){
        list<int> queue;
        visited[u] = true;
        queue.push_back(u);
        vector <int> vertices;

        while(!queue.empty()){
            u = queue.front();
            vertices.push_back(u);
            queue.pop_front();
            for(int i = 0; i<this->graph.AdjList[u].size(); i++){
                if(!visited[this->graph.AdjList[u][i]]){
                    queue.push_back(graph.AdjList[u][i]);
                    visited[this->graph.AdjList[u][i]] = true;
                }
            }
        }
        return vertices;
    }


    void calculateOUT(){
        vector<bool> tmpBool(this->graph.vertices, false);

        this->visited = tmpBool;
        this->tmp.clear();
        this->tmp = BFS(this->lc[0]);

        this->out = this->tmp.size() - this->lc.size();
        cout << "\nLength of Out: " << this->out << "\n";
    }

    void calculateIN(){
        vector<bool> tmpBool(this->graph.vertices, false);
        this->visited = tmpBool;

        this->graph.transposeGraph();
        this->tmp.clear();
        this->tmp = BFS(this->lc[0]);

        this->in = this->tmp.size() - this->lc.size();
        cout << "\nLength of In: " <<  this->in <<"\n";
    }

    void calulateTendrilsandTubes(){
        this->tendrilsAndTubes = this->graph.vertices - this->in - this->out - this->lc.size();
        cout <<"\nLength of Tendrils & Tubes: " << this->tendrilsAndTubes;
    }


};

int main(){
    Graph g(75888);
    g.initEdges("epinions-network.txt");

    StrongestComponents s(g);
    s.findComponents();
    cout << "\nTotal SCC: " << s.getNumberOfComponents();
    cout << "\nMax SCC: " << s.longestComponentLength() <<"\n";

    WeakComponents w(g);
    w.findComponents();
    cout << "\nTotal WCC (Total Disconnected Components): " << w.getNumberOfComponents();
    cout << "\nMax WCC: " << w.longestComponentLength() <<"\n";

    BowTie b(g, s, w);
    b.calculateOUT();
    b.calculateIN();
    b.calulateTendrilsandTubes();


}


