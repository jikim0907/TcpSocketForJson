// client.cpp
#include <QCoreApplication>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QTcpSocket socket;//create tcp socket
    socket.connectToHost("127.0.0.1", 12345); //connect loopback

    if (socket.waitForConnected()) {
        qDebug() << "Connected to server";

        // 데이터 수신
        socket.waitForReadyRead(); //wait for finishing recieve data from server
        QByteArray jsonData = socket.readAll(); //read data from server

        // JSON 데이터 파싱
        QJsonDocument doc = QJsonDocument::fromJson(jsonData); //Get json in QJsonDocument format has process from json data in bytearray format.
        QJsonObject jsonObject = doc.object();//convert QJsonDocument format to QJsonObject

        // 데이터 출력
        qDebug() << "Name: " << jsonObject["name"].toString();
        qDebug() << "Age: " << jsonObject["age"].toInt();

        socket.disconnectFromHost();
    } else {
        qDebug() << "Connection failed!";
    }

    return a.exec();
}
