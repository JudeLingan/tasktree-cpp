import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
	id: root
	property string text: ""
	signal deleteClicked()
	signal nameChanged(newName: string)

	function enableInput(): void {
		text_field.text = text
		text_field.visible = true
		normal_text.visible = false
	}

	function disableInput(): void {
		text_field.visible = false
		normal_text.visible = true
	}

	onTextChanged: {
		text_field.text = text
	}

	Text {
		id: normal_text
		text: root.text
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
		text: ""
		visible: false
		Layout.fillWidth: true
		font.pixelSize: 14
		verticalAlignment: Text.AlignVCenter
		onEditingFinished: {
			root.text = text
			root.nameChanged(text)
			root.disableInput()
		}
	}

	Button {
		text: "Delete"
		onClicked: root.deleteClicked()
	}
}
