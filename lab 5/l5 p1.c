#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

#define ROOT 4

int main(int argc, char* argv[])
{
int rank; 
int p; 
int sursa; 
int destinatie; 
int M;
int eticheta = 99;
int calculMesaj; 
MPI_Status status; 
MPI_Comm graph;

int index[] = {1, 2, 5, 7, 10, 11, 14, 15, 16};
int nrOfNodes = 9;
int edges[] = {4, 6, 4, 7, 8, 4, 6, 0, 2, 3, 6, 1, 3, 5, 2, 2};

MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &p);
MPI_Graph_create(MPI_COMM_WORLD, nrOfNodes, index, edges, 0, &graph);
int count;
MPI_Graph_neighbors_count(graph, rank, &count);
 
int* neighbors = (int*) malloc(count * sizeof(int));
        
if (!neighbors)
    {
        fprintf(stderr, "erorr\n");
        MPI_Comm_free(&graph);
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

  int i;
  int  sumaVecini = 0;
    for (i = 0; i < count; ++i)
    {
        sumaVecini += neighbors[i];
    }

if (rank == ROOT){
int calculMesaj = 0;
        for (i = 0; i < count; ++i)
        {
            sursa = neighbors[i];
            MPI_Recv(&M, 1, MPI_INT, sursa, eticheta, graph, &status);
            printf("[Radacina %d] Am primit mesajul \"%d\" %d.\n", rank, M, sursa);

calculMesaj += M;
        }
        printf("[Radacina %d] Suma de ID-uri calculata: %d.\n", rank, calculMesaj);
    }
    else
    {
        if (count == 1) 
        {
	    destinatie = neighbors[0]; 
            MPI_Send(&rank, 1, MPI_INT, destinatie, eticheta, graph);
            printf("[Nod %d] Am trimis mesajul \"%d\" catre destinatia %d.\n", rank, rank, destinatie);
        }

 else 
        {
 calculMesaj = 0;
            for (i = 0; i < count - 1; ++i)
            {
                sursa = neighbors[i];
                MPI_Recv(&M, 1, MPI_INT, MPI_ANY_SOURCE, eticheta, graph, &status);
                printf("[Nod %d] Am primit mesajul \"%d\" de la sursa %d.\n", rank, M, sursa);
                sumaVecini -= status.MPI_SOURCE;
calculMesaj += M;
            }
destinatie = sumaVecini;
calculMesaj += rank;
            MPI_Send(&calculMesaj, 1, MPI_INT, destinatie, eticheta, graph);
            printf("[Nod %d] Am trimis mesajul \"%d\" catre destinatia %d.\n", rank, calculMesaj, destinatie);
        }
    }
 if (neighbors)
    {
        free(neighbors);
    }
MPI_Comm_free(&graph);
MPI_Finalize();

    return 0;
} 