<!DOCTYPE html>
<html lang="fr">
<head>
    <!-- Métadonnées principales -->
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Accueil - Simulateur de Drone</title>
    
    <!-- Feuilles de style externes -->
    
    <!-- Bootstrap CSS -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
    
    <!-- FontAwesome pour les icônes -->
    <script src="https://kit.fontawesome.com/b99e675b6e.js" crossorigin="anonymous"></script>

    <!-- Bootstrap Icons (alternative à FontAwesome) -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap-icons@1.10.5/font/bootstrap-icons.css" rel="stylesheet" />
    
    <!-- Styles personnalisés -->
    <style>
        .hero {
            padding: 60px 20px;
            text-align: center;
            background: linear-gradient(to right, #007bff, #6610f2);
            color: white;
            border-radius: 12px;
            margin-top: 30px;
        }
        .accueil-buttons {
            margin-top: 30px;
            display: flex;
            gap: 15px;
            flex-wrap: wrap;
            justify-content: center;
        }
    </style>
</head>
<body>

    <!-- Barre de navigation principale -->
    <nav class="navbar navbar-expand-sm bg-dark navbar-dark">
        <div class="container-fluid">
            <a class="navbar-brand" href="#">Simulateur de Drone</a>
            <ul class="navbar-nav">
                <li class="nav-item"><a class="nav-link active" href="acceuil.php">Accueil</a></li>
                <li class="nav-item"><a class="nav-link" href="test1.php">Historique des vols</a></li>
                <li class="nav-item"><a class="nav-link" href="stat.php">Statistiques</a></li>
                <li class="nav-item"><a class="nav-link" href="index.php">Ajout d'élève</a></li>
            </ul>
        </div>
    </nav>

    <!-- Contenu principal de la page -->
    <div class="container my-5">
        <!-- Section de bienvenue (hero) -->
        <div class="hero">
            <h1>Bienvenue sur le Simulateur de Drone</h1>
            <p class="lead">Apprenez à piloter un drone en toute sécurité et suivez votre progression.</p>
        </div>

        <!-- Bouton de connexion -->
        <div class="accueil-buttons">
            <a href="connexion.php" class="btn btn-secondary">
                <i class="fas fa-sign-in-alt"></i> Connexion
            </a>
        </div>
    </div>

    <!-- Scripts Bootstrap (JS Bundle) -->
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js"></script>
</body>
</html>
