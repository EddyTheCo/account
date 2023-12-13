import QtQuick
import QtQuick.Controls
import Esterv.Iota.Account
import Esterv.Styles.Simple


TextArea
{
    id:seed_
    text:Account.seed
    wrapMode:Text.WrapAnywhere
    onEditingFinished:
    {
        Account.seed=seed_.text
    }
}



