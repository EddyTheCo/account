import QtQuick.Layouts
import QtQuick.Controls
import QtQuick

import Esterv.Iota.Account

ColumnLayout
{
    id:control
    onVisibleChanged:
    {
        oldpass.text="";
        newpass.text="";
    }
    signal success();
    Label
    {
        text: qsTr("Enter the vault password:")
    }
    TextField
    {
        id:oldpass
        Layout.fillWidth: true
        echoMode:TextInput.PasswordEchoOnEdit
    }
    Label
    {
        text: qsTr("Set the vault password:")
    }
    TextField
    {
        id:newpass
        Layout.fillWidth: true
        echoMode:TextInput.PasswordEchoOnEdit
        inputMethodHints:Qt.ImhHiddenText|Qt.ImhSensitiveData|Qt.ImhNoAutoUppercase|Qt.ImhNoPredictiveText
    }

    Button
    {
        text:qsTr("ok")
        enabled: (newpass.text.length>8)&&(oldpass.text.length>8)
        onClicked:
        {
            if(Account.changeVaultPassword(oldpass.text,newpass.text))
            {
                control.success();
            }
            else
            {
                oldpass.ToolTip.show(qsTr("Wrong password"),5000);
            }

        }
        Layout.alignment: Qt.AlignRight || Qt.AlignBottom
    }
}

