import account
import MyDesigns


MyTextArea
{
    id:seed_
    textarea.placeholderText: qsTr("Enter seed")
    textarea.onEditingFinished:
    {
        Account.seed=seed_.textarea.text
    }
    textarea.focus: true
}




