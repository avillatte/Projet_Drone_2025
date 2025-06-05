<?php

/**
 * @file config.inc.php
 * @brief Fichier de configuration pour les paramètres de connexion à la base de données.
 *
 * @version 1.0
 * @author Benjamin BANDOU
 * @date 05/06/2025
 *
 * @details Ce fichier définit les constantes nécessaires à l'établissement d'une connexion
 *          PDO vers la base de données MySQL utilisée dans l'application Projet_Drone.
 */

// Adresse IP du serveur de base de données MySQL.
define("SERVEUR_BDD", "172.18.58.8");

// Nom d'utilisateur pour la connexion à la base de données.
define("LOGIN", "snir");

// Mot de passe associé au nom d'utilisateur.
define("MOT_DE_PASSE", "snir");

// Nom de la base de données à utiliser.
define("NOM_DE_LA_BASE", "Projet_Drone");
