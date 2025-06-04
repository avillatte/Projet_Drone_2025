#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "qnetworkaccessmanager.h"
#include <QObject>
#include <QSocketNotifier>
#include <QTimer>

// Classe Joystick : gère la lecture des événements joystick et l’envoi des données
class Joystick : public QObject
{
    Q_OBJECT

public:
    explicit Joystick(QObject *parent = nullptr); // Constructeur
    ~Joystick();                                  // Destructeur

    // Méthodes accessibles depuis l’extérieur (pas implémentées dans le .cpp ici)
    void setRestartClicked(bool clicked);
    void setRthDistance(float distance);

private slots:
    void lireEvenement();     // Lit un événement provenant du joystick
    void envoyerRequete();    // Envoie une requête HTTP avec les données joystick

private:
    int fd;                               // Descripteur de fichier pour le joystick
    QSocketNotifier *notifier;           // Notifie quand le joystick est prêt à lire
    QNetworkAccessManager *networkManager; // Pour envoyer des requêtes HTTP
    QTimer *sendTimer;                   // Timer pour les envois périodiques

    // Valeurs lues depuis le joystick
    int gazValue = 0;
    int lacetValue = 0;
    int tangageValue = 0;
    int roulisValue = 0;
};

#endif // JOYSTICK_H
