import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
	id: root

	property string name: ""
	property real spacing: 0
	property int margins: 0

	signal deleteClicked()

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

	RowLayout {
		anchors.fill: parent
		anchors.margins: root.margins
		spacing: root.spacing

		Text {
			id: normal_text
			text: name
			Layout.fillWidth: true
			font.pixelSize: 14
			verticalAlignment: Text.AlignVCenter

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
