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
    void on_pushButtonLaunchWeb_clicked();
    void onQTcpServer_NewConnection();
    void onQTcpSocket_Connected();
    void onQTcpSocket_Disconnected();
    void onQTcpSocket_ReadyRead();
    void onQTcpSocket_ErrorOccurred(QAbstractSocket::SocketError _socketError);
    bool insererDansBaseDeDonnees(const QJsonObject &json);
    void resetCommandes();

private:
    Ui::Widget *ui;
    QNetworkAccessManager manager;
    QHttpServer serveurHttp;
    static const QString COMMAND_GET_CLASSES;
    static const QString COMMAND_GET_OBJECTIFS;
    QByteArray getClassesJson();
    QByteArray getObjectifsJson();
    void processTcpCommand(const QString &command);
    QHttpServerResponse handleHttpRequest(const QHttpServerRequest &request);
    QTcpServer *socketEcouteServeur;
    QTcpSocket *socketDialogueClient;
    QByteArray ancienneCommande;
    QByteArray jsonData;
    long int cpt;
    QSqlDatabase db;
    QTimer *resetTimer;
};
#endif // WIDGET_H
