import QtQuick.Layouts
import QtQuick
import MyDesigns
import account


MyFrame
{
    id:root_
    description: qsTr("Account settings")

    Connections {
        target: Account
        function onSeedChanged() {root_.changed(); }
    }

    ColumnLayout
    {
        anchors.fill: parent
        spacing:1


        MyRadioButton
        {
            id:but_1
            checked: true
            text:qsTr("New account")
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 75
            Layout.maximumWidth: 75
            Layout.minimumHeight: 45
            Layout.alignment: Qt.AlignLeft

        }
        MyRadioButton
        {
            id:but_2
            checked: false
            text:qsTr("Restore account")
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 75
            Layout.maximumWidth: 75
            Layout.minimumHeight: 45
            Layout.alignment: Qt.AlignLeft

        }



        ShowSeed
        {
            id:rseed_
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 200
            Layout.alignment: Qt.AlignCenter
            visible:but_1.checked
        }

        RestoreAccount
        {
            id:raccou_
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 200
            Layout.alignment: Qt.AlignCenter
            visible:but_2.checked
        }


    }

}
