import QtQuick 2.1
import QtQuick.Layouts 1.0
import VideoTexture 1.0

Rectangle {
    id: root
    width: 1280
    height: 720
    
    GridLayout {
        width: parent.width
        height: parent.height
        columnSpacing: 0
        rowSpacing: 0
        columns: 2
        
        VideoTexture {
            id: videoTexture0
            objectName: "videoTexture0"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        VideoTexture {
            id: videoTexture1
            objectName: "videoTexture1"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        VideoTexture {
            id: videoTexture2
            objectName: "videoTexture2"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        VideoTexture {
            id: videoTexture3
            objectName: "videoTexture3"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        /*
        VideoTexture {
            id: videoTexture4
            Layout.fillWidth: true
            Layout.fillHeight: true
            Connections { onMemoryAddressChanged: root.memoryAddressChanged(4, address) }
        }
        VideoTexture {
            id: videoTexture5
            Layout.fillWidth: true
            Layout.fillHeight: true
            Connections { onMemoryAddressChanged: root.memoryAddressChanged(5, address) }
        }
        VideoTexture {
            id: videoTexture6
            Layout.fillWidth: true
            Layout.fillHeight: true
            Connections { onMemoryAddressChanged: root.memoryAddressChanged(6, address) }
        }
        VideoTexture {
            id: videoTexture7
            Layout.fillWidth: true
            Layout.fillHeight: true
            Connections { onMemoryAddressChanged: root.memoryAddressChanged(7, address) }
        }
        VideoTexture {
            id: videoTexture8
            Layout.fillWidth: true
            Layout.fillHeight: true
            Connections { onMemoryAddressChanged: root.memoryAddressChanged(8, address) }
        }
        VideoTexture {
            id: videoTexture9
            Layout.fillWidth: true
            Layout.fillHeight: true
            Connections { onMemoryAddressChanged: root.memoryAddressChanged(9, address) }
        }
        VideoTexture {
            id: videoTexture10
            Layout.fillWidth: true
            Layout.fillHeight: true
            Connections { onMemoryAddressChanged: root.memoryAddressChanged(10, address) }
        }
        VideoTexture {
            id: videoTexture11
            Layout.fillWidth: true
            Layout.fillHeight: true
            Connections { onMemoryAddressChanged: root.memoryAddressChanged(11, address) }
        }
        VideoTexture {
            id: videoTexture12
            Layout.fillWidth: true
            Layout.fillHeight: true
            Connections { onMemoryAddressChanged: root.memoryAddressChanged(12, address) }
        }
        VideoTexture {
            id: videoTexture13
            Layout.fillWidth: true
            Layout.fillHeight: true
            Connections { onMemoryAddressChanged: root.memoryAddressChanged(13, address) }
        }
        VideoTexture {
            id: videoTexture14
            Layout.fillWidth: true
            Layout.fillHeight: true
            Connections { onMemoryAddressChanged: root.memoryAddressChanged(14, address) }
        }
        VideoTexture {
            id: videoTexture15
            Layout.fillWidth: true
            Layout.fillHeight: true
            Connections { onMemoryAddressChanged: root.memoryAddressChanged(15, address) }
        }
        */
    }
}

