<?php
// ⚙️ Démarrage de la session PHP pour suivre les utilisateurs ou stocker des données temporaires
session_start();
?>

<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <title>Historique des Simulations</title>

    <!-- 🌐 Meta & Responsiveness -->
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <!-- 🎨 Feuilles de style CSS et librairies JS -->

    <!-- 🎨 Bootstrap 5 pour le style -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/css/bootstrap.min.css" rel="stylesheet" />

    <!-- ⚙️ Bootstrap 5 JS (bundle incluant Popper.js) -->
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/js/bootstrap.bundle.min.js"></script>

    <!-- 🔧 jQuery pour manipulation DOM et requêtes AJAX -->
    <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>

    <!-- 🔤 Icônes Bootstrap pour embellir l'interface -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap-icons@1.10.5/font/bootstrap-icons.css" rel="stylesheet" />

    <!-- 🧾 DataTables pour les tableaux dynamiques (tri, recherche, pagination) -->
    <link href="https://cdn.datatables.net/v/bs5/dt-1.13.6/datatables.min.css" rel="stylesheet" />
    <script src="https://cdn.datatables.net/v/bs5/dt-1.13.6/datatables.min.js"></script>

    <!-- 📜 Script JS local pour le comportement de la page -->
    <script src="vues.js"></script>
</head>

<body>
<!-- 🔝 Barre de navigation -->
<nav class="navbar navbar-expand-sm bg-dark navbar-dark">
    <ul class="navbar-nav">
        <!-- 🏠 Lien vers la page d'accueil -->
        <li class="nav-item"><a class="nav-link" href="acceuil.php">Accueil</a></li>
        <!-- 📜 Lien vers la page d'historique des simulations -->
        <li class="nav-item"><a class="nav-link" href="historique.php">Historique</a></li>
        <!-- 📊 Lien vers la page de statistiques -->
        <li class="nav-item"><a class="nav-link" href="stat.php">Statistiques</a></li>
        <!-- ➕ Lien pour ajouter un élève -->
        <li class="nav-item"><a class="nav-link" href="ajouter.php">Ajout d'élève</a></li>
        <!-- 🔐 Lien vers la page de connexion -->
        <li class="nav-item"><a class="nav-link" href="connexion.php">Connexion</a></li>
    </ul>
</nav>

<!-- 📄 Conteneur principal -->
<div class="container my-4">
    <h1>Historique des Simulations</h1>

    <!-- 📊 Tableau affichant les données historiques -->
    <table class="table table-bordered table-hover" id="historique-table" style="width: 100%;">
        <thead>
            <tr>
                <th>ID</th> <!-- 🆔 Identifiant de la simulation -->
                <th>Date</th> <!-- 📅 Date de la simulation -->
                <th>Utilisateur</th> <!-- 👤 Nom ou identifiant de l'utilisateur -->
                <th>Classe</th> <!-- 🏫 Classe de l'utilisateur -->
                <th>Scénario</th> <!-- 🗺️ Scénario choisi -->
                <th>Objectif</th> <!-- 🎯 Objectif de la simulation -->
                <th>Drone</th> <!-- 🚁 Type de drone utilisé -->
            </tr>
        </thead>
        <tbody>
            <!-- 🔄 Le contenu sera chargé dynamiquement par vues.js -->
        </tbody>
    </table>
</div>

</body>
</html>
