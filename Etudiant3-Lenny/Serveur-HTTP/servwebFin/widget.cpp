#include "widget.h"
#include "ui_widget.h"
#include <QJsonObject>
#include <QJsonDocument>
const QString Widget::COMMAND_GET_CLASSES = "GET_CLASSES";
const QString Widget::COMMAND_GET_OBJECTIFS = "GET_OBJECTIFS";


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    ,ancienneCommande(nullptr)
    , socketEcouteServeur(new QTcpServer(this))
    , socketDialogueClient(nullptr)
{
    ui->setupUi(this);
    cpt=0;
    // Serveur HTTP
    serveurHttp.route("/", [this](const QHttpServerRequest &request) {
        return handleHttpRequest(request);
    });

    // Connexion du signal de nouvelle connexion pour le serveur TCP
    connect(socketEcouteServeur, &QTcpServer::newConnection, this, &Widget::onQTcpServer_NewConnection);

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("172.18.58.7");
    db.setDatabaseName("Projet_Drone_test");
    db.setUserName("snir"); // Change selon tes identifiants
    db.setPassword("snir"); // Change aussi ici

    if (!db.open()) {
        qDebug() << "Erreur de connexion BDD :" << db.lastError().text();
    } else {
        //qDebug() << "Connexion BDD réussie !";
    }
    QTimer *resetTimer = new QTimer(this);
    connect(resetTimer, &QTimer::timeout, this, &Widget::resetCommandes);
    resetTimer->start(360000); // 1 heure = 3600000 ms

    const quint16 portHttp = 8080;
    if (serveurHttp.listen(QHostAddress::Any, portHttp)) {
        qDebug() << "Serveur HTTP lancé automatiquement sur le port" << portHttp;
    } else {
        qDebug() << "Échec du lancement automatique du serveur HTTP sur le port" << portHttp;
    }

    // Lancement automatique du serveur TCP sur le port 8081
    const quint16 portTcp = 8081;
    if (socketEcouteServeur->listen(QHostAddress::Any, portTcp)) {
        qDebug() << "Serveur TCP lancé automatiquement sur le port" << portTcp;
    } else {
        qDebug() << "Échec du lancement automatique du serveur TCP sur le port" << portTcp;
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::resetCommandes()
{
    cpt = 0;
    ancienneCommande.clear(); // vide les anciennes données
    ui->textEditLog->clear();
    ui->textEditLog->append("Remise à zéro des commandes.");
    //qDebug() << "Commandes réinitialisées après 1 heure.";
}

// bool Widget::insererDansBaseDeDonnees(const QJsonObject &json) {
//     if (!db.isOpen()) return false;

//     QSqlQuery query;

//     // 1. Insérer dans SIMULATIONS
//     query.prepare("INSERT INTO SIMULATIONS (date, duree, id_utilisateur) VALUES (CURRENT_DATE(), ?, ?)");
//     query.addBindValue(json["Duree"].toInt());
//     query.addBindValue(12); // ou autre utilisateur valide
//     if (!query.exec()) {
//         qDebug() << "Erreur insertion SIMULATIONS:" << query.lastError().text();
//         return false;
//     }
//     int id_simulation = query.lastInsertId().toInt();

//     // 2. Convertir vent en enum
//     QString ventStr = "faible";
//     int ventVal = json["Vent"].toInt();
//     if (ventVal >= 60) ventStr = "fort";
//     else if (ventVal >= 30) ventStr = "moyen";

//     // 3. Insérer dans SCENARIOS
//     query.prepare("INSERT INTO SCENARIOS (point_apparition, vent, pluie, temperature, id_simulation) VALUES (?, ?, ?, ?, ?)");
//     query.addBindValue(json["Scenario"].toString().trimmed());
//     query.addBindValue(ventStr);
//     query.addBindValue(json["Pluie"].toInt());
//     query.addBindValue(json["Celsius"].toDouble());
//     query.addBindValue(id_simulation);
//     if (!query.exec()) {
//         qDebug() << "Erreur insertion SCENARIOS:" << query.lastError().text();
//         return false;
//     }
//     int id_scenario = query.lastInsertId().toInt();

//     // 4. Insérer dans DRONES
//     query.prepare("INSERT INTO DRONES (type_drone, description, id_scenario) VALUES (?, ?, ?)");
//     query.addBindValue(json["Drone"].toString().trimmed());
//     query.addBindValue("Drone reçu depuis JSON");
//     query.addBindValue(id_scenario);
//     if (!query.exec()) {
//         qDebug() << "Erreur insertion DRONES:" << query.lastError().text();
//         return false;
//     }

//     return true;
// }

bool Widget::insererDansBaseDeDonnees(const QJsonObject &json) {
    if (!db.isOpen()) return false;

    QSqlQuery query;

    // 1. SIMULATIONS
    bool dureeOk;
    int duree = json["Duree"].toString().toInt(&dureeOk);
    if (!dureeOk) {
        //qDebug() << "Erreur de conversion de la durée";
        return false;
    }

    query.prepare("INSERT INTO SIMULATIONS (date, duree, id_utilisateur) VALUES (CURRENT_DATE(), ?, ?)");
    query.addBindValue(duree);
    query.addBindValue(12); // id_utilisateur à adapter
    if (!query.exec()) {
        qDebug() << "Erreur SIMULATIONS:" << query.lastError().text();
        return false;
    }
    int id_simulation = query.lastInsertId().toInt();

    // 2. SCENARIOS
    double temp = json["Celsius"].toString().toDouble();
    int pluie = json["Pluie"].toString().toInt();
    int vent = json["Vent"].toString().toInt();

    query.prepare("INSERT INTO SCENARIOS (point_apparition, vent, pluie, temperature, id_simulation) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(json["Scenario"].toString().trimmed());
    query.addBindValue(vent);        // vent brut
    query.addBindValue(pluie);       // pluie exacte
    query.addBindValue(temp);        // température
    query.addBindValue(id_simulation);
    if (!query.exec()) {
        qDebug() << "Erreur SCENARIOS:" << query.lastError().text();
        return false;
    }
    int id_scenario = query.lastInsertId().toInt();

    // 3. DRONES
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



void Widget::on_pushButtonLaunchWeb_clicked()
{
    // // Lance le serveur HTTP
    // if (serveurHttp.listen(QHostAddress::Any, ui->spinBoxPortHttp->value())) {
    //     qDebug() << "Lancement du serveur HTTP sur le port" << ui->spinBoxPortHttp->value();
    // } else {
    //     qDebug() << "Problème de lancement du serveur HTTP sur le port" << ui->spinBoxPortHttp->value();
    // }

    // // Lance le serveur TCP
    // if (socketEcouteServeur->listen(QHostAddress::Any, ui->spinBoxPortTCP->value())) {
    //     qDebug() << "Lancement du serveur TCP sur le port" << ui->spinBoxPortTCP->value();
    // } else {
    //     qDebug() << "Problème de lancement du serveur TCP sur le port" << ui->spinBoxPortTCP->value();
    // }
}

void Widget::onQTcpServer_NewConnection()
{
    // Récupère la nouvelle connexion TCP
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

void Widget::onQTcpSocket_Connected()
{
    //qDebug() << "Connexion TCP établie.";
}

void Widget::onQTcpSocket_Disconnected()
{
    //qDebug() << "Client TCP déconnecté.";
}

void Widget::onQTcpSocket_ReadyRead()
{
    // Lecture des données du client
    QByteArray data = socketDialogueClient->readAll();
    QString command = QString::fromUtf8(data).trimmed();

    //qDebug() << "Commande TCP reçue : " << command;

    processTcpCommand(command);
}
void Widget::onQTcpSocket_ErrorOccurred(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Erreur sur le socket TCP : " << socketError;
}

bool jsonPresqueEgal(const QByteArray &a, const QByteArray &b, double delta)
{
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

QHttpServerResponse Widget::handleHttpRequest(const QHttpServerRequest &request)
{
    QUrl url = request.url();

    // Route pour les classes
    if (url.path() == "/classes" && request.method() == QHttpServerRequest::Method::Get) {
        return QHttpServerResponse(getClassesJson(), "application/json");
    }

    // Route pour les objectifs
    if (url.path() == "/objectifs" && request.method() == QHttpServerRequest::Method::Get) {
        return QHttpServerResponse(getObjectifsJson(), "application/json");
    }

    // Route principale (ancienne fonctionnalité)
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

QByteArray Widget::getClassesJson()
{
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

QByteArray Widget::getObjectifsJson()
{
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

void Widget::processTcpCommand(const QString &command)
{
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
