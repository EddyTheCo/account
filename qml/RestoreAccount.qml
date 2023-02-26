import account
import MyDesigns



MyFrame
{
    id:root_
    description: qsTr("Restore account")


    MyTextArea
    {
        id:seed_
        width:root_.width*0.95
        height:root_.height*0.95
        anchors.centerIn: parent
        placeholderText:qsTr("seed")
        tfield.onEditingFinished:
        {
            Account.seed=seed_.tfield.text
        }
    }



}
