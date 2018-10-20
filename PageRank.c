#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEFAULTALLOC 1000000
#define ALPHA 0.15
#define ITERATION 20

typedef struct{
    unsigned long s;
    unsigned long e;
} link;

typedef struct{
    unsigned long max_node;
    unsigned long nb_links;
    unsigned long *douts;
    link *links;
} graph;

graph* readLinksFromFile(char* filename){
    graph *g = (graph *)malloc(sizeof(graph));
    g->max_node = 0;
    g->nb_links = 0;
    g->douts = NULL;
    g->links = (link *)malloc(DEFAULTALLOC*sizeof(link));
    FILE *f;
    f = fopen(filename, "r");

    unsigned long start, end;
    unsigned long toalloc = DEFAULTALLOC;

    while(fscanf(f, "%lu %lu\n", &start, &end) == 2){
        //printf("%lu %lu %lu %lu\n", start, end, currentsize, g->nb_links);
        if(start > g->max_node) g->max_node = start;
        if(end > g->max_node) g->max_node = end;
        (g->links[g->nb_links]).s = start;
        (g->links[g->nb_links]).e = end;
        
        g->nb_links++;
        if(g->nb_links >= toalloc){
            toalloc += DEFAULTALLOC;
            g->links = realloc(g->links, toalloc*sizeof(link));
        }
    }

    fclose(f);
    g->max_node++;
    g->links = realloc(g->links, g->nb_links*sizeof(link));


    g->douts = calloc(g->max_node, sizeof(unsigned long));
    for(int i = 0; i < g->nb_links; i++){
        g->douts[g->links[i].s]++;
    }

    return g;
}

double* pageRank(graph *g){
    double *p1, *p2, *p3;
    p1 = (double *)malloc(g->max_node*sizeof(double));
    p2 = (double *)calloc(g->max_node, sizeof(double));

    for(int i = 0; i < g->max_node; i++){
        p1[i] = 1./((double)g->max_node);
    }

    double normalize = 0;
    for(int i = 0; i < ITERATION; i++){
        
        free(p2);
        p2 = (double *)calloc(g->max_node, sizeof(double));
        for(int j = 0; j < g->nb_links; j++){
            if (g->douts[g->links[j].s]){
                p2[g->links[j].e] += (double)p1[g->links[j].s]/(double)g->douts[g->links[j].s];
            }
        }
        
        normalize = 0;
        for(int j = 0; j < g->max_node; j++){
            p2[j] *= 1.-(double)ALPHA;
            p2[j] += ALPHA/((double)g->max_node);
            normalize += p2[j];
        }
        
        normalize = (1.-(double)normalize)/((double)g->max_node);
        for(int j = 0; j < g->max_node; j++){
            p2[j] += normalize;
        }
        
        p3 = p2;
        p2 = p1;
        p1 = p3;
    }

    free(p2);
    return p1;
}



unsigned long* computeInDegrees(graph* g){
    unsigned long* indegrees = (unsigned long *)calloc(g->max_node, sizeof(unsigned long));
    for(int i = 0; i < g->nb_links; i++){
        indegrees[g->links[i].e]++;
    }
    return indegrees;
}

void writeFilePageRankResult(char* file, double *p, unsigned long size){
    FILE *f = fopen(file, "w");
    for(int i = 0; i < size; i++){
        if(p[i] > 0.0000009){
            fprintf(f, "%lu %.8lf\n", i, p[i]);
        }
    }
}


void writeFileDegrees(char* file, unsigned long* degrees, unsigned long size){
    FILE *f = fopen(file, "w");
    for(int i = 0; i < size; i++){
        fprintf(f,"%lu %lu\n", i, degrees[i]);
    }
    fclose(f);
}

int main(int argc, char** argv){
    graph *g = readLinksFromFile(argv[1]);
    //printf("Nblinks: %lu\n", g->nb_links);
    //printf("Nbvertices: %lu\n", g->max_node);
    clock_t t;
    t = clock();
    double *p = pageRank(g);
    t = clock() - t;
    //printf("%f seconds to execute.\n", ((double)t)/CLOCKS_PER_SEC);
    writeFilePageRankResult(argv[2], p, g->max_node);
    //writeFileDegrees(argv[3], g->douts, g->max_node);
    //writeFileDegrees(argv[2], computeInDegrees(g), g->max_node);
    return 0;
}

