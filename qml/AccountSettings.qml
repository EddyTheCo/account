import QtQuick.Layouts
import QtQuick.Controls
import QtQuick
import Esterv.Iota.Account
import Esterv.Styles.Simple


Frame
{
    id:control
    property bool advancemode:false
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
                TextArea
                {
                    id:seed_
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: 50
                    Layout.minimumWidth:  100
                    Layout.maximumHeight: 100
                    Layout.alignment: Qt.AlignLeft || Qt.AlignVCenter
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
        }
    }


}


