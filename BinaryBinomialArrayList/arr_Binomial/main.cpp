#include <iostream>
#include <limits.h>
#include<climits>
#include <stdio.h>
#include <cstdlib>
#include<time.h>
#define V 1000
#define INF 0x3f3f3f3f
using namespace std;
clock_t start_t, end_t;
double dif;

class Heap
{
    public:
        virtual pair<int, int> top() = 0;
        virtual pair<int, int> pop() = 0;
        virtual void push(pair<int, int>) = 0;
        virtual int getOperations() = 0;
    private:
        int operations;
};

class Graph
{
    private:
        int V;
        list< pair<int, int> > *adj;
    public:
        Graph(int V);
        void addEdge(int u, int v, int w);
        std::vector<int> shortestPath(int s);
};

struct BNode
{
    pair<int,int> data;
    int degree;
    BNode *child, *sibling, *parent;

    explicit BNode(pair<int, int>);
};

class BinomialHeap: public Heap
{
    public:
        BinomialHeap();
        pair<int, int> top();
        pair<int, int> pop();
        void push(pair<int, int>);
        int getOperations();
        void printTree(BNode *h);
        void printHeap();
    protected:
        pair<int, int> extractMin();
        void insertATreeInHeap(BNode *tree);
        list<BNode*> removeMinFromTreeReturnBHeap(BNode *tree);
        BNode* mergeBinomialTrees(BNode *b1, BNode *b2);
        list<BNode*> unionBinomialHeap(list<BNode*> l1,list<BNode*> l2);
        void adjust();
        void insert(pair<int,int> key);
        BNode* getMin();
    private:
        int operations;
        list<BNode*> heap;
};

BinomialHeap::BinomialHeap()
{
        operations = 0;
}

int BinomialHeap::getOperations()
{
    return operations;
}

void BinomialHeap::push(pair<int, int> BNode)
{
    insert(BNode);
    return;
}

pair<int, int> BinomialHeap::top()
{
    if(heap.size() == 0)
        return make_pair(-1, -1);

    return getMin()->data;
}

pair<int, int> BinomialHeap::pop()
{
    if(heap.size() == 0)
        return make_pair(-1, -1);

    pair<int, int> BNode = extractMin();

    return BNode;
}

pair<int, int> BinomialHeap::extractMin()
{
    if(heap.size() == 0)
        return make_pair(-1, -1);
    list<BNode*> new_heap,lo;
    BNode *temp;
    temp = getMin();
    list<BNode*>::iterator it;
    it = heap.begin();
    while (it != heap.end())
    {
        if (*it != temp)
        {
            new_heap.push_back(*it);
        }
        it++;
    }
    operations++;
    lo=removeMinFromTreeReturnBHeap(temp);
    heap = unionBinomialHeap(new_heap,lo);
    adjust();
    return temp->data;
}

BNode::BNode(pair<int,int> key)
{
    this->data = key;
    this->degree = 0;
    this->child = this->parent = this->sibling = NULL;
}
void BinomialHeap::insert(pair<int,int> key)
{
    BNode *temp = new BNode(key);
    insertATreeInHeap(temp);
}


void swap(BNode *b1,BNode *b2)
{
    BNode *a;
    a=b1;
    b1=b2;
    b2=a;
}

BNode* BinomialHeap::mergeBinomialTrees(BNode *b1, BNode *b2)
{
    if ((b1->data).first > (b2->data).first)
    {
        swap(b1, b2);
        operations++;
    }
    b2->parent = b1;
    b2->sibling = b1->child;
    b1->child = b2;
    b1->degree++;

    return b1;
}

list<BNode*> BinomialHeap::unionBinomialHeap(list<BNode*> l1,
                               list<BNode*> l2)
{
    list<BNode*> _new;
    list<BNode*>::iterator it = l1.begin();
    list<BNode*>::iterator ot = l2.begin();
    while (it!=l1.end() && ot!=l2.end())
    {
        if((*it)->degree <= (*ot)->degree)
        {
            _new.push_back(*it);
            it++;
        }
        else
        {
            _new.push_back(*ot);
            ot++;
        }
    }
    while (it != l1.end())
    {
        _new.push_back(*it);
        it++;
    }
    while (ot!=l2.end())
    {
        _new.push_back(*ot);
        ot++;
    }
    operations++;
    return _new;
}

void BinomialHeap::adjust()
{
    if (heap.size() <= 1)
        return;
    list<BNode*> new_heap;
    list<BNode*>::iterator it1,it2,it3;
    it1 = it2 = it3 = heap.begin();

    if (heap.size() == 2)
    {
        it2 = it1;
        it2++;
        it3 = heap.end();
    }
    else
    {
        it2++;
        it3=it2;
        it3++;
    }
    while (it1 != heap.end())
    {
        if (it2 == heap.end())
            it1++;
        else if ((*it1)->degree < (*it2)->degree)
        {
            it1++;
            it2++;
            if(it3!=heap.end())
                it3++;
        }
        else if (it3!=heap.end() &&
                (*it1)->degree == (*it2)->degree &&
                (*it1)->degree == (*it3)->degree)
        {
            it1++;
            it2++;
            it3++;
        }
        else if ((*it1)->degree == (*it2)->degree)
        {
            *it1 = mergeBinomialTrees(*it1,*it2);
            it2 = heap.erase(it2);
            if(it3 != heap.end())
                it3++;
        }
    }
    operations++;
    return;
}

void BinomialHeap::insertATreeInHeap(BNode *tree)
{
    list<BNode*> temp;
    temp.push_back(tree);
    heap = unionBinomialHeap(heap,temp);
    adjust();
    return;
}

list<BNode*> BinomialHeap::removeMinFromTreeReturnBHeap(BNode *tree)
{
    list<BNode*> tempheap;
    BNode *temp = tree->child;
    BNode *lo;
    operations++;
    while (temp)
    {
        lo = temp;
        temp = temp->sibling;
        lo->sibling = NULL;
        tempheap.push_front(lo);
    }
    return tempheap;
}

BNode* BinomialHeap::getMin()
{
    if (heap.size() == 0)
        return NULL;
    list<BNode*>::iterator it = heap.begin();
    BNode *temp = *it;
    operations++;
    while (it != heap.end())
    {
        if (((*it)->data).first < (temp->data).first)
            temp = *it;
        it++;
    }
    return temp;
}
void BinomialHeap::printTree(BNode *h)
{
    while (h)
    {
        printTree(h->child);
        h = h->sibling;
    }
}

void BinomialHeap::printHeap()
{
    list<BNode*> ::iterator it;
    it = heap.begin();
    while (it != heap.end())
    {
        printTree(*it);
        it++;
    }
}
// A utility function to find the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
//int minDistance(int dist[], bool sptSet[])
//{
    // Initialize min value
//    int min = INT_MAX, min_index;

//    for (int v = 0; v < V; v++)
//        if (sptSet[v] == false && dist[v] <= min)
//            min = dist[v], min_index = v;

//    return min_index;
//}

// A utility function to print the constructed distance array
void printSolution(int dist[], int n)
{
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d tt %d\n", i, dist[i]);
}

// Function that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
int* dijkstra(int graph[V][V], int src)
{
    static int dist[1000];

    bool sptSet[V];
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;

    Heap *heap;
    heap = new BinomialHeap();
    (*heap).push(make_pair(0, src));
    dist[src] = 0;

    while ((*heap).top() != make_pair(-1, -1))
    {
        int u = (*heap).top().second;
        (*heap).pop();
        sptSet[u] = true;
        list< pair<int, int> >::iterator i;
        for (int v = 0; v < V; v++)
        {

            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph[u][v];
                (*heap).push(make_pair(dist[v], v));
            }
        }
    }
    }

    return dist;
    // print the constructed distance array
    //printSolution(dist, V);
}


int main()
{
	int x=100;
	int y=50;

	int a1=0;
	int a2=0;

	int flag=0;

	int* dd;

	int sum=0;
	int z=50;

	srand(time(NULL));

    int graph[V][V];

    start_t = clock();
    for(int i=0;i<V;i++)
    {
        for(int j=0;j<V;j++)
        {

            if(i==0)
            {
               if((j==1)||(j==(V-1))){
                graph[i][j]=1;
               }else{
               graph[i][j]=0;
               }

            }else if(i==(V-1))
            {
                if((j==0)||(j==(V-2))){
                    graph[i][j]=1;
                }else{
                    graph[i][j]=0;
                }

            }else{
                if(j==(i-1)){
                    graph[i][j]=1;
                }else if(j==(i+1)){
                     graph[i][j]=1;
                }else{
                     graph[i][j]=0;
                }
            }

        }
    }

    for(int i=0;i<x;i++)
    {
        flag=0;
        while(flag==0)
        {
            a1=rand()%V;
            a2=rand()%V;

            if(a1==a2){
                flag=0;
            }else if(a1==0)
            {
                if((a2==1)||(a2==(V-1)))
                {
                    flag=0;
                }else{
                    flag=1;
                }

            }else if(a1==(V-1))
            {
                if((a2==0)||(a2==(V-2)))
                {
                    flag=0;
                }else{
                    flag=1;
                }
            }else{
                if(a2==(a1-1))
                {
                    flag=0;
                }else if(a2==(a1+1)){
                    flag=0;
                }else{
                    flag=1;
                }

            }
        }

        graph[a1][a2]=y;

    }
    end_t = clock();
	dif = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    cout<<"Time to construct graph:"<<dif<<endl;

    start_t = clock();
    for(int i=0;i<z;i++)
    {
        a1=rand()%V;
        a2=rand()%V;
        while(a2==a1)
        {
            a2=rand()%V;
        }
        dd=dijkstra(graph, a1);
        sum=sum+dd[a2];
    }
    end_t = clock();
	dif = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    cout<<"Time to complete z pairs of Dijkistra:"<<dif<<endl;

    sum=sum/z;
    cout<<"average shortest distance:"<<sum<<endl;

	return 0;
}

