import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
	SystemPalette { id: sysPalette }

	border.color: sysPalette.midlight
	color: sysPalette.alternateBase

	id: root

	signal deleteClicked()

	function getColor(): color {
		if (completed_box.checked === true) {
			return sysPalette.gray
		}
	}

	function enableInput(): void {
		text_field.visible = true
		text_field.forceActiveFocus()
		text_field.selectAll()
		normal_text.visible = false
	}

	function disableInput(): void {
		text_field.visible = false
		normal_text.visible = true
	}

	property string name: ""
	property bool completed: false
	property real spacing: 0
	property int margins: 0

	RowLayout {
		anchors.fill: parent
		anchors.margins: root.margins
		spacing: root.spacing

		CheckBox {
			id: completed_box
			checked: completed
			text: ""
			onClicked: root.completed = checked
			indicator.opacity: completed_box.checked ? 0.5 : 1

		}

		Text {
			id: normal_text
			text: name
			Layout.fillWidth: true
			font.pixelSize: 14
			verticalAlignment: Text.AlignVCenter
			color: completed_box.checked ? sysPalette.mid : sysPalette.text

			MouseArea {
				anchors.fill: parent
				onPressAndHold: root.enableInput()
			}
		}

		TextField {
			id: text_field
			text: name
			visible: false
			Layout.fillWidth: true
			font.pixelSize: 14
			verticalAlignment: Text.AlignVCenter
			onEditingFinished: {
				name = text
				root.disableInput()
			}
		}

		Button {
			text: "Delete"
			onClicked: root.deleteClicked()
		}
	}
}
