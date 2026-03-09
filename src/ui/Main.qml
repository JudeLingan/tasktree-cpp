import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    width: 600
    height: 500
    visible: true
    title: "Tasktree"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Text {
            text: "Tasks"
            font.pixelSize: 24
            font.bold: true
        }

        RowLayout {
            spacing: 8
            Layout.fillWidth: true

            TextField {
                id: taskInput
                placeholderText: "Enter task name..."
                Layout.fillWidth: true
                onAccepted: addButton.clicked()
            }

            Button {
                id: addButton
                text: "Add Task"
                onClicked: {
                    if (taskInput.text.trim() !== "") {
                        backend.addTask(taskInput.text)
                        taskInput.text = ""
                        taskInput.focus = true
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f5f5f5"
            border.color: "#ddd"
            border.width: 1

            ListView {
                id: taskList
                anchors.fill: parent
                anchors.margins: 8
                spacing: 4
                clip: true

                model: backend.tasks

                delegate: Task {
                    width: taskList.width - 16
                    spacing: 8
                    height: 40
					text: modelData.name
					onDeleteClicked: backend.deleteTask(modelData)
					onNameChanged: (newName) => {
						modelData.name = newName
						backend.changeTaskName(modelData, modelData.name)
					}
                }

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AsNeeded
                }
            }
        }

        Text {
            id: selectedInfo
            text: "Ready"
            font.pixelSize: 12
            color: "#666"
        }
    }
}
