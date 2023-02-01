import QtQuick.Layouts
import QtQuick
import account
import MyDesigns
import QtQuick.Controls


MyFrame
{
    id:root_
    description: qsTr("Save the seed in a secure place")

    signal closed()

    ColumnLayout
    {
        anchors.fill: parent
        spacing: 20

        MyTextField
        {
            id:seed_
            tfield.text:Account.seed
            Layout.preferredWidth: 400
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 75
            Layout.minimumHeight: 45
            Layout.minimumWidth:100
            Layout.preferredHeight: 50
            tfield.echoMode:TextInput.Password
            tfield.readOnly:true

            ToolTip
            {
                id:tooltip
                visible: false
                text:qsTr("Copy")
            }
            TextEdit{
                id: textEdit
                visible: false
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled :true
                onEntered: tooltip.visible=!tooltip.visible
                onExited: tooltip.visible=!tooltip.visible
                onClicked:
                {
                    textEdit.text = seed_.tfield.text
                    textEdit.selectAll()
                    textEdit.copy()
                    root_.closed()
                }
            }

        }

    }
}
