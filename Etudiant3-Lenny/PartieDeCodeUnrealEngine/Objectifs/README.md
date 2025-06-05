# Dossier Étudiant3-Lenny

Ce dossier contient les captures d’écran prises à partir du code Unreal Engine, dans le cadre du **Projet Drone 2025**.

## Captures d’écran – Code Unreal Engine

- `Atterissage (1).png` && `Atterissage (2).png`: Ces blueprints Unreal Engine gèrent l'objectif de l'atterrissage du drone : lorsqu'il reste 3 secondes sur une plateforme (détecté via "On Component Begin Overlap" et "On Component End Overlap"). Une fois que le drone est resté 3 secondes un widget "WP ObjReussi" est créé et affiché via "Create WP ObjReussi Widget" et "Add to Viewport", et un Timer de 5 secondes est créer pour que le message de réussite ne reste que le temps du timer à l'écran. Il y a également la suppression du widget avec "Remove from Parent" et la gestion du timer avec "Clear and Invalidate Timer by Handle". Ensuite pour confirmer la réussite de l'objectif de l'atterissage au serveur, il y a un sous-système REST pour construire un objet JSON avec "Construct Json Object", définir un champ "atterrissage" avec "Set Field", construire une requête JSON avec "Construct Json Request", et exécuter la requête avec "Execute Process Request", ce qui permet d'envoyer le message au serveur en attribuant l'adresse IP du serveur dans "Set URL".

- `Passage Anneaux.png` : Cet extrait de blueprint Unreal Engine détecte le passage d’un drone à travers des cerceaux en utilisant "On Component Begin Overlap" pour trois sphères distinctes (Sphere, Sphere1, Sphere2). À chaque détection, il effectue un "Cast to BP_TCP" pour valider l’objet, puis affiche un message "anneau passé" via "Print String" et incrémente un compteur "Cpt" avec un nœud d’addition.
  
- `Passage 3 Anneaux + Affichage (1).png` & `Passage 3 Anneaux + Affichage (2).png` : Grâce au compteur qui est dans l'extrait de code de l'image Passage Anneaux, ces deux extraits permettent que lorsque 3 anneaux on étaient franchi alors un widget est crée "Create WP ObjReussi Widget" et s'affiche "Add to Viewport" et reste affiché 5 secondes grâce au timer crée ensuite il est enlever grâce au node "Remove from Parent". Ensuite tout comme pour confirmer la réussite de l'objectif de l'atterissage au serveur, on va envoyer au serveur que l'objectif a été réussi avec un sous-système REST pour construire un objet JSON avec "Construct Json Object", définir un champ "anneau" avec "Set Field", construire une requête JSON avec "Construct Json Request", et exécuter la requête avec "Execute Process Request", ce qui permet d'envoyer le message au serveur en attribuant l'adresse IP du serveur dans "Set URL".
---

## Notes

Ces images contiennent des extraits de code développés avec Unreal Engine.

---

## Auteur

**Lenny** – Étudiant 3, Projet Drone 2025
