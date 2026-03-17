import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    width: 600
    height: 500
    visible: true
    title: "Tasktree"
	SystemPalette {
		id: sysPalette
	}

	property int radius: 2

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

		RowLayout {
			Button {
				text: "Go Back"
				onClicked: backend.goBack()
			}
			Text {
				text: backend.current.id == 0 ? "Tasks" : backend.current.name
				font.pixelSize: 24
				font.bold: true
			}
		}

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: sysPalette.base
			radius: root.radius
			antialiasing: true
            border.color: sysPalette.midlight
            border.width: 1

            ListView {
                id: taskList
                anchors.fill: parent
                anchors.margins: 8
                spacing: 4
                clip: true

                model: backend.tasks

                delegate: Task {
                    spacing: 8
                    width: taskList.width
                    height: 40

					radius: root.radius
					margins: 8

					name: modelData.name
					completed: modelData.completed

					onDeleteClicked: backend.deleteTask(modelData)
					onTaskClicked: backend.current = modelData
					onNameChanged: backend.setTaskName(modelData, name)
					onCompletedChanged: backend.setTaskCompleted(modelData, completed)
                }

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AsNeeded
                }
            }
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
    }
}
