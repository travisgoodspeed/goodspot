#ifndef SPOTCONNECT_H
#define SPOTCONNECT_H


#include <QObject>

//Bluetooth stuff.
//Remember to include QtConnectivity in the .pro file!
#include <QtConnectivity/QBluetoothAddress>
#include <QtConnectivity/QBluetoothSocket>
#include <QtConnectivity/QBluetoothUuid>

#include <QtCore/QDataStream>
#include <QtCore/QByteArray>
#include <QtCore/QStringList>

using QTM_PREPEND_NAMESPACE(QBluetoothSocket);
using QTM_PREPEND_NAMESPACE(QBluetoothAddress);
using QTM_PREPEND_NAMESPACE(QBluetoothUuid);

class SpotConnect : public QObject
{
    Q_OBJECT
public:
    explicit SpotConnect(QObject *parent = 0);
    enum MessageType {
        GetID = 0x01,
        GetCoord = 0x25
    };
    QBluetoothSocket *socket;
    QBluetoothAddress *adr;
    QBluetoothUuid *uuid;
    Q_INVOKABLE QString open(const QString &address)  {
        // Initialize privates.
        uid=0;
        lat=0;
        lon=0;

        // Connect to service
        socket = new QBluetoothSocket(QBluetoothSocket::RfcommSocket);
        adr=new QBluetoothAddress(address);
        uuid=new QBluetoothUuid(QString("00001101-0000-1000-8000-00805F9B34FB"));
        qDebug()<<"Created socket to "<<adr->toString();

        QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        QObject::connect(socket, SIGNAL(connected()), this, SLOT(connected()));
        QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
        QObject::connect(socket, SIGNAL(error(QBluetoothSocket::SocketError)),
                         this,SLOT(error(QBluetoothSocket::SocketError)));

        socket->connectToService(*adr,
                                 *uuid);


        qDebug() << "ConnecttoService done";



        return address;
    }
    Q_INVOKABLE bool close()  {
        // Connect to service
        if(!socket) return false;
        socket->close();
        delete socket;
        delete adr;
        delete uuid;
        lat=0;
        lon=0;
        socket=0;
        return true;
    }
    Q_INVOKABLE bool isConnected() {
        if(!socket) return false;
        if(socket->state()==QBluetoothSocket::ConnectedState)
            return true;
        return false;
    }
    Q_INVOKABLE qint64 serial() {
        if(!this->isConnected()) return 0;

        const char *data="\xAA\x03\x01";
        if(uid==0)
            socket->write(data,data[1]);
        return uid;
    }
    Q_INVOKABLE double latitute() {
        if(!this->isConnected()) return 0;

        const char *data="\xaa\x03\x25";
        if(lat==0)
            socket->write(data,data[1]);
        return lat;
    }
    Q_INVOKABLE double longitute() {
        if(!this->isConnected()) return 0;

        /* Don't bother requesting it; lat will take care of that.
        const char *data="\xaa\x03\x25";
        if(lat==0)
            socket->write(data,data[1]);
            */
        return lon;
    }

    Q_INVOKABLE bool checkin(const QString &text) {
        if(!this->isConnected()) return false;

        qDebug()<<"Trying to checkin:"<<text;
        QByteArray bytes=text.toAscii();
        char data[512];
        int len;
        data[0]=0xAA; //preamble
        //data[1] would be length, set later.
        //packet="\xAA"+chr(len)+"\x26\x01\x00\x01\x00\x01"+string;
        data[2]=0x26;
        data[3]=0x01;
        data[4]=0x00;
        data[5]=0x01;
        data[6]=0x00;
        data[7]=0x01;

        //string begins at 0x08;
        for(len=0;len<bytes.length() && len<41;len++){
            data[8+len]=bytes.at(len);
        }
        //Now set length in packet.
        data[1]=len+8;
        //And send it away!
        socket->write(data,data[1]);
        qDebug()<<"Message sent!";
        return true;
    }
signals:
    void gotUID(qint64 data);
    void gotPosition(double lat, double lon);
private:
    qint64 uid;
    double lat, lon;
    void parseGetID(char *data);
    void parsePosition(char *data);

public slots:
    void readSocket();
    void disconnected(){
        qDebug()<<"Disconnected signal fired and caught.";
    }
    void connected(){
        qDebug()<<"Connected signal fired and caught.";
        //Ask for the serial number.
        this->serial();
    }
    void error(QBluetoothSocket::SocketError error){
        qDebug()<<"Caught an error: "<<error<<" "<<socket->errorString();
    }
};

#endif // SPOTCONNECT_H
