/* Determinati daca exista sau nu drum direct intre doua restaurante dintr-o retea de tip graf */
#include <stdlib.h>
#include <stdio.h>

// Structura pentru un nod din lista de adiacenta (reprezentand un restaurant/locatie)
typedef struct Node {
    int data;            // Restaurantul/locatia (identificat printr-un numar intreg)
    struct Node *next;   // Pointer la urmatorul nod
} NODE;

// Structura pentru graf (reprezentarea grafului cu lista de adiacenta)
typedef struct g {
    int vertex_number;   // Numarul de noduri (restaurante)
    int *visited;        // Vector de vizitate pentru a marca nodurile deja vizitate
    struct Node **lista_adj; // Lista de adiacenta (reprezentand legaturile dintre restaurante)
} GPH;

// Structura pentru stiva (folosita la DFS)
typedef struct s {
    int top;             // Indicele de varf al stivei
    int capacity;        // Capacitatea stivei
    int *arr;            // Vectorul care stocheaza elementele stivei
} STK;

// Creaza un nod nou pentru lista de adiacenta
NODE *create_node(int value) {
    NODE *new_node = malloc(sizeof(NODE));   // Alocam memorie pentru un nod
    new_node->data = value;                  // Setam valoarea pentru nod
    new_node->next = NULL;                    // Pointeurul la urmatorul nod este NULL
    return new_node;
}

// Adauga o muchie (legatura) intre doua noduri in graf
void add_edge(GPH *graph, int src, int dest) {
    NODE *new_node = create_node(dest);           // Creaza nodul pentru destinatie
    new_node->next = graph->lista_adj[src];       // Adauga nodul la lista de adiacenta a sursei
    graph->lista_adj[src] = new_node;             // Actualizeaza lista de adiacenta pentru sursa
    
    new_node = create_node(src);                  // Creaza nodul pentru sursa
    new_node->next = graph->lista_adj[dest];      // Adauga nodul la lista de adiacenta a destinatiei
    graph->lista_adj[dest] = new_node;            // Actualizeaza lista de adiacenta pentru destinatie
}

// Creaza un graf cu un numar specificat de noduri
GPH *create_graph(int vertex_number) {
    int i;
    GPH *graph = malloc(sizeof(GPH));                 // Alocam memorie pentru graf
    graph->vertex_number = vertex_number;             // Setam numarul de noduri
    graph->lista_adj = malloc(sizeof(NODE *) * vertex_number); // Alocam memorie pentru listele de adiacenta
    graph->visited = malloc(sizeof(int) * vertex_number);        // Alocam memorie pentru vectorul de vizitate
    
    for(i = 0; i < vertex_number; i++) {
        graph->lista_adj[i] = NULL;   // Initializam listele de adiacenta cu NULL
        graph->visited[i] = 0;        // Initializam vectorul de vizite cu 0 (nevizitat)
    }   
    return graph;
}

// Creaza o stiva pentru DFS
STK *create_stack(int capacity) {
    STK *stack = malloc(sizeof(STK));              // Alocam memorie pentru stiva
    stack->arr = malloc(capacity * sizeof(int));    // Alocam memorie pentru elementele stivei
    stack->top = -1;                               // Initializam varful stivei
    stack->capacity = capacity;                    // Setam capacitatea stivei
    return stack;
}

// Adauga un element in stiva
void push(int value, STK *stack) {
    stack->top++;                        // Incrementam varful stivei
    stack->arr[stack->top] = value;      // Adaugam valoarea in stiva
}

// Algoritmul de cautare DFS (Depth-First Search)
void DFS(GPH *graph, STK *stack, int vertex_idx) {
    NODE *list_adj = graph->lista_adj[vertex_idx];  // Lista de adiacenta pentru nodul curent
    NODE *aux = list_adj;
    
    graph->visited[vertex_idx] = 1;          // Marcam nodul curent ca vizitat
    printf("%d ", vertex_idx);               // Afisam nodul curent
    
    push(vertex_idx, stack);                 // Adaugam nodul curent in stiva
    
    while (aux != NULL) {
        int connected_vertex = aux->data;    // Extragem nodul conectat
        
        if (graph->visited[connected_vertex] == 0) {  // Daca nu a fost vizitat
            DFS(graph, stack, connected_vertex);      // Apelam recursiv DFS pentru nodul conectat
        }
        
        aux = aux->next;    // Mergem la urmatorul nod din lista de adiacenta
    }
}

// Citeste muchiile si le adauga in graf
void insert_edges(GPH *graph, int edge_count) {
    int src, dest, i;
    printf("Adauga %d muchii (de la 0 la %d):\n", edge_count, graph->vertex_number - 1);
    for (i = 0; i < edge_count; i++) {
        scanf("%d%d", &src, &dest);    // Citim sursa si destinatia pentru fiecare muchie
        add_edge(graph, src, dest);     // Adaugam muchia in graf
    }
}

// Reseteaza vectorul de vizite la 0 pentru a putea face o noua cautare
void reset_visited(GPH *graph) {
    for (int i = 0; i < graph->vertex_number; i++) {
        graph->visited[i] = 0;  // Marcam toate nodurile ca nevizitate
    }
}

// Verifica daca exista drum intre doua restaurante
void can_reach(GPH *graph, STK *stack1, STK *stack2, int start, int end) {
    DFS(graph, stack1, start);    // Aplicam DFS pentru restaurantul de inceput
    reset_visited(graph);         // Resetam vizitele
    DFS(graph, stack2, end);      // Aplicam DFS pentru restaurantul de sfarsit

    // Verificam daca exista un drum direct
    for (int i = 0; i < graph->vertex_number; i++) {
        if (stack1->arr[i] == end && stack2->arr[i] == start) {
            printf("\nDrum direct intre restaurantele %d si %d exista.\n", start, end);
            return;
        }
    }
    printf("\nNu exista drum direct intre restaurantele %d si %d.\n", start, end);
}

int main() {
    int vertex_number;
    int edge_count;
    int start, end;
    
    // Citim numarul de noduri si muchii
    printf("Cate noduri sunt in graf?");
    scanf("%d", &vertex_number);
    
    printf("Cate muchii are graful?");
    scanf("%d", &edge_count);
    
    // Creem graful si stivele pentru DFS
    GPH *graph = create_graph(vertex_number);
    STK *stack1 = create_stack(2 * vertex_number);
    STK *stack2 = create_stack(2 * vertex_number);
    
    insert_edges(graph, edge_count);  // Citim muchiile si le adaugam in graf

    // Citim restaurantele de inceput si sfarsit
    printf("Introduceti restaurantul de inceput si restaurantul de sfarsit: ");
    scanf("%d%d", &start, &end);

    can_reach(graph, stack1, stack2, start, end);  // Verificam daca exista drum direct

    return 0;
}
