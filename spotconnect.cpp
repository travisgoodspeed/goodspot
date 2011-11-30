#include "spotconnect.h"

SpotConnect::SpotConnect(QObject *parent) :
    QObject(parent)
{
}

void SpotConnect::readSocket(){
    char data[512];
    qint64 len=0;
    if (!socket)
        return;
    qDebug()<<"Reading data.";
    while (socket->bytesAvailable()) {
        len=socket->read(data,256);
    }
    qDebug()<<"Read "<<len<<" bytes of data.";
    if(data[0]!=0xAA){
        qDebug()<<"Uh oh, SFD is missing.  Skipping message for safety.";
        return;
    }
    while(len<data[1]){
        qDebug()<<"Waiting on remaining packet fragments.  FIXME this might hang.";
        len+=socket->read(data+len,256-len);
    }
    switch(data[2]){
    case 0x01: //got ID.
        this->parseGetID(data);
        break;
    case 0x25: //got GPS.
        this->parsePosition(data);
        break;
    case 0x2B: //set message.
        //break;
    case 0x52: //get status.
        //break;
    default:
        qDebug()<<"Unknown command code "<< ((int)data[2]);
    }
}

void SpotConnect::parseGetID(char *data){
    int i;
    uid=0;
    //Bytes 3,4,5,6 are the ID, Big Endian.
    for(i=3;i<7;i++)
        uid=(uid<<8)|data[i];
    qDebug()<<"UID read to be "<<uid;
    emit gotUID(uid);
}

void SpotConnect::parsePosition(char *data){
    int i;
    long ilat=0, ilon=0;

    //Bytes 3,4,5 are lat; 6,7,8 are lon
    //Format from http://natrium42.com/projects/spot/
    for(i=3;i<6;i++)
        ilat=(ilat<<8)+((unsigned char)data[i]);
    for(i=6;i<9;i++)
        ilon=(ilon<<8)+((unsigned char)data[i]);

    //Decode Latitute.
    lat=ilat*90.0/8388608.0;
    if(lat>45)  lat=lat-90;

    //Decode Longitude.
    lon=ilon*180.0/8388608.0;
    if(lon>180) lon=lon-360;

    qDebug()<<"Position read to be "<<lat<<", "<<lon;
    emit gotPosition(lat,lon);
}
