/*
Author : @Rajat Shah

Task: Solving 8-tile puzzle using A* algorithm with Manhattan Distance as Heuristic.
Comments: Configurations can be done from the macro declarations
0 can be considered as the blank position

Sample Solvable Initial State:
1 | 4 | 2
3 | 5 | 0
6 | 7 | 8

Input format for the above Configuration:
1 4 2 3 5 0 6 7 8

Goal State:
0 | 1 | 2
3 | 4 | 5
6 | 7 | 8
*/

#include<iostream>
#include<cmath>
#include<cstdlib>
#include<algorithm>
#include<sstream>
#include<cstring>
#include<map>
#include<list>
#include<queue>
#include <ctime>
using namespace std;

#define N 3         //grid size NxN
#define SHOW_STEPS 1
#define MAX_EXPLORED_SIZE  181440

typedef struct node_type
{
    int grid[N][N];   // with number 0 as blank tile
    node_type * parent;  //pointer to the parent node
    int fvalue;     //g+h value
    int height;     //g+h value
} node;

struct comp
{
    bool operator()(node* l, node* r)
    {
        return l->fvalue >=  r->fvalue;
    }
};

priority_queue< node*, vector<node*>, comp > openList;       //nodes we haven't yet expanded
map<string,int> closedList;

int manhattan_distance(node *puzzle)        //heuristic function to calculate distance of source to goal node
{
    int c=0,distSum=0;
    for(int i = 0; i<N; i++)
        for(int j=0; j<N; j++,c++)
            if( c && puzzle->grid[i][j]!=c)        //tile not at right place
                distSum += abs( (i-((puzzle->grid[i][j])/N)) + (j-((puzzle->grid[i][j])%N)) );     //verify it!!!!
    return distSum;
}

//read the initial configuration as input
void get_grid(node *puzzle)
{
    cout<<"Enter initial grid."<<endl;
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            cin>>(puzzle->grid[i][j]);

    puzzle->fvalue = 0+ manhattan_distance(puzzle);
    puzzle->height=0;
}

//displays nice formatted grid
void show_grid(node *puzzle)
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            if(puzzle->grid[i][j])
                cout<<puzzle->grid[i][j]<<" | ";
            else
                cout<<" "<<" | ";
        }
        cout<<endl;
    }
    cout<<endl;
}

node* createNode(node *puzzle)
{
    node *newNode  = (node*)malloc( sizeof(node) );
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            newNode->grid[i][j] = puzzle->grid[i][j];
    return newNode;
}

//test for the goal
bool isGoal(node* puzzle)
{
    int c=0;
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++,c++)
            if( (i!=N-1 || j!=N-1) && puzzle->grid[i][j]!=c)
                return false;
    return true;
}

//generates successor of the current ndoe
list<node*> getSuccessors(node *puzzle)
{
    list<node*> successors;
    node * temp;
    int blankTilePosX=-1,blankTilePosY=-1, numSucc = 0;
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            if(puzzle->grid[i][j]==0)
            {
                blankTilePosX=i;
                blankTilePosY=j;
            }
    if(blankTilePosX!=-1 && blankTilePosY!=-1)          //ensure the blank tile was found
    {
        if( blankTilePosX-1 != -1 ) //slide left possible
        {
            temp = createNode(puzzle);
            swap(temp->grid[blankTilePosX][blankTilePosY], temp->grid[blankTilePosX-1][blankTilePosY] );
            successors.push_back(temp);
        }
        if( blankTilePosX+1 != N )     //slide right possible
        {
            temp = createNode(puzzle);
            swap(temp->grid[blankTilePosX][blankTilePosY], temp->grid[blankTilePosX+1][blankTilePosY] );
            successors.push_back(temp);
        }
        if( blankTilePosY-1 !=-1 )      //slide up possible
        {
            temp = createNode(puzzle);
            swap(temp->grid[blankTilePosX][blankTilePosY], temp->grid[blankTilePosX][blankTilePosY-1] );
            successors.push_back(temp);
        }
        if( blankTilePosY+1 != N )      //slide down possible
        {
            temp = createNode(puzzle);
            swap(temp->grid[blankTilePosX][blankTilePosY], temp->grid[blankTilePosX][blankTilePosY+1] );
            successors.push_back(temp);
        }
    }
    return successors;
}

string puzzle_to_string(node *puzzle)
{
    string puzzleString ="";
    ostringstream ostr;
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            ostr << puzzle->grid[i][j];
            puzzleString += ostr.str();
        }
    }
    return puzzleString;
}

//the A* algorithm, returns just the number of moves to the goal state, and optionally prints the steps of moves
int AStar(node *puzzle)
{
    int numMoves=0;
    openList.push(puzzle);
    string puzzleString;    // the input board configuration in the string
    list<node*> successors;
    while(!openList.empty() && closedList.size()<= MAX_EXPLORED_SIZE)
    {
        node* popped = openList.top();          //node with least f value
        openList.pop();
        puzzleString = puzzle_to_string(popped);
        closedList[puzzleString]=1;       //adding processed node to closed list

        //test for the goal state
        if(isGoal(popped))
        {
#if SHOW_STEPS
            cout<<"-------------Showing Steps in Reverse Order-------------\n"<<endl;
#endif
            //iterate over all the steps to the solution
            while(popped!=NULL)
            {
#if SHOW_STEPS
                show_grid(popped);
#endif
                popped=popped->parent;
                numMoves++;
            }
            return (numMoves-1);
        }
        else
        {
            successors = getSuccessors(popped);
            while( !successors.empty() )
            {
                node* temp = createNode(successors.front());
                temp->parent=popped;
                temp->fvalue = (popped->height) +1 + manhattan_distance(temp);
                temp->height=(popped->height)+1;

                string puzzleString = puzzle_to_string(temp);
                if( !closedList[puzzleString] )
                    openList.push(temp);
                successors.pop_front();
            }
        }
    }
    return numMoves;   //return 0 when unsolvable
}
//tests for solvability
bool isSolvable(node* puzzle)
{
    int numInversions=0,blankTileRow=0;
    int a[N*N],k=0;
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++,k++)
            a[k] = puzzle->grid[i][j];

    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++,k++)
            if(puzzle->grid[i][j]==0)
                blankTileRow = i;

    for(int i=0; i<N*N; i++)
        for(int j=i+1; j<N*N; j++)
            if(a[i] && a[j] && a[i]>a[j])
                numInversions++;

    if( (N%2==1 && numInversions%2==0 ) || ((N%2==0) && ( (numInversions+ blankTileRow)%2==1 ) ))
        return true;
    else
        return false;
}

int main()
{
    node *puzzle = (node*)malloc( sizeof(node) );
    int numSteps;
    clock_t start_time, end_time;
    puzzle->parent=NULL;
    get_grid(puzzle);
    show_grid(puzzle);
    start_time = clock();
    if(isSolvable(puzzle) && (numSteps = AStar(puzzle)) )
    {
        cout<<"Number of moves to solution:\t"<<numSteps<<endl;
        cout<<"Number of nodes expanded :\t"<<closedList.size()<<endl;
    }
    else
        cout<<"Unsolvable Puzzle"<<endl;

    end_time = clock();

    cout << "Execution time: \t"  << 1000*((double)(end_time-start_time)/CLOCKS_PER_SEC) << " milliseconds." << "\n\n";

    return 0;
}
