#include "radiocommande.h"
#include <QDebug>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QTimer>

// Définition des axes selon la numérotation du joystick Linux
#define AXE_TANGAGE 2
#define AXE_GAZ     1
#define AXE_ROULIS  0
#define AXE_LACET   5

// Constructeur de la classe Joystick
Joystick::Joystick(QObject *parent)
    : QObject(parent),
    fd(open("/dev/input/js0", O_RDONLY | O_NONBLOCK)), // Ouverture du fichier du joystick en lecture non-bloquante
    networkManager(new QNetworkAccessManager(this)),    // Gestionnaire réseau pour les requêtes HTTP
    sendTimer(new QTimer(this))                         // Timer pour l'envoi périodique des données
{
    // Vérifie si le fichier du joystick a été ouvert avec succès
    if (fd < 0) {
        qFatal("Impossible d'ouvrir /dev/input/js0 (permissions ou périphérique manquant)");
    }

    // Création d'un notifier pour être averti quand des données sont prêtes à être lues
    notifier = new QSocketNotifier(fd, QSocketNotifier::Read, this);
    connect(notifier, &QSocketNotifier::activated, this, &Joystick::lireEvenement);

    // Lancement du timer pour envoyer les données toutes les 200 ms
    connect(sendTimer, &QTimer::timeout, this, &Joystick::envoyerRequete);
    sendTimer->start(200);
}

// Destructeur : fermeture du fichier du joystick si ouvert
Joystick::~Joystick()
{
    if (fd >= 0) {
        close(fd);
    }
}

// Fonction appelée quand un événement est disponible sur le joystick
void Joystick::lireEvenement()
{
    struct js_event event;
    ssize_t bytes = read(fd, &event, sizeof(event)); // Lecture d'un événement depuis le périphérique

    // Vérifie que l'événement est complet et concerne un axe
    if (bytes == sizeof(event) && (event.type & JS_EVENT_AXIS)) {
        switch (event.number) {
        case AXE_TANGAGE:
            tangageValue = event.value / 250; // Mise à l'échelle du tangage
            break;
        case AXE_GAZ:
            gazValue = -event.value / 250;    // Inversion et mise à l'échelle du gaz
            break;
        case AXE_ROULIS:
            roulisValue = event.value / 250;
            break;
        case AXE_LACET:
            lacetValue = event.value / 250;
            break;
        default:
            break; // Ignore les axes non utilisés
        }
    }
}

// Fonction envoyant les valeurs de commande via une requête HTTP GET
void Joystick::envoyerRequete()
{
    QUrl url("http://172.18.58.98:8080/?"); // Adresse IP et port du serveur récepteur

    // Préparation des paramètres à inclure dans l’URL
    QUrlQuery query;
    query.addQueryItem("gaz", QString::number(gazValue / 50.0f, 'f', 2));
    query.addQueryItem("lacet", QString::number(lacetValue / 300.0f, 'f', 2));
    query.addQueryItem("tangage", QString::number(tangageValue / 300.0f, 'f', 2));
    query.addQueryItem("roulis", QString::number(roulisValue / 300.0f, 'f', 2));
    url.setQuery(query);

    // Création et envoi de la requête HTTP GET
    QNetworkRequest request(url);
    networkManager->get(request);

    // Décommenter pour afficher les valeurs envoyées
    // qDebug() << "Envoi de la requête:" << url.toString();
    // qDebug() << "Gaz:" << gazValue << "Lacet:" << lacetValue << "Tangage:" << tangageValue << "Roulis:" << roulisValue;
}
