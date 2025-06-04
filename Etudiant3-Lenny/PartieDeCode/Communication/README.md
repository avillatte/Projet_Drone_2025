# Dossier Étudiant3-Lenny

Ce dossier contient les captures d’écran prises à partir du code Unreal Engine, dans le cadre du **Projet Drone 2025**.

## Captures d’écran – Code Unreal Engine

- `Connexion au serveur.png` : Ce code permet à Unreal Engine de se connecter au serveur TCP situé à l'adresse IP `172.18.59.133` et au port `8081`.

- `Traitement des valeurs de gaz (1).png` & `Traitement des valeurs de gaz (2).png` : Ces deux extraits de code montrent la réception des messages et leur traitement lorsqu’ils contiennent une valeur de gaz. On y voit également que du son est généré lorsque le drone vole. Le son dépend de la valeur de gaz et de l’altitude du drone.

- `Traitement des valeurs (roulis, lacet, tangage).png` : Cet extrait de code montre le traitement des messages contenant des valeurs affectant les axes de rotation du drone, c’est-à-dire le roulis, le lacet ou le tangage (ou plusieurs de ces axes).

- `Définition du scénario et de l’assistanat du drone.png` : Cet extrait met en évidence deux types de messages : d’une part, la définition du scénario de simulation (intérieur ou extérieur), et d’autre part, la configuration du pilotage assisté ou non assisté.

- `Début création timer.png` : Cette capture montre le début du code lié à la création du minuteur (timer), utilisé pour afficher la durée de simulation à l’écran.

- `RTH.png` : Cet extrait de code montre que, dans la simulation mobile, il est possible de revenir à un point visible lorsque le drone sort du champ de vision.

- `Traitement valeur vent.png` : Ce code gère les effets du vent lorsque la simulation se déroule en extérieur, si le formateur choisit d’activer cette condition.

- `Traitement valeur pluie.png` : Ce code gère les effets de la pluie dans les mêmes conditions, lorsque la simulation extérieure intègre cette météo simulée.

---

## Notes

Ces images contiennent des extraits de code développés avec Unreal Engine.

---

## Auteur

**Lenny** – Étudiant 3, Projet Drone 2025
