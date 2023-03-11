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
        height:root_.height*0.5
        anchors.centerIn: parent
        tfield.text:Account.seed
        tfield.readOnly:true
        tfield.wrapMode:Text.WrapAnywhere

    }


}
