import QtQuick.Layouts
import QtQuick
import MyDesigns
import account


MyFrame
{
    id:root_
    description: qsTr("Account settings")

    ColumnLayout
    {
        anchors.fill: parent

        MyRadioButton
        {
            id:but_1
            checked: true
            text:qsTr("New account")
            Layout.maximumHeight: 75
            Layout.maximumWidth: 200
            Layout.preferredHeight: 35
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 100
            Layout.alignment: Qt.AlignLeft
        }
        MyRadioButton
        {
            id:but_2
            checked: false
            text:qsTr("Restore account")
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 100
            Layout.maximumHeight: 75
            Layout.maximumWidth: 200
            Layout.preferredHeight: 35
            Layout.alignment: Qt.AlignLeft

        }



        ShowSeed
        {
            id:rseed_
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 250
            Layout.minimumHeight: 100
            Layout.alignment: Qt.AlignCenter
            visible:but_1.checked
        }

        RestoreAccount
        {
            id:raccou_
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 250
            Layout.minimumHeight: 100
            Layout.alignment: Qt.AlignCenter
            visible:but_2.checked
        }

    }

}
