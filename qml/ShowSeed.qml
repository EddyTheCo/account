import QtQuick
import account
import MyDesigns
import QtQuick.Controls



MyTextArea
{
    id:seed_
    textarea.text:Account.seed
    textarea.readOnly:true
    textarea.wrapMode:Text.WrapAnywhere
    label.text: qsTr("This is your seed")
}



