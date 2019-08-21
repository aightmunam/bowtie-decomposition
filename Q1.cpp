#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <windows.h>
#include <stack>
#include <algorithm>
#include <list>
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
        AdjList.resize(size);
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

    void convertToDirected(){
        vector<vector <int>> tmp;
        tmp.resize(vertices);
        for(int i=0; i<this->AdjList.size(); ++i){
            for(int j=0; j<this->AdjList[i].size(); j++){
                if(std::find(this->AdjList[AdjList[i][j]].begin(), AdjList[AdjList[i][j]].end(),i) == this->AdjList[AdjList[i][j]].end()) {
                    cout << "Placing " << i + 1 << " in " << AdjList[i][j] + 1 << ".\n";
                    AdjList[AdjList[i][j]].push_back(i);
                }
            }
        }


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
                this->addEdge(from-1, to-1);
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

    void DFSIterative(int u){
        int total = 0;
        stack<int> stack;
        stack.push(u);
        while(!stack.empty()){
            u = stack.top();
            if (this->color[u] != -1){
                stack.pop();
                if (this->color[u] == 1){
                    if (this->isFinished){
                       this->components[this->totalComponents].push_back(u);
                    }
                    this->Stack.push(u);
                    this->color[u] = 2;
                }
            } else {
                this->color[u] = 1;
                total++;
                for (int i = this->graph.AdjList[u].size()-1; i>=0; --i){
                    if(this->color[this->graph.AdjList[u][i]] == -1){
                        stack.push(this->graph.AdjList[u][i]);
                    }
                }
            }
        }
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

    int maxLengthComponent(){
        int max = 0;
        int index = 0;
        for (int i=0; i<this->totalComponents; i++){
            if (this->components[i].size() > max){
                max = this->components[i].size();
                index = i;
            }
        }
        cout << "The index of longest component: "<< index+1 << ". \n";
        cout << "The length of the longest component: ";
        return max;
    }

    void writeToFile(){
        ofstream myfile;
        myfile.open("output.txt", ios_base::out | ios_base::trunc);
        myfile.close();


        myfile.open("output.txt", ios_base::out);
        for(int i=0; i<this->totalComponents; ++i){
            myfile << "\n" << i+1 << ":\t" << "size=" << this->components[i].size() << "\t\t";

            for(int j=0; j<this->components[i].size(); ++j){
                if (j != 0){
                    myfile << ", ";
                }
                myfile << this->components[i][j] + 1;

            }
        }
    }

    int getNumberOfComponents(){
        return this->totalComponents;
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

};



int main(){
    Graph g(875715);
    g.initEdges("SCC.txt");
    StrongestComponents s(g);
    s.findComponents();
    s.writeToFile();

}


