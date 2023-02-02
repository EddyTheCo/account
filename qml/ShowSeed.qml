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
        tfield.selectByMouse:true
        tfield.readOnly:true
        tfield.wrapMode:Text.WrapAnywhere

    }

}
