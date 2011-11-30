import QtQuick 1.1
import com.nokia.meego 1.0
//import QtMobility.connectivity 1.2
//import QtMobility.sensors 1.2


Page {
    //This shouldn't be hardcoded.
    property string btAddress: "00:80:e1:fb:db:97"  //spot
    //property string btAddress: "2c:d2:e7:0c:0e:93" //n900
    property bool connecting: false

    tools: commonTools


    Label {
        id: title

        anchors.top: parent.top
        anchors.topMargin: 32
        anchors.verticalCenter: parent.verticalCenter

        text: "Goodspeed's Spot Connector"
        platformStyle: LabelStyle {
            fontFamily: "Nokia Pure"
            fontPixelSize: 32
        }
    }

    Button {
        id: macAddress
        anchors.top: parent.top
        anchors.topMargin: 64

        text: btAddress
    }



    Label {
        id: uid
        anchors.top: parent.top
        anchors.topMargin: 128
        anchors.verticalCenter: parent.verticalCenter

        text: "UID: Unknown"
        platformStyle: LabelStyle {
            fontFamily: "Nokia Pure"
            fontPixelSize: 32
        }
    }

    Label {
        id: poslat
        anchors.top: parent.top
        anchors.topMargin: 160
        anchors.verticalCenter: parent.verticalCenter

        text: "Pos: Unknown"
        platformStyle: LabelStyle {
            fontFamily: "Nokia Pure"
            fontPixelSize: 32
        }
    }
    Label {
        id: poslon
        anchors.top: parent.top
        anchors.topMargin: 192
        anchors.verticalCenter: parent.verticalCenter

        text: "Pos: Unknown"
        platformStyle: LabelStyle {
            fontFamily: "Nokia Pure"
            fontPixelSize: 32
        }
    }

    Button {
        id: checkin
        anchors.top: parent.top
        anchors.topMargin: 224
        width: parent.width-16

        text: "Check In"
        onClicked:{
            if(SpotConnect.isConnected())
                SpotConnect.checkin(message.text);
        }
    }

    TextField {
        id: message
        anchors.top: checkin.bottom
        anchors.topMargin: 10
        height: 64
        width: parent.width-16

        text: "All is well."
    }


    Switch {
        id: blueSwitch
        anchors.top: parent.top
        anchors.topMargin: 64
        anchors.left: parent.right
        anchors.leftMargin: -96

        anchors.verticalCenter: parent.verticalCenter

        checked: false
        onCheckedChanged: {
            console.log("switch: " + checked)
            if (blueSwitch.checked == false) {
                //blueSocket.connected = false //connect to bluetooth
                console.log("Disconnecting by switch.");
                SpotConnect.close();
            } else {
                //blueSocket.connected = true //connect to bluetooth
                console.log("Connecting to "+btAddress);
                SpotConnect.open(btAddress);
                connecting=true;
            }
        }
    }

    function updateView() {
        uid.text="UID: "+SpotConnect.serial();
    }  // slot

    //TODO replace this with proper events.
    Timer {
        interval: 1000; running: true; repeat: true
        onTriggered: {
            if (SpotConnect.isConnected()) {
                uid.text="UID: "+SpotConnect.serial();
                poslat.text=SpotConnect.latitute().toFixed(3);
                poslon.text=SpotConnect.longitute().toFixed(3);
                //Done connecting, so make sure that's marked.
                connecting=false;
            }else{
                if(blueSwitch.checked && connecting==false)
                    blueSwitch.checked=false;
            }
        }
    }

}
