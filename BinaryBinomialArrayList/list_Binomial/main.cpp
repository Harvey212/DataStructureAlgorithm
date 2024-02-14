#include <bits/stdc++.h>
#include<time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include<iostream>
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



Graph::Graph(int V)
{
    this->V = V;
    adj = new list<pair<int, int> > [V];
}

void Graph::addEdge(int u, int v, int w)
{
    adj[u].push_back(make_pair(v, w));
    //adj[v].push_back(make_pair(u, w));
}

std::vector<int> Graph::shortestPath(int src)
{
    Heap *heap;

    heap = new BinomialHeap();

    vector<int> dist(V, INF);

    (*heap).push(make_pair(0, src));

    dist[src] = 0;

    while ((*heap).top() != make_pair(-1, -1))
    {
        int u = (*heap).top().second;
        (*heap).pop();

        list< pair<int, int> >::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
        {
            int v = (*i).first;
            int weight = (*i).second;

            if (dist[v] > dist[u] + weight)
            {
                dist[v] = dist[u] + weight;
                (*heap).push(make_pair(dist[v], v));
            }
        }
    }

    //printf("Vertex Distance from Source\n");
    //for (int i = 0; i < V; ++i)
        //printf("%d \t\t %d\n", i, dist[i]);

    //printf("Number of Operations performed : %d \n", (*heap).getOperations());
    return dist;
}

int main()
{
	int V=1000;
	int x=100;
	int y=50;

	int start=0;
	int ee=1;

	int a1=0;
	int a2=0;

	std::vector<int> dd;

	int sum=0;
	int z=100;

    Graph* g = new Graph(V);
	srand(time(NULL));

    start_t = clock();
	for(int i=0;i<V;i++)
    {
        if(ee==(V))
        {
        ee=0;
        }
        g->addEdge(start, ee, 1);
        start=start+1;
        ee=ee+1;

	}

    for(int i=0;i<x;i++)
    {
        a1=rand()%V;
        a2=rand()%V;
        while(a2==a1)
        {
            a2=rand()%V;
        }
        g->addEdge(a1, a2, y);
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
        dd=g->shortestPath(a1);
        sum=sum+dd[a2];
    }
    end_t = clock();
	dif = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    cout<<"Time to complete z pairs of Dijkistra:"<<dif<<endl;

    sum=sum/z;
    cout<<"average shortest distance:"<<sum<<endl;


    return 0;
}

