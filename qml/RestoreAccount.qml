import QtQuick.Layouts
import QtQuick
import account
import MyDesigns



MyFrame
{
    id:root_
    description: qsTr("Restore account")
    ColumnLayout
    {
        anchors.fill: parent
        spacing: 20

        MyTextField
        {
            id:seed_
            placeholderText:qsTr("seed")
            Layout.preferredWidth: 400
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 75
            Layout.minimumHeight: 45
            Layout.minimumWidth:100
            Layout.preferredHeight: 50
            tfield.echoMode:TextInput.PasswordEchoOnEdit
            tfield.onEditingFinished: Account.seed=seed_.text
        }


    }
}
