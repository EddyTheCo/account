import QtQuick.Layouts
import QtQuick
import MyDesigns
import account


MyFrame
{
    id:root
    description: qsTr("Account settings")

    ColumnLayout
    {
        visible: (root.collapsed>0.7)
        anchors.fill: parent
        GridLayout
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumWidth: 300

            columns: parent.width > 200 ? 2 : 1
            rows : parent.width > 200 ? 1 : 2
            MyRadioButton
            {
                id:but_1
                checked: true
                text:qsTr("New")

                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignLeft
            }
            MyRadioButton
            {
                id:but_2
                checked: false
                text:qsTr("Restore")
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignLeft
            }
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
