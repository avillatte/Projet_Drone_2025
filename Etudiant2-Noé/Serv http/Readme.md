# Projet_Drone_2025

Ce code implémente une application serveur Qt qui combine plusieurs fonctionnalités : un serveur HTTP, un serveur TCP et une interaction avec une base de données MySQL. L'application permet de recevoir des requêtes HTTP, de traiter des commandes TCP, et de stocker des données dans une base de données.

Le serveur HTTP, géré par ***QHttpServer***, expose des endpoints pour récupérer des données (comme ***/classes*** et ***/objectifs***) et traite les requêtes GET en insérant les données reçues dans la base de données. Les données sont également transmises à un client TCP connecté si elles diffèrent significativement des données précédentes (vérifié via ***jsonPresqueEgal***).

Le serveur TCP, géré par ***QTcpServer***, écoute les connexions entrantes et permet d'envoyer des réponses JSON aux clients en fonction des commandes reçues (par exemple, ***GET_CLASSES*** ou ***GET_OBJECTIFS***).

La base de données MySQL stocke les informations dans trois tables principales : ***SIMULATIONS***, ***SCENARIOS***, et ***DRONES***. La fonction ***insererDansBaseDeDonnees*** valide et insère les données JSON reçues dans ces tables.

L'interface utilisateur, définie dans ***widget.ui***, comprend des champs pour configurer les ports HTTP et TCP, un bouton pour lancer les serveurs, et un journal (***QTextEdit***) pour afficher les logs.

En résumé, ce code sert de pont entre des clients HTTP/TCP et une base de données, tout en offrant une interface simple pour surveiller et contrôler le serveur.
