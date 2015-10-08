/*
Name : Rajat Shah
Task : Implement DFS and BFS for an undirected graph and unweighted graph.
Notes: Contains hard-coded map of the city to be searched, uses just string reprentation of city(nodes)!

Instructions to compile and run:

To compile:
g++ BFSDFS.cpp

To run (example):
./a.out dfs zerind oradea
./a.out bfs zerind oradea

Output format:

- First line shows the from and to cities and search type given as input
- Next line, size of explored set implies the number of nodes expanded
- Next line has the path to reach from source city to destination city.

*/
#include<iostream>
#include<stack>
#include<queue>
#include<list>
#include<string>
#include<map>
#include<algorithm>
#include<set>
#include<cstring>
#include<cstdlib>
#include<iterator>
using namespace std;

class SearchingProblem
{
private:
    map<string, list<string> > romania_map;     //City1 -> [City2, City3]

public:
    SearchingProblem();
    bool addUndirectedEdge(string, string);
    bool addDirectedEdge(string, string);
    list<string> getNeighbours(string);
    bool areNeighbours (string, string);
    list<string> BFS(string, string);
    list<string> DFS(string, string);
};

//Hard Coded as the input graph of the city known, can be easily modified to read from file
SearchingProblem::SearchingProblem()
{
    addUndirectedEdge ( "oradea" , "zerind" );
    addUndirectedEdge ( "zerind" , "arad" );
    addUndirectedEdge ( "arad" , "timisoara" );
    addUndirectedEdge ( "timisoara" , "lugoj" );
    addUndirectedEdge ( "lugoj" , "mehadia" );
    addUndirectedEdge ( "dobreta" , "mehadia" );
    addUndirectedEdge ( "oradea" , "sibiu" );
    addUndirectedEdge ( "arad" , "sibiu" );
    addUndirectedEdge ( "dobreta" , "craiova" );
    addUndirectedEdge ( "sibiu" , "rimnicu_vilcea" );
    addUndirectedEdge ( "sibiu" , "fagaras" );
    addUndirectedEdge ( "rimnicu_vilcea" , "craiova" );
    addUndirectedEdge ( "pitesti" , "craiova" );
    addUndirectedEdge ( "rimnicu_vilcea" , "pitesti" );
    addUndirectedEdge ( "bucharest" , "pitesti" );
    addUndirectedEdge ( "bucharest" , "fagaras" );
    addUndirectedEdge ( "bucharest" , "giurgiu" );
    addUndirectedEdge ( "bucharest" , "urziceni" );
    addUndirectedEdge ( "vaslui" , "urziceni" );
    addUndirectedEdge ( "hirsova" , "urziceni" );
    addUndirectedEdge ( "hirsova" , "eforie" );
    addUndirectedEdge ( "vaslui" , "iasi" );
    addUndirectedEdge ( "neamt" , "iasi" );

    for(map<string,list<string> >::iterator it = romania_map.begin(); it != romania_map.end(); ++it)
    {
        romania_map[it->first].sort();
        romania_map[it->first].reverse();
    }
}

//returns true on success
bool SearchingProblem::addUndirectedEdge(string city1, string city2)
{
    if(city1 == city2)
        return false;
    if(find(romania_map[city1].begin(),romania_map[city1].end(),city2) == romania_map[city1].end())
        romania_map[city1].push_back(city2);
    if(find(romania_map[city2].begin(),romania_map[city2].end(),city1) == romania_map[city2].end())
        romania_map[city2].push_back(city1);

    return true;
}

list<string> SearchingProblem::getNeighbours(string city)
{
    return romania_map[city];
}

//returns path from->to
list<string> SearchingProblem::BFS(string start, string goal)
{
    list<string> path;
    map<string,string> parent;
    deque<string> frontier;     //treat like queue and push at back and pop from front
    set<string> explored;
    string current;
    if(start == goal)
        return path;

    frontier.push_back(start);

    while(!frontier.empty())
    {
        current = frontier.front();
        frontier.pop_front();
        explored.insert(current);
        list<string> neighbours = getNeighbours(current);
        neighbours.reverse();
        for(list<string>::iterator it = neighbours.begin(); it!=neighbours.end(); ++it )
        {
            string child = *it;
            if(explored.find(child) == explored.end() && find(frontier.begin(),frontier.end(),child)==frontier.end())
            {
                parent[child] = current;
                if(child==goal)
                {
                    //construct path
                    while(child!=start)
                    {
                        path.push_back(child);
                        child = parent[child];
                    }
                    path.push_back(start);
                    cout<<"Size of explored set= "<<explored.size()<<endl;
                    path.reverse();
                    return path;
                }
                frontier.push_back(child);
            }
        }
    }
    return path;
}

//returns path from->to
list<string> SearchingProblem::DFS(string start, string goal)
{
    list<string> path;
    map<string,string> parent;
    deque<string> frontier; //treat like stack and push/pop at back only
    set<string> explored;
    string current;

    frontier.push_back(start);
    if(start == goal)
    {
        path.push_back(start);
        return path;
    }
    while(!frontier.empty())
    {
        current = frontier.back();
        frontier.pop_back();
        explored.insert(current);

        if(current == goal)
        {
            while(current != start && current !="")
            {
                path.push_back(current);
                current = parent[current];
            }
            path.push_back(start);
            cout<<"Size of explored set= "<<explored.size()<<endl;
            path.reverse();
            return path;
        }
        list<string> neighbours = getNeighbours(current);
        for(list<string>::iterator it = neighbours.begin(); it!=neighbours.end(); ++it )
        {
            string child = *it;
            if(explored.find(child) == explored.end())
            {
                frontier.push_back(child);
                parent[child] = current;
            }
        }
    }
    return path;
}

int main(int argc, char* argv[])
{
    SearchingProblem problem;
    string from, to, search_type;
    if(argc!=4)
    {
        cout<<"Provide 3 cmd line args, only "<<argc<<" provided."<<endl;
    }
    search_type = string(argv[1]);
    from = string(argv[2]);
    to = string(argv[3]);

    cout<<endl;
    if(search_type == "bfs")
    {
        cout<<"BFS from "<<from<<" to "<<to<<endl;
        list<string> path =  problem.BFS(from,to);
        for(list<string>::iterator it = path.begin(); it!=path.end(); ++it )
        {
            cout<<*it<<"->";
        }
        cout<<"\b\b  "<<endl<<endl;
    }
    else
    {
        cout<<"DFS from "<<from<<" to "<<to<<endl;
        list<string> path =  problem.DFS(from,to);
        for(list<string>::iterator it = path.begin(); it!=path.end(); ++it )
        {
            cout<<*it<<"->";
        }
        cout<<"\b\b  "<<endl<<endl;
    }
    return 0;
}
