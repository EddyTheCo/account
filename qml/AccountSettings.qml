import QtQuick.Layouts
import QtQuick.Controls
import QtQuick
import Esterv.Iota.Account

Frame
{
    id:control
    property bool advancemode:false

    Popup
    {
        id:checkpop
        visible:false
        closePolicy: Popup.CloseOnPressOutside
        anchors.centerIn: Overlay.overlay
        focus:true
        modal:true

        SetPassword
        {
            id:checkpassword
            onSuccess:
            {
                seed_.text=Account.seed;
                showorrestore.restore=false;
                checkpop.close()
            }
        }
    }
    Popup
    {
        id:changepop
        visible:false
        closePolicy: Popup.CloseOnPressOutside
        anchors.centerIn: Overlay.overlay
        focus:true
        modal:true
        ChangePassword
        {
            id:changepassword
            onSuccess:changepop.close()
        }
    }

    ColumnLayout
    {
        anchors.fill: parent
        RowLayout
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 5
            Label
            {
                text:qsTr("Account settings")
                elide:Text.ElideRight
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
                Layout.fillWidth: true
            }
            RoundButton
            {
                text: (control.advancemode)?qsTr("Advanced"):qsTr("Basic")
                onClicked:
                {
                    control.advancemode=!control.advancemode;
                    Account.mnmonicMode=!control.advancemode;
                    seed_.text=showorrestore.restore?"":Account.seed;
                }
                flat:true
            }


        }
        Frame
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ColumnLayout
            {
                anchors.fill: parent
                RowLayout
                {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: control.advancemode
                    Label
                    {
                        text:qsTr("Coin type:")
                    }
                    SpinBox {
                        value: Account.path[1]
                        from:0
                        to: 429496729 //fix this
                        onValueChanged: Account.path[1]=value;
                    }
                }

                RowLayout
                {
                    Layout.fillWidth: true

                    TabBar {
                        id:showorrestore
                        property bool restore:false
                        Layout.fillWidth: true

                        TabButton {
                            text: qsTr("Current")
                            onClicked:
                            {
                                showorrestore.restore=false;
                                seed_.text=Account.seed;
                            }
                        }
                        TabButton {
                            text: qsTr("Restore")
                            onClicked:
                            {

                                showorrestore.restore=true;
                                seed_.text="";
                            }

                        }
                    }
                }
                ScrollView {
                    id: view
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: 50
                    Layout.minimumWidth:  100
                    Layout.maximumHeight: 100
                    Layout.alignment: Qt.AlignLeft || Qt.AlignVCenter
                    TextArea
                    {
                        id:seed_

                        readOnly:!showorrestore.restore

                        wrapMode:Text.WrapAnywhere
                        inputMethodHints:Qt.ImhSensitiveData
                        text:Account.seed
                        onEditingFinished:
                        {
                            Account.seed=seed_.text
                        }
                        placeholderText: control.advancemode?qsTr("e2f88a043776c828063..."):qsTr("ozone drill grab fiber ...")
                    }
                }
                RowLayout
                {
                    Layout.alignment: Qt.AlignRight
                    Button
                    {
                        Layout.margins: 5
                        Layout.fillWidth: true
                        Layout.maximumWidth: 150
                        text:qsTr("Change password")
                        visible: (!Account.isVaultEmpty&&!showorrestore.restore)
                        ToolTip.text: text
                        ToolTip.visible: hovered
                        onClicked:
                        {
                            changepop.open();
                        }
                    }
                    Button
                    {

                        Layout.margins: 5
                        Layout.fillWidth: true
                        Layout.maximumWidth: 150
                        text:qsTr((showorrestore.restore)?"From Vault":"Save To Vault")
                        ToolTip.text: text
                        ToolTip.visible: hovered
                        visible: ((showorrestore.restore&&!Account.isVaultEmpty)||!showorrestore.restore)
                        onClicked:
                        {
                            if(showorrestore.restore)
                            {
                                checkpassword.isSet=false;
                                checkpop.open();
                            }
                            else
                            {
                                checkpassword.isSet=true;
                                checkpop.open();
                            }
                        }
                    }

                }
            }
        }
    }


}


