<?php

/**
 * @file controleur_utilisateur.php
 * @brief Contrôleur principal des actions AJAX (GET et POST) pour l'application Projet Drone.
 * 
 * Ce script reçoit des requêtes HTTP (GET ou POST) contenant une action,
 * et redirige vers les fonctions appropriées du modèle pour gérer les utilisateurs,
 * classes, historiques, etc. Toutes les réponses sont retournées au format JSON.
 *
 * @version 1.0
 * @author Benjamin BANDOU
 * @date 05/06/2025
 */

session_start(); ///< Démarre la session utilisateur

// Inclusion des modèles nécessaires
require_once __DIR__ . '/../Modeles/modele.inc.php';
require_once __DIR__ . '/../Modeles/modele_utilisateurs.inc.php';

// Récupère la méthode HTTP utilisée et définit le format de réponse
$method = $_SERVER['REQUEST_METHOD'];
header('Content-Type: application/json');

// Traitement des requêtes GET
if ($method === 'GET') {
    /**
     * @var string $action Action à exécuter, transmise via l'URL (ex: ?action=getUsers)
     */
    $action = filter_input(INPUT_GET, 'action', FILTER_SANITIZE_STRING);

    switch ($action) {
        case 'getUsers':
            // Récupère tous les utilisateurs
            echo json_encode(getAllUsers());
            break;

        case 'deleteUser':
            // Supprime un utilisateur à partir de son ID
            $id = filter_input(INPUT_GET, 'id', FILTER_SANITIZE_NUMBER_INT);
            echo json_encode(delUser($id));
            break;

        case 'getClasses':
            // Récupère la liste des classes disponibles
            $pdo = connexionBdd();
            echo json_encode(getClasses($pdo));
            break;

        case 'getHistorique':
            // Récupère l'historique des simulations
            $pdo = connexionBdd();
            echo json_encode(getHistoriqueSimulations($pdo));
            break;

        default:
            // Action non reconnue
            echo json_encode(['error' => "Action GET inconnue : $action"]);
    }

} elseif ($method === 'POST') {
    /**
     * @var string $action Action à exécuter, transmise via l'URL (GET même en POST)
     */
    $action = filter_input(INPUT_GET, 'action', FILTER_SANITIZE_STRING);

    switch ($action) {
        case 'ajouterClasse':
            // Ajoute une nouvelle classe
            $classe = filter_input(INPUT_POST, 'classe', FILTER_SANITIZE_SPECIAL_CHARS);
            if ($classe) {
                $pdo = connexionBdd();
                $resultat = ajouterClasse($pdo, $classe);
                echo json_encode(['success' => $resultat]);
            } else {
                echo json_encode(['success' => false, 'message' => 'Classe invalide']);
            }
            break;

        case 'ajouterUtilisateur':
            // Ajoute un nouvel utilisateur
            $nom = filter_input(INPUT_POST, 'nom', FILTER_SANITIZE_SPECIAL_CHARS);
            $prenom = filter_input(INPUT_POST, 'prenom', FILTER_SANITIZE_SPECIAL_CHARS);
            $id_classes = filter_input(INPUT_POST, 'id_classes', FILTER_VALIDATE_INT);

            if ($nom && $prenom && $id_classes) {
                $pdo = connexionBdd();
                $resultat = ajouterUtilisateur($pdo, $nom, $prenom, $id_classes);
                echo json_encode(['success' => $resultat]);
            } else {
                echo json_encode(['success' => false, 'message' => 'Données utilisateur invalides']);
            }
            break;

        case 'modifierUtilisateur':
            // Modifie les informations d'un utilisateur
            $id = filter_input(INPUT_POST, 'id_utilisateur', FILTER_VALIDATE_INT);
            $nom = filter_input(INPUT_POST, 'nom', FILTER_SANITIZE_SPECIAL_CHARS);
            $prenom = filter_input(INPUT_POST, 'prenom', FILTER_SANITIZE_SPECIAL_CHARS);
            $id_classes = filter_input(INPUT_POST, 'id_classes', FILTER_VALIDATE_INT);

            if ($id && $nom && $prenom && $id_classes) {
                $pdo = connexionBdd();
                $resultat = modifierUtilisateur($pdo, $id, $nom, $prenom, $id_classes);
                echo json_encode(['success' => $resultat]);
            } else {
                echo json_encode(['success' => false, 'message' => 'Données invalides pour la modification']);
            }
            break;

        case 'supprimerUtilisateur':
            // Supprime un utilisateur via méthode POST
            $id = filter_input(INPUT_POST, 'id_utilisateur', FILTER_VALIDATE_INT);

            if ($id) {
                $resultat = delUser($id);
                echo json_encode(['success' => $resultat['success']]);
            } else {
                echo json_encode(['success' => false, 'message' => 'ID utilisateur invalide']);
            }
            break;

        default:
            // Action POST inconnue
            echo json_encode(['error' => "Action POST inconnue : $action"]);
    }
}
