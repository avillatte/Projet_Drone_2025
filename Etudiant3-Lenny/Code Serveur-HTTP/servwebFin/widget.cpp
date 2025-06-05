#include "widget.h"
#include "ui_widget.h"
#include <QJsonObject>
#include <QJsonDocument>

//ETU-2 Noé
// Constantes pour les commandes TCP
const QString Widget::COMMAND_GET_CLASSES = "GET_CLASSES";
const QString Widget::COMMAND_GET_OBJECTIFS = "GET_OBJECTIFS";

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , ancienneCommande(nullptr)
    , socketEcouteServeur(new QTcpServer(this))
    , socketDialogueClient(nullptr)
{
    ui->setupUi(this);
    cpt = 0;

    //ETU-3 Lenny
    // Configuration du serveur HTTP avec une route principale
    serveurHttp.route("/", [this](const QHttpServerRequest &request) {
        return handleHttpRequest(request);
    });

    //ETU-3 Lenny
    // Connexion du signal pour gérer les nouvelles connexions TCP
    connect(socketEcouteServeur, &QTcpServer::newConnection, this, &Widget::onQTcpServer_NewConnection);

    //ETU-2 Noé
    // Configuration de la base de données MySQL
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("172.18.58.7");
    db.setDatabaseName("Projet_Drone_test");
    db.setUserName("snir"); // identifiants base de données
    db.setPassword("snir"); // mot de passe base de données

    if (!db.open()) {
        qDebug() << "Erreur de connexion BDD :" << db.lastError().text();
    } else {
        //qDebug() << "Connexion BDD réussie !";
    }

    //ETU-3 Lenny
    // Timer pour réinitialiser les commandes toutes les heures
    QTimer *resetTimer = new QTimer(this);
    connect(resetTimer, &QTimer::timeout, this, &Widget::resetCommandes);
    resetTimer->start(360000); // 360000 ms = 1 heure

    //ETU-3 Lenny
    // Lancement du serveur HTTP sur le port 8080
    const quint16 portHttp = 8080;
    if (serveurHttp.listen(QHostAddress::Any, portHttp)) {
        qDebug() << "Serveur HTTP lancé automatiquement sur le port" << portHttp;
    } else {
        qDebug() << "Échec du lancement automatique du serveur HTTP sur le port" << portHttp;
    }

    //ETU-3 Lenny
    // Lancement du serveur TCP sur le port 8081
    const quint16 portTcp = 8081;
    if (socketEcouteServeur->listen(QHostAddress::Any, portTcp)) {
        qDebug() << "Serveur TCP lancé automatiquement sur le port" << portTcp;
    } else {
        qDebug() << "Échec du lancement automatique du serveur TCP sur le port" << portTcp;
    }
}
//ETU-3 Lenny
Widget::~Widget()
{
    delete ui; // Libération de l'interface utilisateur
}

void Widget::resetCommandes()
{
    cpt = 0; // Réinitialisation du compteur
    ancienneCommande.clear(); // Vidage des anciennes commandes
    ui->textEditLog->clear(); // Effacement du log
    ui->textEditLog->append("Remise à zéro des commandes.");
    //qDebug() << "Commandes réinitialisées après 1 heure.";
}

//ETU-2 Noé
bool Widget::insererDansBaseDeDonnees(const QJsonObject &json)
{
    if (!db.isOpen()) return false; // Vérification de la connexion à la BDD

    QSqlQuery query;

    // 1. Insertion dans la table SIMULATIONS
    bool dureeOk;
    int duree = json["Duree"].toString().toInt(&dureeOk);
    if (!dureeOk) {
        //qDebug() << "Erreur de conversion de la durée";
        return false;
    }
    query.prepare("INSERT INTO SIMULATIONS (date, duree, id_utilisateur) VALUES (CURRENT_DATE(), ?, ?)");
    query.addBindValue(duree);
    query.addBindValue(12); // ID utilisateur fixe, à adapter
    if (!query.exec()) {
        qDebug() << "Erreur SIMULATIONS:" << query.lastError().text();
        return false;
    }
    int id_simulation = query.lastInsertId().toInt();

    // 2. Insertion dans la table SCENARIOS avec conversion des données
    double temp = json["Celsius"].toString().toDouble();
    int pluie = json["Pluie"].toString().toInt();
    int vent = json["Vent"].toString().toInt();

    query.prepare("INSERT INTO SCENARIOS (point_apparition, vent, pluie, temperature, id_simulation) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(json["Scenario"].toString().trimmed());
    query.addBindValue(vent); // Vent brut
    query.addBindValue(pluie); // Pluie exacte
    query.addBindValue(temp); // Température
    query.addBindValue(id_simulation);
    if (!query.exec()) {
        qDebug() << "Erreur SCENARIOS:" << query.lastError().text();
        return false;
    }
    int id_scenario = query.lastInsertId().toInt();

    // 3. Insertion dans la table DRONES
    query.prepare("INSERT INTO DRONES (type_drone, description, id_scenario) VALUES (?, ?, ?)");
    query.addBindValue(json["Drone"].toString().trimmed());
    query.addBindValue("Drone reçu via JSON");
    query.addBindValue(id_scenario);
    if (!query.exec()) {
        qDebug() << "Erreur DRONES:" << query.lastError().text();
        return false;
    }

    return true;
}
//ETU-3 Lenny
void Widget::onQTcpServer_NewConnection()
{
    // Gestion d'une nouvelle connexion TCP
    QTcpSocket *newClient = socketEcouteServeur->nextPendingConnection();
    if (newClient) {
        qDebug() << "Client TCP connecté : " << newClient->peerAddress().toString() << ":" << newClient->peerPort();
        socketDialogueClient = newClient;

        connect(newClient, &QTcpSocket::connected, this, &Widget::onQTcpSocket_Connected);
        connect(newClient, &QTcpSocket::disconnected, this, &Widget::onQTcpSocket_Disconnected);
        connect(newClient, &QTcpSocket::readyRead, this, &Widget::onQTcpSocket_ReadyRead);
        connect(newClient, &QTcpSocket::errorOccurred, this, &Widget::onQTcpSocket_ErrorOccurred);
    } else {
        qDebug() << "Aucun client TCP connecté.";
    }
}
//ETU-3 Lenny
void Widget::onQTcpSocket_Connected()
{
    //qDebug() << "Connexion TCP établie.";
}
//ETU-3 Lenny
void Widget::onQTcpSocket_Disconnected()
{
    //qDebug() << "Client TCP déconnecté.";
}
//ETU-3 Lenny
void Widget::onQTcpSocket_ReadyRead()
{
    // Lecture des données reçues du client TCP
    QByteArray data = socketDialogueClient->readAll();
    QString command = QString::fromUtf8(data).trimmed();

    //qDebug() << "Commande TCP reçue : " << command;

    processTcpCommand(command);
}
//ETU-3 Lenny
void Widget::onQTcpSocket_ErrorOccurred(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Erreur sur le socket TCP : " << socketError;
}
//ETU-3 Lenny
bool jsonPresqueEgal(const QByteArray &a, const QByteArray &b, double delta)
{
    // Comparaison approximative de deux JSON avec une tolérance delta pour les nombres
    QJsonDocument docA = QJsonDocument::fromJson(a);
    QJsonDocument docB = QJsonDocument::fromJson(b);

    if (!docA.isObject() || !docB.isObject()) return false;

    QJsonObject objA = docA.object();
    QJsonObject objB = docB.object();

    if (objA.keys() != objB.keys()) return false;

    for (const QString &key : objA.keys()) {
        QJsonValue valA = objA.value(key);
        QJsonValue valB = objB.value(key);

        if (valA.isDouble() && valB.isDouble()) {
            if (qAbs(valA.toDouble() - valB.toDouble()) > delta) return false;
        } else {
            if (valA != valB) return false;
        }
    }
    return true;
}
//ETU-3 Lenny
QHttpServerResponse Widget::handleHttpRequest(const QHttpServerRequest &request)
{
    QUrl url = request.url();

    // Gestion des routes HTTP
    if (url.path() == "/classes" && request.method() == QHttpServerRequest::Method::Get) {
        return QHttpServerResponse(getClassesJson(), "application/json");
    }

    if (url.path() == "/objectifs" && request.method() == QHttpServerRequest::Method::Get) {
        return QHttpServerResponse(getObjectifsJson(), "application/json");
    }

    if (url.path() == "/" && request.method() == QHttpServerRequest::Method::Get) {
        QUrlQuery query(url);
        QJsonObject json;

        for (const auto &pair : query.queryItems()) {
            json.insert(pair.first, pair.second);
        }

        QJsonDocument jsonDoc(json);
        jsonData = jsonDoc.toJson(QJsonDocument::Compact);
        insererDansBaseDeDonnees(json);

        //qDebug() << "Requête GET reçue : " << jsonData;

        if (!jsonPresqueEgal(jsonData, ancienneCommande, 4.0)) {
            cpt++;
            ui->textEditLog->append(QString::number(cpt));
            if (socketDialogueClient && socketDialogueClient->isOpen()) {
                socketDialogueClient->write(jsonData + "\n");
                socketDialogueClient->flush();
                //qDebug() << "Données envoyées au client TCP.";
                ancienneCommande = jsonData;
            } else {
                //qDebug() << "Aucun client TCP connecté.";
            }
        } else {
            //qDebug() << "Commande similaire (delta <= 4), rien à envoyer.";
        }

        return QHttpServerResponse(jsonData, "application/json");
    }

    return QHttpServerResponse("Endpoint non trouvé", QHttpServerResponder::StatusCode::NotFound);
}
//ETU-2 Noé
QByteArray Widget::getClassesJson()
{
    // Récupération des classes depuis la base de données
    QJsonArray classesArray;
    QSqlQuery query("SELECT id_classes, nom_classe FROM CLASSES");

    while (query.next()) {
        QJsonObject classObj;
        classObj["id"] = query.value("id_classes").toInt();
        classObj["nom"] = query.value("nom_classe").toString();
        classesArray.append(classObj);
    }

    QJsonDocument doc(classesArray);
    return doc.toJson(QJsonDocument::Compact);
}
//ETU-2 Noé
QByteArray Widget::getObjectifsJson()
{
    // Récupération des objectifs depuis la base de données
    QJsonArray objectifsArray;
    QSqlQuery query("SELECT id_objectif, type_objectif, nom_objectif, description, id_scenario FROM OBJECTIFS");

    while (query.next()) {
        QJsonObject obj;
        obj["id"] = query.value("id_objectif").toInt();
        obj["type"] = query.value("type_objectif").toString();
        obj["nom"] = query.value("nom_objectif").toString();
        obj["description"] = query.value("description").toString();
        obj["id_scenario"] = query.value("id_scenario").toInt();
        objectifsArray.append(obj);
    }

    QJsonDocument doc(objectifsArray);
    return doc.toJson(QJsonDocument::Compact);
}
//ETU-2 Noé
void Widget::processTcpCommand(const QString &command)
{
    // Traitement des commandes TCP reçues
    if (command == COMMAND_GET_CLASSES) {
        QByteArray response = getClassesJson();
        socketDialogueClient->write(response + "\n");
    }
    else if (command == COMMAND_GET_OBJECTIFS) {
        QByteArray response = getObjectifsJson();
        socketDialogueClient->write(response + "\n");
    }
    else {
        socketDialogueClient->write("Commande inconnue\n");
    }
}
