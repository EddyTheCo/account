import QtQuick.Layouts
import QtQuick
import MyDesigns
import account


MyFrame
{
    id:root_
    description: qsTr("Account settings")

    signal changed()

    Connections {
        target: Account
        function onSeedChanged() {root_.changed(); }
    }

    ColumnLayout
    {
        anchors.fill: parent
        spacing: 20


        MyRadioButton
        {
            id:but_1
            checked: true
            text:qsTr("New account")
            Layout.preferredWidth: 200
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 75
            Layout.maximumWidth: 300
            Layout.minimumHeight: 45
            Layout.minimumWidth:150
            Layout.preferredHeight: 50
            Layout.alignment: Qt.AlignLeft

        }
        MyRadioButton
        {
            id:but_2
            checked: false
            text:qsTr("Restore account")
            Layout.preferredWidth: 200
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 75
            Layout.maximumWidth: 300
            Layout.minimumHeight: 45
            Layout.minimumWidth:150
            Layout.preferredHeight: 50
            Layout.alignment: Qt.AlignLeft
        }

        ShowSeed
        {
            id:rseed_
            Layout.preferredWidth: 300
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 150
            Layout.maximumWidth: 400
            Layout.minimumHeight: 100
            Layout.minimumWidth:200
            Layout.preferredHeight: 100
            Layout.alignment: Qt.AlignHCenter
            visible:but_1.checked
            onClosed: () =>changed()
        }

        RestoreAccount
        {
            id:raccou_
            Layout.preferredWidth: 300
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 150
            Layout.maximumWidth: 400
            Layout.minimumHeight: 100
            Layout.minimumWidth:200
            Layout.preferredHeight: 100
            Layout.alignment: Qt.AlignHCenter
            visible:but_2.checked
        }


    }

}
