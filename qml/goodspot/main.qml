import QtQuick 1.1
import com.nokia.meego 1.0
import "database.js" as Storage

PageStackWindow {
    id: appWindow

    initialPage: mainPage



    ScannerPage {
        id: scannerPage
        orientationLock: PageOrientation.LockPortrait
    }
    MainPage {
        id: mainPage
        btAddress: "00:80:e1:fb:db:97"
        orientationLock: PageOrientation.LockPortrait
    }

    Component.onCompleted: {
        // Initialize the database
        Storage.initialize();
        // Sets a value in the database
        //Storage.setSetting("mySetting","myValue");
        // Sets the textDisplay element's text to the value we just set
        mainPage.btAddress = Storage.getSetting("btAddress");
    }

    function saveAddress(){
        Storage.setSetting("btAddress", mainPage.btAddress);
    }



    ToolBarLayout {
        id: commonTools
        visible: true
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("Bluetooth Discovery")
                onClicked: scannerPage.startScan();
            }
        }
    }
}
