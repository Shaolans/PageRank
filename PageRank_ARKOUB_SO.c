#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
 
typedef struct link {
     int source;
     int dest;
   
}Link;
 
typedef struct resultat{
    int ** res;
    Link ** adj;
   
}Resultat;
 
 
 
 
double * powerIteration(int *d_out, Link ** adj, double alpha, int n, int m, int nb_ite){
   
   
    double * P1 = calloc(n, sizeof(double));
    double * P2 = calloc(n, sizeof(double));
    double *P3;
   
    int i;
    int j;
    int s=0;
    int k=0;
    double sum=0;
 
    for(i=0; i<n; i++){
        P1[i] = 1/n;
    }
   
    for(i=0; i<nb_ite; i++){
       
        for(j=0; j<n; j++){
            P2[j] = 0;
        }
       
 
       
        for(j=0 ; j<m; j++){
           
            if(d_out[adj[j]->source]==0) continue;
            P2[adj[j]->dest] += P1[adj[j]->source]/(double)d_out[adj[j]->source];
                       
        }
       
       
       
        sum=0;
       
        for(j=0;  j<n; j++){
            P2[j] = (1.0-alpha)*P2[j];
            P2[j] += alpha/(double)n;
            sum += P2[j];
           
        }
       
       
       
        for(j=0; j<n; j++){
            P2[j] += (1.0-sum)/(double)n;
           
        }
       
        P3 = P2;
        P2 = P1;
        P1 = P3;
       
       
    }
   
    free(P2);
       
    return P1;
   
   
}
 
 Resultat formatFile2(char *fileLink, char* fileName){
     
     
     FILE *f = fopen(fileName, "r");
     
     FILE *l = fopen(fileLink, "r");
     
 
     
         
     int s=-1, t;
     char name[1000];
     char num[12];
     char reste[1000];
     int ** res = calloc(4, sizeof(int*));
     
     if(f==NULL) exit(1);
     if(l==NULL) exit(1);
     
     int i=0;
     
     while(i<5){
         fgets(name, 1000, f);
         fgets(name, 1000, l);
         i++;
    }
     i=0;
     int j;
     int source, dest;
     
     int sum=0;
     int p=0;
     
     int* d_out = calloc(13834640, sizeof(int));
     Link ** adj = calloc(46092178, sizeof(Link*));
     int* n = calloc(2, sizeof(int));
     
     
     Resultat ret ;
     
     ret.res = res;
     
     ret.adj = adj;
     
     int* correspondance = calloc(13834640, sizeof(int));
     int* correspondance2 = calloc(13834640, sizeof(int));
     
     int max=0;
     
     for(i =0; i<13834640; i++){
         correspondance[i] = -1;
     }
     
     res[0] = d_out;
     //res[1] = adj;
     res[2] = n;
     s = 12;
     correspondance[12] = 0;
     /*while(fgets(name, 1000, f)!=NULL){
         j=0;
         
         while(name[j] != '\t'){
             num[j] = name[j];
             j++;
         }
         
         
         int k =0;
         while(j<1000){
             reste[k++] = name[j];
             j++;
         }
         
         reste[k] = '\n';
         
         num[j] = '\n';
         
         s = atoi(num);*/
         
         i=0;
         
         correspondance2[0] = 12;
         
         Link* lien ;
 
         while(fscanf(l, "%d\t%d\n", &source, &dest)!=EOF){
           
            lien =(Link*) (malloc(sizeof(struct link)));           
           
             if(source==s){
               
               
                                             
                 d_out[i]++;
                 
                 adj[p++] = lien;
                 lien->source = s;
                 lien->dest = dest;
                 
             }
             else{
                 
                d_out[i+1] ++;
                adj[p++] = lien;
               
           
       
                s = source;
                i++;
                lien->source = s;
                lien->dest = dest;
               
                correspondance[source]=i;
                correspondance2[i] = source;
            }
             
         }
         
         
         
         int w;
         for(w=0; w<p; w++){
       
             if(correspondance[adj[w]->dest]==-1){
                 //printf("ICI %d\n",adj[w]);
                  s = adj[w]->source;
                adj[w]->dest = i++;
                correspondance2[i-1] = s;
                correspondance[adj[w]->dest] = i-1;
             }
             else{
                adj[w]->dest = correspondance[adj[w]->dest];
             }
         }
         
         
 
     
     n[0] = i;
     n[1] = p;
     
     fclose(f);
     
     fclose(l);
     
     res[3] = correspondance2;
     
     free(correspondance);
   
    return ret;
     
 }
 
 
  int * computeInDegrees(Link **adj, int n, int m){
     
     int * d_in = calloc(n, sizeof(double));
     
     int i;
     
     for(i=0; i<m; i++){
         d_in[adj[i]->dest]++;
     }
     
     
     return d_in;
     
 }
 
 void writeFilePageRankResult(char *file, double *P, int n){
     
     FILE *f = fopen(file, "w");
     
     int i;
     
     for(i=0; i<n; i++){
         if(P[i] > 0.0000009){
             fprintf(f, "%d %.81f\n", i, P[i]);
         }
                 
         
     }
     
     fclose(f);
     
     
 }
 
 
 void writeFileDegrees(char *file, int *deg, int n, int * correspondance){
     
    FILE *f = fopen(file, "w");
    int i;
   
     for(i=0; i<n; i++){
 
        fprintf(f, "%d %d\n", correspondance[i], deg[i]);
 
                 
         
     }
   
   
    fclose(f);
 }
 
 
int main(){
   
   
    time_t t1=time(NULL);
   
    Resultat ret = formatFile2("alr21--dirLinks--enwiki-20071018.txt", "alr21--pageNum2Name--enwiki-20071018.txt");
   
    int ** res = ret.res;
   
    int * d_out = res[0];
    Link ** adj = ret.adj;
   
    int n = res[2][0];
    int m = res[2][1];
   
    int * correspondance=res[3];
   
    int i;
   
   
   
    double * P = powerIteration(d_out, adj, 0.15, n, m, 20);
   
    time_t t2 = time(NULL);
   
    printf("Temps total %lds\n", (t2-t1)%60 );
   
    double max1=0, max2=0, max3=0, max4=0, max5= 0;
    int k1, k2, k3, k4, k5;
   
    double max1_tmp, max2_tmp, max3_tmp, max4_tmp, max5_tmp;
    int k1_tmp, k2_tmp, k3_tmp, k4_tmp, k5_tmp;
   
    double min1=1, min2=1, min3=1, min4=1, min5= 1;
    int p1, p2, p3, p4, p5;
   
    double min1_tmp, min2_tmp, min3_tmp, min4_tmp, min5_tmp;
    int p1_tmp, p2_tmp, p3_tmp, p4_tmp, p5_tmp;
   
   
   
   
   
    double sum = 0;
   
    for(i=0; i<n; i++){
        sum += P[i];
        max1_tmp = max1;
        max2_tmp = max2;
        max3_tmp = max3;
        max4_tmp = max4;
        max5_tmp = max5;
       
        k1_tmp = k1;
        k2_tmp = k2;
        k3_tmp = k3;
        k4_tmp = k4;
        k5_tmp = k5;
       
        min1_tmp = min1;
        min2_tmp = min2;
        min3_tmp = min3;
        min4_tmp = min4;
        min5_tmp = min5;
       
        p1_tmp = p1;
        p2_tmp = p2;
        p3_tmp = p3;
        p4_tmp = p4;
        p5_tmp = p5;
       
       
       
       
        if(max1 < P[i]){
           
            max1 = P[i];
            k1 = i;
           
            max2 = max1_tmp;
            k2 = k1_tmp;
           
            max3 = max2_tmp;
            k3 = k2_tmp;
           
            max4 = max3_tmp;
            k4 = k3_tmp;
           
            max5 = max4_tmp;
            k5 = k4_tmp;
 
        }
        else{
            if(max2 < P[i] ){
               
                max2 = P[i];
                k2 = i;
               
                max3 = max2_tmp;
                k3 = k2_tmp;
               
                max4 = max3_tmp;
                k4 = k3_tmp;
               
                max5 = max4_tmp;
                k5 = k4_tmp;
               
               
            }
            else{
                if(max3 < P[i]){
                    max3 = P[i];
                    k3 = i;
                   
                    max4 = max3_tmp;
                    k4 = k3_tmp;
                   
                    max5 = max4_tmp;
                    k5 = k4_tmp;
                   
                }
                else{
                    if(max4 < P[i]){
                        max4 = P[i];
                        k4 = i;
                       
                        max5 = max4_tmp;
                        k5 = k4_tmp;
                    }
                    else{
                        if(max5 < P[i]){
                           
                            max5 = P[i];
                            k5 = i;
                           
                        }
                       
                    }
                   
                }
               
            }
           
        }
       
       
        if(min1 > P[i] ){
            min1 = P[i];
            p1 = i;
           
            min2 = min1_tmp;
            p2 = p1_tmp;
           
            min3 = min2_tmp;
            p3 = p2_tmp;
           
            min4 = min3_tmp;
            p4 = p3_tmp;
           
            min5 = min4_tmp;
            p5 = p4_tmp;
           
        }
        else{
            if(min2 > P[i]){
               
                min2 = P[i];
                p2 = i;
               
                min3 = min2_tmp;
                p3 = p2_tmp;
               
                min4 = min3_tmp;
                p4 = p3_tmp;
               
                min5 = min4_tmp;
                p5 = p4_tmp;
               
            }
            else{
               
                if(min3 > P[i]){
                       
                    min3 = P[i];
                    p3 = i;
                   
                    min4 = min3_tmp;
                    p4 = p3_tmp;
                   
                    min5 = min4_tmp;
                    p5 = p4_tmp;
                   
                }
                else{
                    if(min4 > P[i]){
                       
                        min4 = P[i];
                        p4 = i;
                   
                        min5 = min4_tmp;
                        p5 = p4_tmp;
                       
                    }
                    else{
                        if(min5 > P[i]){
                            min5 = P[i];
                            p5 = i;
                           
                        }
                       
                    }
                   
                }
               
               
            }
           
        }
       
       
 
    }
   
    printf("MAX :\n");
   
    printf("%.20f\t%d\n", max1, correspondance[k1]);
    printf("%.20f\t%d\n", max2, correspondance[k2]);
    printf("%.20f\t%d\n", max3, correspondance[k3]);
    printf("%.20f\t%d\n", max4, correspondance[k4]);
    printf("%.20f\t%d\n", max5, correspondance[k5]);
   
    printf("\nMIN :\n");
   
    printf("%.20f\t%d\n", min1, correspondance[p1]);
    printf("%.20f\t%d\n", min2, correspondance[p2]);
    printf("%.20f\t%d\n", min3, correspondance[p3]);
    printf("%.20f\t%d\n", min4, correspondance[p4]);
    printf("%.20f\t%d\n", min5, correspondance[p5]);
   
   
    printf("Somme %f\n", sum);
   
    fflush(NULL);
   
    int *d_in = computeInDegrees(adj, n, m);
   
    /*writeFilePageRankResult("pr_15.txt", P, n);
    writeFileDegrees("deg_out_15.txt", d_out, n, correspondance);
    writeFileDegrees("deg_in_15.txt", d_in, n, correspondance);*/
   
    /*free(P);
   
    free(d_out);
    free(adj);
    free(res[2]);*/
   
    return 0;
}
