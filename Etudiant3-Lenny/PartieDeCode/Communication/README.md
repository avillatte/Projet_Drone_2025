# Dossier Étudiant3-Lenny

Ce dossier contient les captures d’écran prises à partir du code Unreal Engine, dans le cadre du **Projet Drone 2025**.

## Captures d’écran – Code Unreal Engine

- `Connexion au serveur.png` : Cette extrait de code gère la connexion à un serveur TCP pour recevoir des messages. Il établit une connexion à l'adresse IP 172.18.59.133 sur le port 8081. Lorsqu'un message est reçu, il est lu, transformé en JSON, et sa longueur est vérifiée (via "String Length"). Les données JSON sont ensuite utilisées pour extraire des paramètres dans les autres extraits de code.

- `Traitement des valeurs de gaz (1).png` & `Traitement des valeurs de gaz (2).png` : Ces deux extraits de code montrent la réception des messages et leur traitement lorsqu’ils contiennent une valeur de gaz. On y voit également que du son est généré lorsque le drone vole. Le son dépend de la valeur de gaz et de l’altitude du drone. Si la valeur du gaz est inférieur à 0.4, que l'altitude est inférieur à 1500, et que le son "Audio" qui signifie que le drone est déja en vol est activé, alors le son "Audio" s'arrète grâce au noeud "Stop" avec la target "Audio" et le son "Audio Fin" qui signifie que le drone est en train d'atterrir est activé grâce au noeud "Play" avec la target "Audio Fin". Enfin la valeur de gaz va agir sur l'axe Z d'un vecteur que l'on va créer avec la valeur de gaz que l'on recois et que l'on a extrait avec le noeud "Get Field" et pour créer le vecteur à partir de cette valeur nous allons utiliser le noeud "Make Vector". Et enfin on utilise le noeud "Add Local Offset" avec pour target "Drone" ce qui va permettre de faire monter le drone de la valeur du gaz en agissant sur l'axe Z du drone. 

- `Traitement des valeurs (roulis, lacet, tangage).png` : Cet extrait de code Unreal Engine illustre le traitement des messages reçus via une requête JSON, qui contient des valeurs concernant les axes de rotation du drone, donc le roulis (X), le lacet (Z) et le tangage (Y), ou une combinaison de ces axes. Tout d'abord on extrait les valeurs "Roulis", "Tangage" et "Lacet" à l'aide de noeuds "Get Field", puis les applique via un noeud "Make Rotator" pour construire une rotation basée sur ces paramètres. Une interpolation fluide est ensuite réalisée avec "RInterp To" pour ajuster la rotation actuelle du drone (via "Add Local Rotation"), en utilisant un temps delta et une vitesse de 0.5 pour une transition progressive. Enfin, les anciennes valeurs sont enregistrées et mises à jour avec des nœuds "SET".
  
- `Définition du scénario et de l’assistanat du drone.png` : Cet extrait met en évidence deux types de messages : d’une part, la définition du scénario de simulation (intérieur ou extérieur) si la valeur de "depart" est "interieur" alors l'apprenti se verra piloter le drone en intérieur en revanche si la valeur de "depart" est autre alors l'apprenti pilotera le drone en extérieur.D’autre part, ce blueprint active un mode d'assistance pour le drone si le champ "assiste" dans une requête JSON est à 1. Si c'est le cas, il ajuste les paramètres de "Linear Damping" et "Angular Damping" du drone pour faciliter son contrôle, avec des valeurs définies (4.0 et 6.0). Sinon, il désactive l'assistance en mettant ces valeurs à 0.

- `Début création timer.png` : Ce blueprint initialise un timer pour limiter le contrôle du drone. Il récupère la "Durée" depuis une requête JSON et l'utilise pour définir la durée du timer. Il crée un widget "WP Timer" et l'ajoute à l'interface utilisateur. Si la durée est supérieure à 0, il désactive la mobilité du drone (via "Set Mobility Component") pour empêcher tout contrôle une fois le timer écoulé.

- `RTH.png` : Cette extrait de code gère le "Return to Home" (RTH) donc le fait de revenir a un point visible pour le drone. Il utilise une requête JSON pour vérifier si le champ "RTH" est à 1. Si c'est le cas, il positionne le drone à des coordonnées prédéfinies (X: 2150, Y: 2150, Z: 240) avec l'option "Sweep Hit Result" pour éviter les collisions, et désactive la téléportation.

- `Traitement valeur vent.png` : Ce code gère les effets du vent lorsque la simulation se déroule en extérieur, si le formateur choisit d’activer du vent pour la simulation. Si la valeur de "Vent" est supérieur ou égale à 1 alors le vent est activé via un "Niagara Particle System Component" et une force exercer sur le drone en fonction de la valeur du vent qui a était recu.

- `Traitement valeur pluie.png` : Cet extrait de code active un effet de pluie si la valeur du champ "Pluie" dans une requête JSON est 1. Il compare la valeur récupérée : si elle est égale à 1, il active la pluie via un "Niagara Particle System Component". Si la valeur de la pluie est autre que 1 alors l'effet de pluie n'est pas activé.
---

## Notes

Ces images contiennent des extraits de code développés avec Unreal Engine.

---

## Auteur

**Lenny** – Étudiant 3, Projet Drone 2025
