import QtQuick 2.8
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQml 2.2

ApplicationWindow {
    visible: true
    width: 950
    height: 900
    title: qsTr("AgvServerTest")

    header: Rectangle{
        id:headRect
        width: 800
        height: 50
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            id:rfidLabel
            width: 100
            anchors.verticalCenter:parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 100
            color: "red"
            text: qsTr("RFID号")
        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: rfidLabel.right
            anchors.leftMargin: 100
            width: 100
            id:orderLabel
            color: "red"
            text: qsTr("指令")

        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: orderLabel.right
            anchors.leftMargin: 100
            width: 100
            id:paramLabel
            color: "red"
            text: qsTr("参数")
        }

        Rectangle{
            id:btnAdd
            width: 100
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height-20
            anchors.left: paramLabel.right
            anchors.leftMargin: 100
            border.width: 1
            color: "gray"
            Text {
                text: qsTr("添加")
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    btnAdd.color = "lightgray"
                }
                onExited: {
                    btnAdd.color = "gray"
                }
                onClicked: {
                    orderListModel.append({"rfid": "", "order":0,param:"0"})
                }
            }
        }
    }

    ListView {
        width: parent.width
        anchors.top:headRect.bottom
        anchors.topMargin: 20
        anchors.bottom:footerRect.top
        anchors.bottomMargin: 20
        model: orderListModel
        ScrollBar.vertical: ScrollBar {}
        delegate:Component {
            id: contactDelegate
            Rectangle{
                anchors.horizontalCenter: parent.horizontalCenter
                id:delegateRect
                property int visualIndex: index
                width: 800
                height: rfidInput.height+30

                TextInput {
                    width: 200
                    anchors.verticalCenter:parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    id: rfidInput
                    text: param
                    validator:RegExpValidator{regExp: /[0-9]{0,10}/}
                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: -10
                        color: "transparent"
                        border.width: 1
                    }
                    onTextChanged: {
                        if(rfidInput.text.length>0){
                            var rfid = parseInt(rfidInput.text)
                            orderListModel.modifyRfid(rfid,delegateRect.visualIndex)
                        }
                    }
                }

                ComboBox {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: rfidInput.right
                    anchors.leftMargin: 20
                    id:orderInput
                    width: 200
                    model: [qsTr("停止"), qsTr("前进"), qsTr("后退"), qsTr("左转"), qsTr("右转"), qsTr("MP3文件"), qsTr("MP3音量"), qsTr("前进到磁条检出"), qsTr("后退到磁条检出"), qsTr("升降插齿"), qsTr("不打断当前执行卡") ]
                    currentIndex:cmd
                    onCurrentTextChanged: {
                        orderListModel.modifyCmd(orderInput.currentIndex,delegateRect.visualIndex)
                    }
                }


                TextInput {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: orderInput.right
                    anchors.leftMargin: 20
                    id: paramInput
                    text: param
                    width: 200
                    validator:RegExpValidator{regExp: /[0-9]{1,10}/}
                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: -10
                        color: "transparent"
                        border.width: 1
                    }
                    onTextChanged: {
                        if(paramInput.text.length>0){
                            var param = parseInt(paramInput.text)
                            orderListModel.modifyParam(param,delegateRect.visualIndex)
                        }
                    }
                }

                Rectangle{
                    id:btnDelete
                    width: 100
                    anchors.verticalCenter: parent.verticalCenter
                    height: 30
                    anchors.left: paramInput.right
                    anchors.leftMargin: 20
                    border.width: 1
                    color: "gray"
                    Text {
                        text: qsTr("删除")
                        anchors.centerIn: parent
                    }
                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: {
                            btnDelete.color = "lightgray"
                        }
                        onExited: {
                            btnDelete.color = "gray"
                        }
                        onClicked: {
                            orderListModel.remove(delegateRect.visualIndex)
                        }
                    }
                }
            }
        }
    }

    footer: Rectangle{
        id:footerRect
        width: parent.width
        height:100
        Rectangle{
            id:btnExcute
            width: 250
            height: parent.height-50
            anchors.right: btnStop.left
            anchors.rightMargin: 20
            border.width: 1
            color: "gray"
            Text {
                text: qsTr("执行")
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    btnExcute.color = "lightgray"
                }
                onExited: {
                    btnExcute.color = "gray"
                }
                onClicked: {
                    //TODO
                    msgCenter.sendOrders(orderListModel);
                }
            }
        }
        Rectangle{
            id:btnStop
            width: 250
            height:parent.height-50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.leftMargin: 10
            border.width: 1
            color: "gray"
            Text {
                text: qsTr("停止")
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    btnStop.color = "lightgray"
                }
                onExited: {
                    btnStop.color = "gray"
                }
                onClicked: {
                    msgCenter.stopOrders();
                }
            }
        }
        Rectangle{
            id:btnClear
            width: 250
            height: parent.height-50
            anchors.left: btnStop.right
            anchors.leftMargin: 20
            border.width: 1
            color: "gray"
            Text {
                text: qsTr("清空")
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    btnClear.color = "lightgray"
                }
                onExited: {
                    btnClear.color = "gray"
                }
                onClicked: {
                    orderListModel.clear()
                }
            }
        }

        Rectangle{
            id:tipRect
            width: parent.width
            height: 50
            anchors.bottom: parent.bottom
            Text {
                id: tipText
                text: qsTr("hahhahaha")
                opacity:0
                anchors.centerIn: parent
                color: "red"
                font.pixelSize: 15

                function showStr(txt){
                    tipText.text = txt;
                    showTipAnimation.start()
                }
                SequentialAnimation {
                    id:showTipAnimation
                    //渐入
                    NumberAnimation {
                        target: tipText
                        property: "opacity"
                        duration: 800
                        easing.type: Easing.InOutQuad
                        from:0
                        to:1
                    }
                    //持续
                    PauseAnimation {
                        duration: 3000
                    }
                    //渐出
                    NumberAnimation {
                        target: tipText
                        property: "opacity"
                        duration: 800
                        easing.type: Easing.InOutQuad
                        from:1
                        to:0
                    }
                }
            }
        }
    }

    Connections{
        target:msgCenter

        onTip:{
            //tipstr
            tipText.showStr(tipstr)
        }

        onRequestFail:{
            tipText.showStr(qsTr("请求失败或超时"))
        }

    }
}
