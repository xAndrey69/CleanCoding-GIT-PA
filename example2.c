/* Parcurgere graf cu DFS și BFS */

// Biblioteci necesare
#include <stdlib.h>
#include <stdio.h>

// Definirea structurii unui nod din lista de adiacență
typedef struct Node {
    int data;             // Datele nodului (reprezentând un restaurant, de exemplu)
    struct Node *next;    // Pointeur către următorul nod din lista de adiacență
} NODE;

// Definirea structurii grafului
typedef struct Graph {
    int vertices;               // Numărul de noduri din graf
    int *visited;               // Vector pentru a marca nodurile vizitate
    struct Node **adjacency_lists; // Listele de adiacență pentru fiecare nod
} GPH;

// Funcție de creare a unui nod nou
NODE *create_node(int v) {
    NODE *new_node = malloc(sizeof(NODE));
    new_node->data = v;
    new_node->next = NULL;
    return new_node;
}

// Funcție de creare a grafului
GPH *create_graph(int vertices) {
    int i;
    GPH *graph = malloc(sizeof(GPH));
    graph->vertices = vertices;
    graph->adjacency_lists = malloc(vertices * sizeof(NODE *));
    graph->visited = malloc(sizeof(int) * vertices);

    // Inițializarea listelor de adiacență și a vectorului de vizitate
    for (i = 0; i < vertices; i++) {
        graph->adjacency_lists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

// Funcție de adăugare a unei muchii între două noduri
void add_edge(GPH *graph, int src, int dest) {
    NODE *new_node = create_node(dest);
    new_node->next = graph->adjacency_lists[src];
    graph->adjacency_lists[src] = new_node;

    new_node = create_node(src);
    new_node->next = graph->adjacency_lists[dest];
    graph->adjacency_lists[dest] = new_node;
}

// Funcție de inserare a muchiilor în graf
int *insert_edges(int nr_of_vertices, int nr_of_edges, GPH *graph) {
    int src, dest, i;
    printf("Adauga %d muchii (de la 1 la %d)\n", nr_of_edges, nr_of_vertices);
    for (i = 0; i < nr_of_edges; i++) {
        scanf("%d%d", &src, &dest);
        add_edge(graph, src, dest);
    }
    return NULL;
}

// Verifică dacă coada este goală
int is_empty(NODE *queue) {
    return queue == NULL;
}

// Funcție de inserare a unui element în coadă
void enqueue(NODE **queue, int data) {
    NODE *new_node = create_node(data);
    if (is_empty(*queue)) {
        *queue = new_node;
    } else {
        NODE *temp = *queue;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

// Funcție de extragere a unui element din coadă
int dequeue(NODE **queue) {
    int data = (*queue)->data;
    NODE *temp = *queue;
    *queue = (*queue)->next;
    free(temp); // Eliberăm memoria nodului extras
    return data;
}

// Funcție de afișare a grafului
void print_graph(GPH *graph) {
    int i;
    for (i = 0; i < graph->vertices; i++) {
        NODE *temp = graph->adjacency_lists[i];
        printf("Vertex %d: ", i);
        while (temp) {
            printf("%d ", temp->data);
            temp = temp->next;
        }
        printf("\n");
    }
}

// Funcție de afișare a cozii
void print_queue(NODE *queue) {
    while (queue != NULL) {
        printf("%d ", queue->data);
        queue = queue->next;
    }
    printf("\n");
}

// Funcție de resetare a vectorului de noduri vizitate
void wipe_visited_list(GPH *graph, int nr_of_vertices) {
    for (int i = 0; i < nr_of_vertices; i++) {
        graph->visited[i] = 0;
    }
}

// Parcurgere DFS (Depth-First Search)
void DFS(GPH *graph, int vertex_nr) {
    NODE *adj_list = graph->adjacency_lists[vertex_nr];
    NODE *temp = adj_list;

    graph->visited[vertex_nr] = 1;
    printf("%d -> ", vertex_nr);

    while (temp != NULL) {
        int connected_vertex = temp->data;
        if (graph->visited[connected_vertex] == 0) {
            DFS(graph, connected_vertex);
        }
        temp = temp->next;
    }
}

// Parcurgere BFS (Breadth-First Search)
void BFS(GPH *graph, int start) {
    NODE *queue = NULL;

    graph->visited[start] = 1;
    enqueue(&queue, start);

    while (!is_empty(queue)) {
        int current = dequeue(&queue);
        printf("%d ", current);

        NODE *temp = graph->adjacency_lists[current];
        while (temp) {
            int adj_vertex = temp->data;
            if (graph->visited[adj_vertex] == 0) {
                graph->visited[adj_vertex] = 1;
                enqueue(&queue, adj_vertex);
            }
            temp = temp->next;
        }
    }
}

// Funcția principală
int main() {
    int nr_of_vertices, nr_of_edges, starting_vertex;

    // Citirea numărului de noduri și muchii
    printf("Cate noduri are graful? ");
    scanf("%d", &nr_of_vertices);

    printf("Cate muchii are graful? ");
    scanf("%d", &nr_of_edges);

    // Crearea grafului
    GPH *graph = create_graph(nr_of_vertices);

    // Inserarea muchiilor
    insert_edges(nr_of_vertices, nr_of_edges, graph);

    // Parcurgere DFS
    printf("De unde plecam in DFS? ");
    scanf("%d", &starting_vertex);
    printf("Parcurgere cu DFS: ");
    DFS(graph, starting_vertex);
    printf("\n");

    // Resetarea listei de vizitate
    wipe_visited_list(graph, nr_of_vertices);

    // Parcurgere BFS
    printf("De unde plecam in BFS? ");
    scanf("%d", &starting_vertex);
    printf("Parcurgere cu BFS: ");
    BFS(graph, starting_vertex);
    printf("\n");

    return 0;
}
