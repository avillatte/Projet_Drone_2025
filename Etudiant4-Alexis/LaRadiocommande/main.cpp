#include <QCoreApplication>
#include "radiocommande.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv); // Création de l'application Qt (mode console)
    Joystick joystick;              // Instanciation du joystick (déclenche lecture + requêtes)
    return a.exec();                // Lancement de la boucle d’événements Qt
}
