// server.cpp

#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>

class MyThread : public QThread {
    Q_OBJECT

public:
    MyThread(qintptr socketDescriptor, QObject *parent) : QThread(parent), socketDescriptor(socketDescriptor) {}

    void run() override { //func has called when thread -> start
        QTcpSocket tcpSocket;//create socket
        if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
            emit error(tcpSocket.error());
            return;
        }

        // create json data
        QJsonObject jsonObject;
        jsonObject["name"] = "John";
        jsonObject["age"] = 30;

        QJsonDocument doc(jsonObject);
        QByteArray jsonData = doc.toJson(); //json form like dictionary

        // 데이터 전송
        tcpSocket.write(jsonData);
        tcpSocket.waitForBytesWritten();//wait for finishing send data
        tcpSocket.disconnectFromHost();
    }

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    qintptr socketDescriptor;
};


class MyServer : public QTcpServer {
    Q_OBJECT

public:
    MyServer(QObject *parent = 0) : QTcpServer(parent) {}

protected:
    void incomingConnection(qintptr socketDescriptor) override {
        MyThread *thread = new MyThread(socketDescriptor, this);
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();
    }
};



int main(int argc, char *argv[]) {
    QCoreApplication  a(argc, argv);

    MyServer server;
    if (!server.listen(QHostAddress::Any, 12345)) {
        qDebug() << "Server could not start!";
        return 1;
    }

    qDebug() << "Server started on port 12345";

    return a.exec();
}

#include "main.moc"
