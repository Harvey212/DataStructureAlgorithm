#include <iostream>
#include <limits.h>
#include<climits>
#include <stdio.h>
#include <cstdlib>
#include<time.h>
#define V 1000
using namespace std;
clock_t start_t, end_t;
double dif;

// A utility function to find the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
// Structure to represent a min heap node
struct MinHeapNode
{
	int v;
	int dist;
};

// Structure to represent a min heap
struct MinHeap
{
	int size;	 // Number of heap nodes present currently
	int capacity; // Capacity of min heap
	int *pos;	 // This is needed for decreaseKey()
	struct MinHeapNode **array;
};

// A utility function to create a new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, int dist)
{
	struct MinHeapNode* minHeapNode =
		(struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
	minHeapNode->v = v;
	minHeapNode->dist = dist;
	return minHeapNode;
}

// A utility function to create a Min Heap
struct MinHeap* createMinHeap(int capacity)
{
	struct MinHeap* minHeap =
		(struct MinHeap*) malloc(sizeof(struct MinHeap));
	minHeap->pos = (int *)malloc(capacity * sizeof(int));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array =
		(struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
	return minHeap;
}

// A utility function to swap two nodes of min heap. Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap, int idx)
{
	int smallest, left, right;
	smallest = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;

	if (left < minHeap->size &&
		minHeap->array[left]->dist < minHeap->array[smallest]->dist )
	smallest = left;

	if (right < minHeap->size &&
		minHeap->array[right]->dist < minHeap->array[smallest]->dist )
	smallest = right;

	if (smallest != idx)
	{
		// The nodes to be swapped in min heap
		MinHeapNode *smallestNode = minHeap->array[smallest];
		MinHeapNode *idxNode = minHeap->array[idx];

		// Swap positions
		minHeap->pos[smallestNode->v] = idx;
		minHeap->pos[idxNode->v] = smallest;

		// Swap nodes
		swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

		minHeapify(minHeap, smallest);
	}
}

// A utility function to check if the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap)
{
	return minHeap->size == 0;
}

// Standard function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
	if (isEmpty(minHeap))
		return NULL;

	// Store the root node
	struct MinHeapNode* root = minHeap->array[0];

	// Replace root node with last node
	struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	// Update position of last node
	minHeap->pos[root->v] = minHeap->size-1;
	minHeap->pos[lastNode->v] = 0;

	// Reduce heap size and heapify root
	--minHeap->size;
	minHeapify(minHeap, 0);

	return root;
}

// Function to decreasy dist value of a given vertex v. This function
// uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
	// Get the index of v in heap array
	int i = minHeap->pos[v];

	// Get the node and update its dist value
	minHeap->array[i]->dist = dist;

	// Travel up while the complete tree is not hepified.
	// This is a O(Logn) loop
	while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
	{
		// Swap this node with its parent
		minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
		minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
		swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

		// move to parent index
		i = (i - 1) / 2;
	}
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
bool isInMinHeap(struct MinHeap *minHeap, int v)
{
if (minHeap->pos[v] < minHeap->size)
	return true;
return false;
}

// A utility function used to print the solution
void printArr(int dist[], int n)
{
	printf("Vertex Distance from Source\n");
	for (int i = 0; i < n; ++i)
		printf("%d \t\t %d\n", i, dist[i]);
}


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
    struct MinHeap* minHeap = createMinHeap(V);
	for (int v = 0; v < V; ++v)
	{
		dist[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v, dist[v]);
		minHeap->pos[v] = v;
		sptSet[v] = false;
	}


	minHeap->array[src] = newMinHeapNode(src, dist[src]);
	minHeap->pos[src] = src;
	dist[src] = 0;
	decreaseKey(minHeap, src, dist[src]);

	minHeap->size = V;

    while (!isEmpty(minHeap))
	{

		struct MinHeapNode* minHeapNode = extractMin(minHeap);
		int u = minHeapNode->v;
        sptSet[u] = true;

		for (int v = 0; v < V; v++)
		{

			if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&
										dist[u] + graph[u][v] < dist[v])
			{
				dist[v] = dist[u] + graph[u][v];

				// update distance value in min heap also
				decreaseKey(minHeap, v, dist[v]);
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

