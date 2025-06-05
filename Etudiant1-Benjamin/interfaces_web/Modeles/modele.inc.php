<?php

/**
 * @file modele.inc.php
 * @brief Fichier contenant les fonctions d'accès aux données pour la gestion des utilisateurs, classes et simulations.
 * @version 1.0
 * @author Benjamin BANDOU
 * @date 05/06/2025
 */

require_once __DIR__ . '/config.inc.php';

/**
 * @brief Établit une connexion PDO à la base de données.
 *
 * @details Utilise les constantes définies dans le fichier `config.inc.php` pour se connecter
 *          à la base MySQL. Définit l'encodage en UTF-8 et active les exceptions en cas d'erreur.
 *
 * @return PDO Objet de connexion à la base de données.
 */
function connexionBdd() {
    try {
        $dsn = 'mysql:host=' . SERVEUR_BDD . ';dbname=' . NOM_DE_LA_BASE;
        $bdd = new PDO($dsn, LOGIN, MOT_DE_PASSE);
        $bdd->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        $bdd->exec("set names utf8");
        return $bdd;
    } catch (PDOException $ex) {
        echo ('</br>Erreur de connexion au serveur BDD : ' . $ex->getMessage());
        die();
    }
}

/**
 * @brief Ajoute un nouvel utilisateur à la base de données.
 *
 * @param PDO $pdo Connexion à la base de données.
 * @param string $nom Nom de l'utilisateur.
 * @param string $prenom Prénom de l'utilisateur.
 * @param int $id_classes Identifiant de la classe associée.
 * 
 * @return bool True si l'insertion a réussi, False sinon.
 */
function ajouterUtilisateur(PDO $pdo, string $nom, string $prenom, int $id_classes): bool {
    $stmt = $pdo->prepare("INSERT INTO UTILISATEURS (nom, prenom, id_classes) VALUES (:nom, :prenom, :id_classes)");
    return $stmt->execute([
        'nom' => $nom,
        'prenom' => $prenom,
        'id_classes' => $id_classes
    ]);
}

/**
 * @brief Ajoute une nouvelle classe à la base de données.
 *
 * @param PDO $pdo Connexion à la base de données.
 * @param string $classe Nom de la classe à ajouter.
 * 
 * @return bool True si l'ajout a réussi, False sinon.
 */
function ajouterClasse(PDO $pdo, string $classe): bool {
    $stmt = $pdo->prepare("INSERT INTO CLASSES (nom_classe) VALUES (:classe)");
    return $stmt->execute(['classe' => $classe]);
}

/**
 * @brief Modifie les informations d'un utilisateur existant.
 *
 * @param PDO $pdo Connexion à la base de données.
 * @param int $id Identifiant de l'utilisateur à modifier.
 * @param string $nom Nouveau nom.
 * @param string $prenom Nouveau prénom.
 * @param int $id_classes Nouvel identifiant de classe.
 * 
 * @return bool True si la modification a réussi, False sinon.
 */
function modifierUtilisateur(PDO $pdo, int $id, string $nom, string $prenom, int $id_classes): bool {
    $stmt = $pdo->prepare("
        UPDATE UTILISATEURS
        SET nom = :nom, prenom = :prenom, id_classes = :id_classes
        WHERE id_utilisateur = :id
    ");
    return $stmt->execute([
        'id' => $id,
        'nom' => $nom,
        'prenom' => $prenom,
        'id_classes' => $id_classes
    ]);
}

/**
 * @brief Récupère l'historique des simulations avec détails utilisateur, classe, scénario, objectif et drone.
 *
 * @details Effectue plusieurs jointures entre les tables `SIMULATIONS`, `UTILISATEURS`, `CLASSES`, `SCENARIOS`,
 *          `OBJECTIFS` et `DRONES` pour construire un tableau complet des informations associées à chaque simulation.
 *
 * @param PDO $pdo Connexion à la base de données.
 * 
 * @return array Tableau contenant les simulations passées, chaque entrée ayant :
 *         - id_simulation : int
 *         - date : string (formaté `d/m/Y`)
 *         - utilisateur : string (prénom + nom)
 *         - classe : string
 *         - scenario : string (description du scénario)
 *         - objectif : string (type et nom)
 *         - drone : string (type de drone)
 */
function getHistoriqueSimulations(PDO $pdo): array {
    $sql = "SELECT 
                s.id_simulation,
                s.date,
                u.nom,
                u.prenom,
                c.nom_classe,
                sc.point_apparition,
                sc.vent,
                sc.pluie,
                sc.temperature,
                o.type_objectif,
                o.nom_objectif,
                d.type_drone
            FROM SIMULATIONS s
            LEFT JOIN UTILISATEURS u ON s.id_utilisateur = u.id_utilisateur
            LEFT JOIN CLASSES c ON u.id_classes = c.id_classes
            LEFT JOIN SCENARIOS sc ON s.id_simulation = sc.id_simulation
            LEFT JOIN OBJECTIFS o ON sc.id_scenario = o.id_scenario
            LEFT JOIN DRONES d ON sc.id_scenario = d.id_scenario
            ORDER BY s.date DESC";

    $stmt = $pdo->prepare($sql);
    $stmt->execute();

    $resultats = [];
    while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
        $resultats[] = [
            'id_simulation' => $row['id_simulation'],
            'date' => $row['date'] ? date('d/m/Y', strtotime($row['date'])) : 'Non définie',
            'utilisateur' => $row['prenom'] && $row['nom'] ? $row['prenom'] . ' ' . $row['nom'] : 'Non assigné',
            'classe' => $row['nom_classe'] ?? 'Non assignée',
            'scenario' => $row['point_apparition'] !== null
                ? 'Point : ' . ($row['point_apparition'] == '0' ? 'Extérieur' : $row['point_apparition']) .
                  ', Vent : ' . $row['vent'] . ' km/h, Pluie : ' . ($row['pluie'] ? 'Oui' : 'Non') .
                  ', Température : ' . $row['temperature'] . ' °C'
                : 'Aucun scénario',
            'objectif' => $row['type_objectif'] ? $row['type_objectif'] . ' - ' . $row['nom_objectif'] : 'Aucun',
            'drone' => $row['type_drone'] ?? 'Aucun'
        ];
    }

    return $resultats;
}
