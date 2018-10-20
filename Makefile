all:
	gcc -w -O3 PageRank.c -o PageRank
	gcc -w -O3 PageRankP.c -o PageRankP

clean:
	rm -rdf PageRank