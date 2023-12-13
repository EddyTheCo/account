import QtQuick.Layouts
import QtQuick.Controls
import QtQuick
import Esterv.Iota.Account
import Esterv.Styles.Simple


ColumnLayout
{
    id:control
    property bool advancemode:false
    RowLayout
    {
        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: 10
        Label
        {
            text:qsTr("Account settings")
            elide:Text.ElideRight
            horizontalAlignment: TextEdit.AlignLeft
            verticalAlignment: TextEdit.AlignVCenter
            Layout.fillWidth: true
        }
        TabBar {
            Layout.fillWidth: true
            Layout.maximumWidth: 250
            TabButton {
                text: "Basic"
                onClicked:control.advancemode=false;

            }
            TabButton {
                text: "Advanced"
                onClicked:control.advancemode=true

            }
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
                Layout.maximumWidth: 400
                Label
                {
                    text:qsTr("Seed")
                    elide:Text.ElideRight
                    Layout.fillWidth: true
                }

                TabBar {
                    id:showorrestore
                    property bool restore:false
                    Layout.fillWidth: true
                    Layout.maximumWidth: 250

                    TabButton {
                        text: qsTr("Show")
                        onClicked: showorrestore.restore=false;
                    }
                    TabButton {
                        text: qsTr("Restore")
                        onClicked: showorrestore.restore=true;

                    }
                }
            }
            ShowSeed
            {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 50
                Layout.maximumHeight: 200
                Layout.maximumWidth: 400
                Layout.alignment: Qt.AlignLeft || Qt.AlignVCenter
                readOnly:!showorrestore.restore
            }
        }
    }
}


