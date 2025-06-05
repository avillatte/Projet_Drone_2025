#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QHttpServer>
#include <QHttpServerResponder>
#include <QHttpServerRequest>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonArray>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


private slots:
    void onQTcpServer_NewConnection();
    void onQTcpSocket_Connected();
    void onQTcpSocket_Disconnected();
    void onQTcpSocket_ReadyRead();
    void onQTcpSocket_ErrorOccurred(QAbstractSocket::SocketError _socketError);
    bool insererDansBaseDeDonnees(const QJsonObject &json);
    void resetCommandes();

private:
    // Pointeur vers l'interface utilisateur générée par Qt Designer, permettant d'accéder aux widgets de l'UI
    Ui::Widget *ui;
    // Gestionnaire d'accès réseau pour effectuer des requêtes HTTP ou autres opérations réseau
    QNetworkAccessManager manager;
    // Serveur HTTP pour gérer les requêtes entrantes via le protocole HTTP
    QHttpServer serveurHttp;
    // Constante statique représentant la commande TCP pour récupérer les données des classes
    static const QString COMMAND_GET_CLASSES;
    // Constante statique représentant la commande TCP pour récupérer les données des objectifs
    static const QString COMMAND_GET_OBJECTIFS;
    // Méthode pour générer un QByteArray contenant les données JSON des classes depuis la base de données
    QByteArray getClassesJson();
    // Méthode pour générer un QByteArray contenant les données JSON des objectifs depuis la base de données
    QByteArray getObjectifsJson();
    // Méthode pour traiter les commandes TCP reçues du client, en fonction de leur type (ex. GET_CLASSES)
    void processTcpCommand(const QString &command);
    // Méthode pour gérer les requêtes HTTP reçues, en fonction de l'URL et de la méthode (GET, POST, etc.)
    QHttpServerResponse handleHttpRequest(const QHttpServerRequest &request);
    // Pointeur vers un serveur TCP qui écoute les connexions entrantes des clients
    QTcpServer *socketEcouteServeur;
    // Pointeur vers un socket TCP utilisé pour communiquer avec un client connecté
    QTcpSocket *socketDialogueClient;
    // Variable stockant la dernière commande ou données JSON reçues ou envoyées, sous forme de QByteArray
    QByteArray ancienneCommande;
    // Variable stockant les données JSON actuelles à traiter ou à envoyer, sous forme de QByteArray
    QByteArray jsonData;
    // Compteur de type long int pour suivre le nombre de commandes ou d'opérations effectuées
    long int cpt;
    // Objet représentant la connexion à la base de données MySQL pour exécuter des requêtes SQL
    QSqlDatabase db;
    // Pointeur vers un timer Qt utilisé pour déclencher des événements périodiques (ex. réinitialisation des commandes)
    QTimer *resetTimer;
};
#endif // WIDGET_H
