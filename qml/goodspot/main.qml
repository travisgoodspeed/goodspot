import QtQuick 1.1
import com.nokia.meego 1.0

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
