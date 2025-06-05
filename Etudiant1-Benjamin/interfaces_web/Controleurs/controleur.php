<?php

/**
 * @file controleur.php
 * @brief Contrôleur AJAX léger pour les opérations liées aux utilisateurs.
 * 
 * Ce script répond aux requêtes GET envoyées depuis le client.
 * Il appelle les fonctions du modèle pour retourner des données JSON.
 * 
 * @version 1.0
 * @author Benjamin BANDOU
 * @date 05/06/2025
 */

require_once __DIR__ . '/../Modeles/modele.inc.php'; ///< Inclusion du fichier modèle contenant la logique d'accès aux données

/**
 * Traitement des requêtes GET envoyées par le client.
 */
if (filter_input(INPUT_SERVER, 'REQUEST_METHOD') == 'GET') {
    /**
     * @var string|null $action Action à exécuter, récupérée via les paramètres d'URL.
     */
    $action = filter_input(INPUT_GET, "action");

    switch ($action) {
        case 'obtenirUtilisateurs':
            // Récupère la liste des utilisateurs et la renvoie au format JSON
            header('Content-Type: application/json');
            echo json_encode(obtenirUtilisateurs());
            break;

        default:
            // Action non reconnue ou non implémentée
            echo json_encode(["error" => "Action non reconnue"]);
            break;
    }
}
