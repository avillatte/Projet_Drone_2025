<!DOCTYPE html>
<html lang="fr">
<head>
    <!-- Encodage et titre -->
    <meta charset="UTF-8" />
    <title>Liste des Utilisateurs</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" />

    <!-- Framework CSS Bootstrap -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/css/bootstrap.min.css" rel="stylesheet" />
    
    <!-- Script JS Bootstrap -->
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/js/bootstrap.bundle.min.js"></script>

    <!-- jQuery (nécessaire pour certaines interactions dynamiques) -->
    <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>

    <!-- Icônes Bootstrap -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap-icons@1.10.5/font/bootstrap-icons.css" rel="stylesheet" />

    <!-- Intégration de DataTables (tableaux dynamiques) -->
    <link href="https://cdn.datatables.net/v/bs5/dt-1.13.6/datatables.min.css" rel="stylesheet" />
    <script src="https://cdn.datatables.net/v/bs5/dt-1.13.6/datatables.min.js"></script>

    <!-- Style personnalisé pour les icônes de modification/suppression -->
    <style>
        .mod {
            color: blue;
            cursor: pointer;
        }
        .supp {
            color: red;
            cursor: pointer;
        }
    </style>
</head>
<body>

<!-- Barre de navigation principale -->
<nav class="navbar navbar-expand-sm bg-dark navbar-dark">
    <ul class="navbar-nav">
        <li class="nav-item"><a class="nav-link" href="accueil.php">Accueil</a></li>
        <li class="nav-item"><a class="nav-link" href="historique.php">Historique</a></li>
        <li class="nav-item"><a class="nav-link" href="stat.php">Statistiques</a></li>
        <li class="nav-item"><a class="nav-link" href="ajouter.php">Ajout élève</a></li>
        <li class="nav-item"><a class="nav-link" href="connection.php">Connexion</a></li>
    </ul>
</nav>

<!-- Contenu principal -->
<div class="container my-4">

    <!-- Conteneur pour messages d'alerte dynamiques -->
    <div id="alert-container"></div>

    <!-- En-tête avec titre et boutons d'action -->
    <header class="d-flex justify-content-between my-4">
        <h1>Liste des Utilisateurs</h1>
        <div>
            <!-- Bouton pour afficher la modale d'ajout utilisateur -->
            <button class="btn btn-primary me-2" id="btnAddUser" data-bs-toggle="modal" data-bs-target="#addUserModal">
                Ajouter un Utilisateur
            </button>
            <!-- Bouton pour afficher la modale d'ajout de classe -->
            <button class="btn btn-primary" id="btnAddClasse" data-bs-toggle="modal" data-bs-target="#addClasseModal">
                Ajouter une classe
            </button>
        </div>
    </header>

    <!-- Tableau affichant les utilisateurs avec DataTables -->
    <table id="user-table" class="table table-striped table-bordered text-center">
        <thead class="table-dark">
            <tr>
                <th>ID</th>
                <th>Nom</th>
                <th>Prénom</th>
                <th>Classe</th>
                <th>Actions</th>
            </tr>
        </thead>
        <tbody>
            <!-- Rempli dynamiquement via AJAX -->
        </tbody>
    </table>
</div>

<!-- Modale pour ajouter une classe -->
<div class="modal fade" id="addClasseModal" tabindex="-1" aria-labelledby="addClasseModalLabel" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title">Ajouter une classe</h5>
                <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Fermer"></button>
            </div>
            <div class="modal-body">
                <form id="addClasseForm" action="#">
                    <div class="mb-3">
                        <label for="classeAdd" class="form-label">Classe</label>
                        <input type="text" class="form-control" id="classeAdd" required />
                    </div>
                </form>
            </div>
            <div class="modal-footer">
                <button class="btn btn-secondary" data-bs-dismiss="modal">Annuler</button>
                <button class="btn btn-primary" id="ajout">Ajouter</button>
            </div>
        </div>
    </div>
</div>

<!-- Modale pour ajouter un utilisateur -->
<div class="modal fade" id="addUserModal" tabindex="-1" aria-labelledby="addUserModalLabel" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title">Ajouter un utilisateur</h5>
                <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Fermer"></button>
            </div>
            <div class="modal-body">
                <form id="addUserForm" action="#">
                    <div class="mb-3">
                        <label for="nomAdd" class="form-label">Nom</label>
                        <input type="text" class="form-control" id="nomAdd" required />
                    </div>
                    <div class="mb-3">
                        <label for="prenomAdd" class="form-label">Prénom</label>
                        <input type="text" class="form-control" id="prenomAdd" required />
                    </div>
                    <div class="mb-3">
                        <label for="classeSelect" class="form-label">Classe</label>
                        <select id="classeSelect" class="form-select" required>
                            <option value="">-- Sélectionnez une classe --</option>
                            <!-- Options dynamiques chargées via JS -->
                        </select>
                    </div>
                </form>
            </div>
            <div class="modal-footer">
                <button class="btn btn-secondary" data-bs-dismiss="modal">Annuler</button>
                <button class="btn btn-primary" id="ajoutUtilisateur">Ajouter</button>
            </div>
        </div>
    </div>
</div>

<!-- Modale pour modifier un utilisateur -->
<div class="modal fade" id="editUserModal" tabindex="-1" aria-labelledby="editUserModalLabel" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title">Modifier l'utilisateur</h5>
                <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Fermer"></button>
            </div>
            <div class="modal-body">
                <form id="editUserForm" action="#">
                    <!-- Champ caché pour stocker l'ID -->
                    <input type="hidden" id="editUserId" />
                    <div class="mb-3">
                        <label for="editNom" class="form-label">Nom</label>
                        <input type="text" class="form-control" id="editNom" required />
                    </div>
                    <div class="mb-3">
                        <label for="editPrenom" class="form-label">Prénom</label>
                        <input type="text" class="form-control" id="editPrenom" required />
                    </div>
                    <div class="mb-3">
                        <label for="editClasse" class="form-label">Classe</label>
                        <select id="editClasse" class="form-select" required>
                            <option value="">-- Sélectionnez une classe --</option>
                            <!-- Options dynamiques via JS -->
                        </select>
                    </div>
                </form>
            </div>
            <div class="modal-footer">
                <button class="btn btn-secondary" data-bs-dismiss="modal">Annuler</button>
                <button class="btn btn-primary" id="modifierUtilisateur">Enregistrer</button>
            </div>
        </div>
    </div>
</div>

<!-- Modale de confirmation de suppression -->
<div class="modal fade" id="deleteConfirmModal" tabindex="-1" aria-labelledby="deleteConfirmLabel" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content rounded-3">
            <div class="modal-header bg-danger text-white">
                <h5 class="modal-title">Confirmer la suppression</h5>
                <button type="button" class="btn-close btn-close-white" data-bs-dismiss="modal" aria-label="Fermer"></button>
            </div>
            <div class="modal-body">
                Êtes-vous sûr de vouloir supprimer cet utilisateur ?
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Annuler</button>
                <button type="button" class="btn btn-danger" id="confirmDeleteBtn">Supprimer</button>
            </div>
        </div>
    </div>
</div>

<!-- Script JS spécifique pour gérer les actions (AJAX, événements, etc.) -->
<script src="vues.js"></script>
</body>
</html>
