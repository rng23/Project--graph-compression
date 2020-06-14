-) pour visualiser la decomposition en communautés, il suffit d'executer à
la console la commande suivante, et lui passer le fichier .out engendré
par votre code C++:

java -jar CommunityViewer.jar ca-AstroPh_communities.out

Attention: il faut juste une communauté par ligne.

-) decomposition en commuanutés avec l'algo de Louvain. Pour l'executer ils fat deux paramètres:

-) fichier du graphe (.edges, format de Konect)
-) valeur de la resolution (une valeur réelle: par exemple 0.5, 0.1, 0.01,...)

Un exemple:
java -jar ComputeCommunities.jar ca-AstroPh.edges 0.01