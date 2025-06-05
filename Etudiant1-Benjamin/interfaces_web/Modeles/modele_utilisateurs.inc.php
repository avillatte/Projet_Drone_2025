<?php

/**
 * @file modele_utilisateurs.inc.php
 * @brief Contient les fonctions PHP pour la gestion des utilisateurs et des classes (backend).
 * @version 1.0
 * @author Benjamin BANDOU
 * @date 05/06/2025
 */

require_once __DIR__ . '/modele.inc.php';

/**
 * @brief Récupère la liste complète des utilisateurs avec leurs informations de classe.
 *
 * @details Cette fonction établit une connexion à la base de données, exécute une requête SQL
 *          pour obtenir les informations des utilisateurs et leur classe associée, puis renvoie
 *          les résultats sous forme de tableau associatif.
 *
 * @return array Liste des utilisateurs. Chaque élément est un tableau associatif contenant :
 *         - id_utilisateur : int
 *         - nom : string
 *         - prenom : string
 *         - nom_classe : string|null
 */
function getAllUsers()
{
    try {
        $bdd = connexionBdd();

        $sql = "SELECT U.id_utilisateur, U.nom, U.prenom, C.nom_classe 
                FROM UTILISATEURS U
                LEFT JOIN CLASSES C ON U.id_classes = C.id_classes";
        $requete = $bdd->query($sql);

        $users = array();
        while ($ligne = $requete->fetch(PDO::FETCH_ASSOC)) {
            array_push($users, $ligne);
        }
        $requete->closeCursor();
        return $users;
    } catch (PDOException $exc) {
        print("Pb getAllUsers :" . $exc->getMessage());
        die();
    }
}

/**
 * @brief Récupère toutes les classes enregistrées dans la base de données.
 *
 * @param PDO $pdo Objet PDO déjà connecté à la base de données.
 * 
 * @return array Liste des classes, chaque élément contient :
 *         - id_classes : int
 *         - nom_classe : string
 */
function getClasses(PDO $pdo): array {
    $stmt = $pdo->query("SELECT id_classes, nom_classe FROM CLASSES");
    return $stmt->fetchAll(PDO::FETCH_ASSOC);
}

/**
 * @brief Supprime un utilisateur de la base de données.
 *
 * @param int $id Identifiant de l'utilisateur à supprimer.
 * 
 * @return array Retourne un tableau contenant :
 *         - success : bool (true si la suppression a réussi, false sinon)
 */
function delUser(int $id): array {
    $pdo = connexionBdd();
    $stmt = $pdo->prepare("DELETE FROM UTILISATEURS WHERE id_utilisateur = :id");
    $success = $stmt->execute(['id' => $id]);
    return ['success' => $success];
}
