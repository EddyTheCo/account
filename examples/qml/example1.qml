
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Esterv.Styles.Simple
import Esterv.CustomControls
import Esterv.Iota.Account
import Esterv.CustomControls.QrGen

ApplicationWindow {
    visible: true
    id:window

    Connections {
            target: Account
            function onChanged() {
                addressmodel.clear();
            }
        }
    background:Rectangle
    {
        color:Style.backColor1
    }
    ColumnLayout
    {
        anchors.fill: parent

        ThemeSwitch
        {
            id:themeswitch
        }

        AccountSettings
        {
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.minimumWidth: 350
            Layout.maximumWidth: 500
        }
        RowLayout
        {
            Layout.alignment: Qt.AlignHCenter
            Label
            {
                text:"HRP:"
            }
            ComboBox {
                id:hrp
                editable: true
                model: ListModel {
                    id: hrplist
                    ListElement { text: "rms" }
                    ListElement { text: "iota" }
                    ListElement { text: "smr" }
                }
                onAccepted: {
                    if (find(editText) === -1)
                        hrplist.append({text: editText})
                }
            }

            Label
            {
                text:"account:"
            }
            SpinBox {
                id:accountnumber
                value: 0
                from:0
                to: 429496729
            }
        }
        ListView
        {
            id: listview
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 100
            Layout.maximumWidth: parent.width*0.8
            Layout.alignment: Qt.AlignHCenter
            delegate:QrText
            {
                required property int addressindex
                property var vec:[accountnumber.value,0,addressindex];
                text: Account.getAddrBech32(vec,hrp.currentText)
                popWidth:275
                popHeight:400
                width:listview.width
            }

            clip:true
            model:addressmodel
        }
        ListModel
        {
            id:addressmodel
        }
        Button
        {
            Layout.alignment: Qt.AlignRight
            text:qsTr("Generate next index")
            onClicked:
            {
                addressmodel.append({"addressindex":addressmodel.count});
            }
        }




    }






}
