import QtQuick 2.8
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQml 2.2

ApplicationWindow {
    visible: true
    width: 950
    height: 900
    title: qsTr("AgvServerTest")

    ListModel {
        id:listModel
        ListElement {
            rfid:""
            order: 0
            param:"0"
        }
    }

    header: Rectangle{
        id:headRect
        width: 800
        height: 50
        Text {
            id:rfidLabel
            width: 200
            anchors.verticalCenter:parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 80
            color: "red"
            text: qsTr("RFID号")
        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: rfidLabel.right
            anchors.leftMargin: 20
            width: 200
            id:orderLabel
            color: "red"
            text: qsTr("指令")

        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: orderLabel.right
            anchors.leftMargin: 20
            width: 200
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
                    listModel.append({"rfid": "", "order":0,param:"0"})
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
        model: listModel
        ScrollBar.vertical: ScrollBar {}
        delegate:Component {
            id: contactDelegate
            Rectangle{
                id:delegateRect
                property int visualIndex: index
                width: 800
                height: rfidInput.height+10
                ComboBox {
                    id:rfidInput
                    width: 200
                    anchors.verticalCenter:parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10

                    model: [qsTr("立即"), qsTr("空") ]
                    editable: true
                    editText:rfid
                }

                ComboBox {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: rfidInput.right
                    anchors.leftMargin: 10
                    id:orderInput
                    width: 200
                    model: [qsTr("停止"), qsTr("前进"), qsTr("后退"), qsTr("左转"), qsTr("右转"), qsTr("MP3文件"), qsTr("MP3音量"), qsTr("前进到磁条检出"), qsTr("后退到磁条检出"), qsTr("升降插齿"), qsTr("不打断当前执行卡") ]
                    currentIndex:order
                }


                TextEdit {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: orderInput.right
                    anchors.leftMargin: 20
                    id: paramInput
                    text: param
                    width: 200
                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: -10
                        color: "transparent"
                        border.width: 1
                    }
                }

                Rectangle{
                    id:btnDelete
                    width: 100
                    anchors.verticalCenter: parent.verticalCenter
                    height: parent.height-20
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
                            //TODO
                            console.log("visualIndex="+delegateRect.visualIndex);
                            listModel.remove(delegateRect.visualIndex)
                        }
                    }
                }
            }
        }
    }

    footer: Rectangle{
        id:footerRect
        width: parent.width
        height:70
        Rectangle{
            id:btnExcute
            width: 250
            height: parent.height
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
                }
            }
        }
        Rectangle{
            id:btnStop
            width: 250
            height: parent.height
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
                    //TODO

                }
            }
        }
        Rectangle{
            id:btnClear
            width: 250
            height: parent.height
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
                    listModel.clear()
                }
            }
        }
    }


    Connections{
        target:msgCenter

    }
}
