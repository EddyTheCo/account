import QtQuick.Layouts
import QtQuick
import account
import MyDesigns
import QtQuick.Controls


MyFrame
{
    id:root_
    description: qsTr("Save the seed in a secure place")

    MyTextArea
    {
        id:seed_
        width:root_.width*0.95
        height:root_.height*0.95
        anchors.centerIn: root_
        tfield.text:Account.seed
        tfield.readOnly:true
        tfield.wrapMode:Text.WrapAnywhere

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
                textEdit.text = Account.seed
                textEdit.selectAll()
                textEdit.copy()
            }
        }
    }


}
