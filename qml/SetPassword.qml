import QtQuick.Layouts
import QtQuick.Controls
import QtQuick
import Esterv.Iota.Account

ColumnLayout
{
    id:control
    property bool isSet:false
    signal success()
    onVisibleChanged:
    {
        pass.text="";
    }
    Label
    {
        text: qsTr(((Account.isVaultEmpty)?"Set":"Enter")+" the password:")
    }
    TextField
    {
        id:pass
        Layout.fillWidth: true
        echoMode:TextInput.PasswordEchoOnEdit
        ToolTip.text: qsTr("Wrong password")
        ToolTip.visible: false
    }

    Button
    {
        text:qsTr("ok")
        enabled: pass.text.length>8
        onClicked:
        {
            if(control.isSet)
            {
                if(Account.writeToVault(pass.text))
                {
                    control.success();
                }
                else
                {
                    pass.ToolTip.show(qsTr("Wrong password"),5000);
                }
            }
            else
            {
                if(Account.readFromVault(pass.text))
                {
                    control.success();
                }
                else
                {
                    pass.ToolTip.show(qsTr("Wrong password"),5000);
                }
            }
        }
        Layout.alignment: Qt.AlignRight || Qt.AlignBottom
    }

}
