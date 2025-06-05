<!DOCTYPE html>
<html>
<head>
    <title>Authentification</title>

    <!-- @brief Encodage et responsive design -->
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">   
    
    <!-- @brief Inclusion de Bootstrap pour la mise en page -->
    <link href="libs/bootstrap/css/bootstrap.min.css" rel="stylesheet" type="text/css"/>

    <!-- @brief Scripts nécessaires : Bootstrap JS, jQuery, et script local -->
    <script src="libs/bootstrap/js/bootstrap.min.js"></script>
    <script src="libs/jquery/jquery.min.js"></script>        
    <script src="auth.js"></script> <!-- @see auth.js : contient la logique de soumission et validation -->
</head>
<body>

    <!-- @section Conteneur principal centré pour la page de connexion -->
    <div class="container mt-5">
        <div class="row justify-content-center">
            <div class="col-md-6">

                <!-- @section Bouton de bascule d’affichage du formulaire -->
                <div class="text-center mb-4">
                    <!-- @brief Bouton qui affiche le formulaire de connexion -->
                    <button id="showLogin" class="btn btn-primary">Connexion</button>
                </div>

                <!-- @section Formulaire de connexion -->
                <div id="loginForm">
                    <h3 class="text-center">Connexion</h3>

                    <!-- 
                        @form Formulaire de connexion
                        @field loginPseudo Pseudo de l'utilisateur
                        @field loginPassword Mot de passe de l'utilisateur
                        @button Bouton pour soumettre le formulaire
                    -->
                    <form id="formLogin">
                        <div class="mb-3">
                            <label for="loginPseudo" class="form-label">Pseudo</label>
                            <input type="text" class="form-control" id="loginPseudo" required>
                        </div>
                        <div class="mb-3">
                            <label for="loginPassword" class="form-label">Mot de passe</label>
                            <input type="password" class="form-control" id="loginPassword" required>
                        </div>
                        <div class="d-grid">
                            <button type="submit" class="btn btn-success">Se connecter</button>
                        </div>
                    </form>
                </div>

            </div>
        </div>
    </div>

</body>
</html>
