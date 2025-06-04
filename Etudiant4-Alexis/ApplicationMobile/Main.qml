import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// Fenêtre principale de l'application
ApplicationWindow {
    visible: true
    width: 800
    height: 480
    title: "Drone Control Interface"
    color: "#0F0F0F" // Fond noir

    // =============================================
    // PROPRIÉTÉS DE L'APPLICATION
    // =============================================
    property bool isDroneCameraActive: true // État de la caméra du drone
    property bool showValues: false        // Afficher/masquer les valeurs numériques
    property int joystickOffset: 80         // Marge des joysticks
    property int joystickSize: 200          // Taille des joysticks
    property int joystickReturnDuration: 150 // Durée d'animation retour au centre
    property real joystickSensitivity: 0.7  // Sensibilité des joysticks
    property real yawSensitivityFactor: 0.5 // Facteur de sensibilité du lacet
    property bool isConnected: false        // État de connexion au serveur
    property string serverAddress: "http://172.18.59.133:8080" // Adresse du serveur
    property int rthDistance: 20            // Distance de retour à la maison (RTH)

    // Valeurs des axes des joysticks
    property int gazValue: 0     // Valeur de l'axe gaz (hauteur)
    property int lacetValue: 0   // Valeur de rotation (yaw)
    property int tangageValue: 0 // Valeur d'inclinaison avant/arrière (pitch)
    property int roulisValue: 0  // Valeur d'inclinaison gauche/droite (roll)

    // =============================================
    // PALETTE DE COULEURS
    // =============================================
    property color primaryColor: "#2A7BFF"    // Bleu vif
    property color secondaryColor: "#00E396"  // Vert cyan
    property color accentColor: "#FF6B6B"     // Rouge corail
    property color darkColor: "#7c7c7c"       // Noir profond
    property color lightColor: "#F5F5F5"      // Blanc doux
    property color surfaceColor: "#7c7c7c"    // Surface sombre
    property color cardColor: "#252525"       // Couleur des cartes

    // =============================================
    // BARRE DE MENU SUPÉRIEURE
    // =============================================
    Rectangle {
        id: topBar
        width: parent.width
        height: 60
        color: "#121212" // Couleur de fond
        z: 2 // Position Z élevée pour être au-dessus des autres éléments

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            spacing: 20

            // Logo de l'application
            Rectangle {
                width: 36
                height: 36
                radius: 8
                color: primaryColor
                Layout.alignment: Qt.AlignVCenter

                Image {
                    source: "qrc:/icons/drone.svg" // Icône du drone
                    anchors.fill: parent
                    anchors.margins: 6
                    fillMode: Image.PreserveAspectFit
                }
            }

            // Titre de l'application
            Text {
                text: "Controle du drone"
                font {
                    pixelSize: 18
                    family: "Roboto"
                    weight: Font.DemiBold
                    letterSpacing: 1
                }
                color: lightColor
                Layout.alignment: Qt.AlignVCenter
            }

            // Espaceur pour pousser les éléments suivants à droite
            Item { Layout.fillWidth: true }

            // Indicateur de connexion (vert/rouge)
            Rectangle {
                Layout.preferredHeight: 32
                Layout.preferredWidth: 140
                radius: 16
                color: isConnected ? "#1E3A1E" : "#3A1E1E" // Fond vert/rouge
                border.color: isConnected ? secondaryColor : accentColor
                border.width: 1

                Row {
                    anchors.centerIn: parent
                    spacing: 8
                    Rectangle {
                        width: 12
                        height: 12
                        radius: width / 2
                        color: isConnected ? secondaryColor : accentColor
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        text: isConnected ? "CONNECTE" : "DECONNECTE"
                        color: isConnected ? secondaryColor : accentColor
                        font {
                            pixelSize: 12
                            bold: true
                            family: "Roboto"
                            letterSpacing: 0.5
                        }
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            // Bouton pour ouvrir le menu
            Button {
                id: menuButton
                text: "MENU"
                Layout.preferredWidth: 100
                Layout.preferredHeight: 36
                onClicked: menu.visible = !menu.visible // Basculer la visibilité du menu

                background: Rectangle {
                    color: parent.down ? Qt.darker(primaryColor, 1.2) : primaryColor
                    radius: 6
                }

                contentItem: Text {
                    text: parent.text
                    font {
                        pixelSize: 14
                        bold: true
                        family: "Roboto"
                    }
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }

    // =============================================
    // ZONE PRINCIPALE DE L'INTERFACE
    // =============================================
    Rectangle {
        id: mainArea
        anchors.top: topBar.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        color: darkColor

        // =========================================
        // JOYSTICK DE GAUCHE (GAZ ET LACET)
        // =========================================
        Item {
            id: leftJoystickContainer
            width: joystickSize
            height: joystickSize
            anchors.left: parent.left
            anchors.leftMargin: joystickOffset
            anchors.verticalCenter: parent.verticalCenter

            // Base du joystick
            Rectangle {
                id: leftJoystickBase
                anchors.fill: parent
                radius: width/2 // Forme circulaire
                color: surfaceColor
                border.color: Qt.lighter(surfaceColor, 1.5)
                border.width: 1

                // Grille de fond avec axes et cercles
                Canvas {
                    anchors.fill: parent
                    onPaint: {
                        var ctx = getContext("2d")
                        ctx.clearRect(0, 0, width, height)
                        ctx.strokeStyle = "#30FFFFFF" // Couleur semi-transparente
                        ctx.lineWidth = 1

                        // Axes principaux (croix)
                        ctx.beginPath()
                        ctx.moveTo(width/2, 0)
                        ctx.lineTo(width/2, height)
                        ctx.stroke()

                        ctx.beginPath()
                        ctx.moveTo(0, height/2)
                        ctx.lineTo(width, height/2)
                        ctx.stroke()

                        // Cercles concentriques
                        ctx.beginPath()
                        ctx.arc(width/2, height/2, width/4, 0, Math.PI*2)
                        ctx.stroke()

                        ctx.beginPath()
                        ctx.arc(width/2, height/2, width/2.5, 0, Math.PI*2)
                        ctx.stroke()
                    }
                }
            }

            // Curseur du joystick
            Rectangle {
                id: leftCursor
                width: 40
                height: 40
                radius: width/2 // Forme circulaire
                x: (leftJoystickContainer.width - width) / 2
                y: (leftJoystickContainer.height - height) / 2 + (leftJoystickContainer.height / 2)
                color: secondaryColor // Vert cyan
                border.color: Qt.lighter(secondaryColor, 1.2)
                border.width: 2

                // Effet de lumière intérieure
                Rectangle {
                    width: parent.width / 2
                    height: parent.height / 2
                    radius: width/2
                    anchors.centerIn: parent
                    color: Qt.lighter(parent.color, 1.8)
                    opacity: 0.6
                }
            }

            // Affichage des valeurs numériques (si activé)
            Column {
                anchors.top: parent.bottom
                anchors.topMargin: 15
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5

                Text {
                    text: showValues ? "GAZ: " + gazValue : ""
                    font {
                        pixelSize: 14
                        family: "Roboto"
                        weight: Font.Medium
                    }
                    color: lightColor
                }

                Text {
                    text: showValues ? "LACET: " + lacetValue : ""
                    font {
                        pixelSize: 14
                        family: "Roboto"
                        weight: Font.Medium
                    }
                    color: lightColor
                }
            }
        }

        // =========================================
        // JOYSTICK DE DROITE (TANGAGE ET ROULIS)
        // =========================================
        Item {
            id: rightJoystickContainer
            width: joystickSize
            height: joystickSize
            anchors.right: parent.right
            anchors.rightMargin: joystickOffset
            anchors.verticalCenter: parent.verticalCenter

            // Base du joystick
            Rectangle {
                id: rightJoystickBase
                anchors.fill: parent
                radius: width/2
                color: surfaceColor
                border.color: Qt.lighter(surfaceColor, 1.5)
                border.width: 1

                // Grille de fond similaire au joystick gauche
                Canvas {
                    anchors.fill: parent
                    onPaint: {
                        var ctx = getContext("2d")
                        ctx.clearRect(0, 0, width, height)
                        ctx.strokeStyle = "#30FFFFFF"
                        ctx.lineWidth = 1

                        // Axes principaux
                        ctx.beginPath()
                        ctx.moveTo(width/2, 0)
                        ctx.lineTo(width/2, height)
                        ctx.stroke()

                        ctx.beginPath()
                        ctx.moveTo(0, height/2)
                        ctx.lineTo(width, height/2)
                        ctx.stroke()

                        // Cercles concentriques
                        ctx.beginPath()
                        ctx.arc(width/2, height/2, width/4, 0, Math.PI*2)
                        ctx.stroke()

                        ctx.beginPath()
                        ctx.arc(width/2, height/2, width/2.5, 0, Math.PI*2)
                        ctx.stroke()
                    }
                }
            }

            // Curseur du joystick avec animations
            Rectangle {
                id: rightCursor
                width: 40
                height: 40
                radius: width/2
                x: (parent.width - width) / 2
                y: (parent.height - height) / 2
                color: accentColor // Rouge corail
                border.color: Qt.lighter(accentColor, 1.2)
                border.width: 2

                // Animations pour retour au centre
                Behavior on x {
                    NumberAnimation { duration: joystickReturnDuration; easing.type: Easing.OutQuad }
                }
                Behavior on y {
                    NumberAnimation { duration: joystickReturnDuration; easing.type: Easing.OutQuad }
                }

                // Effet de lumière intérieure
                Rectangle {
                    width: parent.width / 2
                    height: parent.height / 2
                    radius: width/2
                    anchors.centerIn: parent
                    color: Qt.lighter(parent.color, 1.8)
                    opacity: 0.6
                }
            }

            // Affichage des valeurs numériques
            Column {
                anchors.top: parent.bottom
                anchors.topMargin: 15
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5

                Text {
                    text: showValues ? "TANGAGE: " + tangageValue : ""
                    font {
                        pixelSize: 14
                        family: "Roboto"
                        weight: Font.Medium
                    }
                    color: lightColor
                }

                Text {
                    text: showValues ? "ROULIS: " + roulisValue : ""
                    font {
                        pixelSize: 14
                        family: "Roboto"
                        weight: Font.Medium
                    }
                    color: lightColor
                }
            }
        }

        // =========================================
        // PANEL CENTRAL D'INFORMATIONS
        // =========================================
        Rectangle {
            id: centerPanel
            width: 240
            height: 120
            anchors.centerIn: parent
            color: "transparent"

            // Affichage de l'heure actuelle
            Text {
                id: timeText
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                text: Qt.formatDateTime(new Date(), "hh:mm:ss")
                font {
                    pixelSize: 24
                    family: "Roboto"
                    weight: Font.Medium
                }
                color: lightColor

                // Timer pour mettre à jour l'heure chaque seconde
                Timer {
                    interval: 1000
                    running: true
                    repeat: true
                    onTriggered: timeText.text = Qt.formatDateTime(new Date(), "hh:mm:ss")
                }
            }

            // Affichage de la distance de retour à la maison
            Text {
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                text: "RTH: " + rthDistance + "m"
                font {
                    pixelSize: 16
                    family: "Roboto"
                }
                color: secondaryColor
            }
        }

        // =========================================
        // ZONE TACTILE POUR LES JOYSTICKS
        // =========================================
        MultiPointTouchArea {
            id: touchArea
            anchors.fill: parent
            minimumTouchPoints: 1
            maximumTouchPoints: 2
            mouseEnabled: true

            // Points de contact (2 doigts maximum)
            touchPoints: [
                TouchPoint { id: touch1 },
                TouchPoint { id: touch2 }
            ]

            // Gestion des événements tactiles
            onPressed: handleTouchPoints()
            onUpdated: handleTouchPoints()
            onReleased: handleRelease()

            // Suivi des touches actives
            property int activeLeftTouch: -1
            property int activeRightTouch: -1

            // Fonction pour gérer les points de contact
            function handleTouchPoints() {
                // Gestion du joystick gauche
                if (activeLeftTouch === -1) {
                    if (touch1.pressed && isInLeftJoystickArea(touch1)) {
                        activeLeftTouch = 1;
                    } else if (touch2.pressed && isInLeftJoystickArea(touch2)) {
                        activeLeftTouch = 2;
                    }
                }

                if (activeLeftTouch === 1 && touch1.pressed) {
                    updateLeftJoystick(touch1);
                } else if (activeLeftTouch === 2 && touch2.pressed) {
                    updateLeftJoystick(touch2);
                }

                // Gestion du joystick droit
                if (activeRightTouch === -1) {
                    if (touch1.pressed && isInRightJoystickArea(touch1) && activeLeftTouch !== 1) {
                        activeRightTouch = 1;
                    } else if (touch2.pressed && isInRightJoystickArea(touch2) && activeLeftTouch !== 2) {
                        activeRightTouch = 2;
                    }
                }

                if (activeRightTouch === 1 && touch1.pressed) {
                    updateRightJoystick(touch1);
                } else if (activeRightTouch === 2 && touch2.pressed) {
                    updateRightJoystick(touch2);
                }

                // Envoi des commandes au serveur
                sendCombinedRequest();
            }

            // Vérifie si le toucher est dans la zone du joystick gauche
            function isInLeftJoystickArea(touch) {
                var pos = mapToItem(leftJoystickContainer, touch.x, touch.y);
                return leftJoystickContainer.contains(Qt.point(pos.x, pos.y));
            }

            // Vérifie si le toucher est dans la zone du joystick droit
            function isInRightJoystickArea(touch) {
                var pos = mapToItem(rightJoystickContainer, touch.x, touch.y);
                return rightJoystickContainer.contains(Qt.point(pos.x, pos.y));
            }

            // Met à jour la position du joystick gauche
            function updateLeftJoystick(touch) {
                var touchPos = leftJoystickContainer.mapFromItem(touchArea, touch.x, touch.y);
                var centerX = leftJoystickContainer.width / 2;
                var centerY = leftJoystickContainer.height / 2;
                var radius = leftJoystickContainer.width / 2 - 20;

                // Calcul de la valeur gaz (vertical)
                var normalizedY = Math.max(0, Math.min(100, 100 - (touchPos.y / leftJoystickContainer.height * 100)));
                gazValue = Math.round(normalizedY);

                // Position Y du curseur
                var cursorY = centerY + (50 - normalizedY) * (leftJoystickContainer.height / 100);

                // Calcul de la valeur lacet (horizontal avec sensibilité réduite)
                var yawSensitivityFactor = 0.2;
                var dx = (touchPos.x - centerX) * yawSensitivityFactor;
                var distanceX = Math.abs(dx);

                var x = centerX + dx;
                if (distanceX > radius) {
                    x = centerX + (dx > 0 ? radius : -radius);
                }

                // Mise à jour de la position du curseur
                leftCursor.x = x - leftCursor.width / 2;
                leftCursor.y = cursorY - leftCursor.height / 2;

                // Calcul de la valeur normalisée (-100 à 100)
                lacetValue = Math.round(((x - centerX) / radius) * 100);
            }

            // Met à jour la position du joystick droit
            function updateRightJoystick(touch) {
                var touchPos = rightJoystickContainer.mapFromItem(touchArea, touch.x, touch.y);
                var centerX = rightJoystickContainer.width / 2;
                var centerY = rightJoystickContainer.height / 2;
                var maxRadius = rightJoystickContainer.width / 2 - 15;

                // Zone morte pour éviter les micro-mouvements
                var deadZoneRadius = maxRadius * 0.05;
                var effectiveRadius = maxRadius - deadZoneRadius;

                var dx = touchPos.x - centerX;
                var dy = touchPos.y - centerY;
                var distance = Math.sqrt(dx * dx + dy * dy);

                // Si dans la zone morte, réinitialiser
                if (distance < deadZoneRadius) {
                    rightCursor.x = centerX - rightCursor.width / 2;
                    rightCursor.y = centerY - rightCursor.height / 2;
                    tangageValue = 0;
                    roulisValue = 0;
                    return;
                }

                // Calcul de la position angulaire et distance normalisée
                var angle = Math.atan2(dy, dx);
                var normalizedDistance = Math.min(1, (distance - deadZoneRadius) / effectiveRadius);
                var adjustedDistance = normalizedDistance * maxRadius;

                // Position finale du curseur
                var x = centerX + Math.cos(angle) * adjustedDistance;
                var y = centerY + Math.sin(angle) * adjustedDistance;

                rightCursor.x = x - rightCursor.width / 2;
                rightCursor.y = y - rightCursor.height / 2;

                // Calcul des valeurs normalisées (-100 à 100)
                tangageValue = Math.round(-(y - centerY) / effectiveRadius * 100);
                roulisValue = Math.round((x - centerX) / effectiveRadius * 100);
            }

            // Réinitialise les joysticks au centre
            function resetReturnToCenter() {
                leftCursor.x = (leftJoystickContainer.width - leftCursor.width) / 2;
                lacetValue = 0;

                rightCursor.x = (rightJoystickContainer.width - rightCursor.width) / 2;
                rightCursor.y = (rightJoystickContainer.height - rightCursor.height) / 2;
                tangageValue = 0;
                roulisValue = 0;
            }

            // Gère le relâchement des touches
            function handleRelease() {
                // Joystick gauche
                if ((activeLeftTouch === 1 && !touch1.pressed) ||
                        (activeLeftTouch === 2 && !touch2.pressed)) {
                    activeLeftTouch = -1;
                    leftCursor.x = (leftJoystickContainer.width - leftCursor.width) / 2;
                    lacetValue = 0;
                }

                // Joystick droit
                if ((activeRightTouch === 1 && !touch1.pressed) ||
                        (activeRightTouch === 2 && !touch2.pressed)) {
                    activeRightTouch = -1;
                    rightCursor.x = (rightJoystickContainer.width - rightCursor.width) / 2;
                    rightCursor.y = (rightJoystickContainer.height - rightCursor.height) / 2;
                    tangageValue = 0;
                    roulisValue = 0;
                }

                // Envoi des dernières commandes
                sendCombinedRequest();
            }
        }

        // =========================================
        // PRÉVISUALISATION DE LA CAMÉRA
        // =========================================
        Rectangle {
            id: cameraPreview
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: 240
            height: 135
            radius: 6
            color: isDroneCameraActive ? "transparent" : surfaceColor
            visible: !isDroneCameraActive
            border.color: "#333"
            border.width: 1

            // Texte de remplacement si la caméra est inactive
            Text {
                anchors.centerIn: parent
                text: "CAMERA FEED"
                color: "#666"
                font {
                    pixelSize: 14
                    family: "Roboto"
                    letterSpacing: 1
                }
                visible: !isDroneCameraActive
            }

            // Bouton de rafraîchissement
            Button {
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 6
                width: 28
                height: 28
                text: "↻"
                font.pixelSize: 14
                visible: !isDroneCameraActive

                background: Rectangle {
                    radius: width/2
                    color: parent.down ? Qt.darker(primaryColor, 1.2) : primaryColor
                }

                contentItem: Text {
                    text: parent.text
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: console.log("Refresh camera feed")
            }
        }
    }

    // =============================================
    // MENU CONTEXTUEL
    // =============================================
    Menu {
        id: menu
        width: 220
        visible: false
        topPadding: 8
        bottomPadding: 8
        leftPadding: 8
        rightPadding: 8

        background: Rectangle {
            color: surfaceColor
            radius: 6
            border.color: "#333"
            border.width: 1
        }

        // Style des items du menu
        delegate: MenuItem {
            id: menuItem
            implicitHeight: 40
            implicitWidth: 200

            background: Rectangle {
                color: menuItem.highlighted ? "#333" : "transparent"
                radius: 4
            }

            contentItem: Text {
                text: menuItem.text
                font.pixelSize: 14
                font.family: "Roboto"
                color: menuItem.highlighted ? "white" : "#AAAAAA"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                leftPadding: 12
            }

            // Indicateur pour les items cochés
            indicator: Item {
                implicitWidth: 24
                implicitHeight: 24
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.verticalCenter: parent.verticalCenter

                Rectangle {
                    visible: menuItem.checked
                    width: 12
                    height: 12
                    radius: 6
                    anchors.centerIn: parent
                    color: secondaryColor
                }
            }
        }

        // Séparateur
        MenuSeparator {
            contentItem: Rectangle {
                implicitHeight: 1
                color: "#333"
            }
        }

        // Sous-menu pour le retour à la maison
        Menu {
            id: rthMenu
            title: "RETURN TO HOME"
            width: 240

            background: Rectangle {
                color: surfaceColor
                radius: 6
                border.color: "#333"
                border.width: 1
            }

            // Option pour définir la distance RTH
            MenuItem {
                text: "Set RTH Distance"
                onTriggered: rthDistanceWindow.visible = true
            }

            // Option pour exécuter le RTH
            MenuItem {
                text: "Execute RTH (" + rthDistance + "m)"
                onTriggered: sendCombinedRequest(true)
            }
        }

        MenuSeparator {}

        // Option pour afficher/masquer les valeurs
        MenuItem {
            id: toggleValuesButton
            text: showValues ? "Cacher les valeurs" : "Montrer les valeurs"
            onTriggered: showValues = !showValues
        }

        // Option pour ouvrir les paramètres du serveur
        MenuItem {
            text: "Paramètres du serveur"
            onTriggered: serverWindow.visible = true
        }

        MenuSeparator {}

        // Option pour quitter l'application
        MenuItem {
            text: "Quitter"
            onTriggered: Qt.quit()
            contentItem: Text {
                text: parent.text
                color: accentColor
                font.pixelSize: 14
                font.family: "Roboto"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                leftPadding: 12
            }
        }

        // =========================================
        // FENÊTRE DE CONFIGURATION DU SERVEUR
        // =========================================
        Window {
            id: serverWindow
            width: 400
            height: 200
            minimumWidth: 400
            minimumHeight: 200
            visible: false
            title: "CONFIGURATION DU SERVEUR"
            modality: Qt.ApplicationModal // Fenêtre modale
            color: surfaceColor

            Rectangle {
                anchors.fill: parent
                anchors.margins: 1
                color: "transparent"
                border.color: "#333"
                border.width: 1
                radius: 6
            }

            Column {
                spacing: 16
                anchors.centerIn: parent
                width: parent.width - 40

                // Titre
                Label {
                    text: "ADRESSE DU SERVEUR"
                    font {
                        pixelSize: 12
                        family: "Roboto"
                        letterSpacing: 1
                        bold: true
                    }
                    color: "#AAAAAA"
                }

                // Champ de saisie de l'adresse
                TextField {
                    id: serverInput
                    width: parent.width
                    height: 40
                    text: serverAddress
                    placeholderText: "ex: 192.168.1.100:8080"
                    font.pixelSize: 14
                    padding: 12
                    color: "white"
                    selectionColor: primaryColor
                    selectedTextColor: "white"

                    background: Rectangle {
                        color: "#252525"
                        radius: 4
                        border.color: serverInput.focus ? primaryColor : "#333"
                        border.width: 1
                    }
                }

                // Boutons Annuler/Enregistrer
                Row {
                    spacing: 10
                    anchors.right: parent.right

                    Button {
                        text: "ANNULE"
                        width: 100
                        height: 36
                        onClicked: serverWindow.visible = false

                        background: Rectangle {
                            color: parent.down ? "#333" : "#252525"
                            radius: 4
                            border.color: "#333"
                            border.width: 1
                        }

                        contentItem: Text {
                            text: parent.text
                            color: "white"
                            font.pixelSize: 14
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    Button {
                        text: "ENREGISTRER"
                        width: 100
                        height: 36
                        onClicked: {
                            serverAddress = serverInput.text
                            console.log("Nouvelle adresse du serveur : " + serverAddress)
                            serverWindow.visible = false
                        }

                        background: Rectangle {
                            color: parent.down ? Qt.darker(primaryColor, 1.2) : primaryColor
                            radius: 4
                        }

                        contentItem: Text {
                            text: parent.text
                            color: "white"
                            font.pixelSize: 14
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }

        // =========================================
        // FENÊTRE DE CONFIGURATION RTH
        // =========================================
        Window {
            id: rthDistanceWindow
            width: 400
            height: 220
            minimumWidth: 400
            minimumHeight: 220
            visible: false
            title: "RTH Distance"
            modality: Qt.ApplicationModal
            color: surfaceColor

            Rectangle {
                anchors.fill: parent
                anchors.margins: 1
                color: "transparent"
                border.color: "#333"
                border.width: 1
                radius: 6
            }

            Column {
                spacing: 16
                anchors.centerIn: parent
                width: parent.width - 40

                // Titre
                Label {
                    text: "RETURN TO HOME DISTANCE (20m - 100m)"
                    font {
                        pixelSize: 12
                        family: "Roboto"
                        letterSpacing: 1
                        bold: true
                    }
                    color: "#AAAAAA"
                }

                // Slider pour choisir la distance
                Slider {
                    id: rthDistanceSlider
                    width: parent.width
                    from: 20
                    to: 100
                    stepSize: 1
                    value: rthDistance
                    onValueChanged: rthDistanceValue.text = Math.round(value) + "m"

                    // Style du slider
                    background: Rectangle {
                        x: parent.leftPadding
                        y: parent.topPadding + parent.availableHeight / 2 - height / 2
                        implicitWidth: parent.width
                        implicitHeight: 4
                        width: parent.availableWidth
                        height: implicitHeight
                        radius: 2
                        color: "#252525"

                        Rectangle {
                            width: parent.width * (rthDistanceSlider.visualPosition)
                            height: parent.height
                            color: secondaryColor
                            radius: 2
                        }
                    }

                    // Style du curseur
                    handle: Rectangle {
                        x: rthDistanceSlider.leftPadding + rthDistanceSlider.visualPosition * (rthDistanceSlider.availableWidth - width)
                        y: rthDistanceSlider.topPadding + rthDistanceSlider.availableHeight / 2 - height / 2
                        implicitWidth: 18
                        implicitHeight: 18
                        radius: 9
                        color: rthDistanceSlider.pressed ? Qt.lighter(secondaryColor, 1.2) : secondaryColor
                        border.color: Qt.lighter(secondaryColor, 1.4)
                        border.width: 1
                    }
                }

                // Affichage de la valeur actuelle
                Text {
                    id: rthDistanceValue
                    text: rthDistance + "m"
                    font {
                        pixelSize: 18
                        family: "Roboto"
                        bold: true
                    }
                    color: secondaryColor
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                // Boutons Annuler/Enregistrer
                Row {
                    spacing: 10
                    anchors.right: parent.right

                    Button {
                        text: "CANCEL"
                        width: 100
                        height: 36
                        onClicked: rthDistanceWindow.visible = false

                        background: Rectangle {
                            color: parent.down ? "#333" : "#252525"
                            radius: 4
                            border.color: "#333"
                            border.width: 1
                        }

                        contentItem: Text {
                            text: parent.text
                            color: "white"
                            font.pixelSize: 14
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    Button {
                        text: "SAVE"
                        width: 100
                        height: 36
                        onClicked: {
                            rthDistance = Math.round(rthDistanceSlider.value)
                            console.log("New RTH distance: " + rthDistance + "m")
                            rthDistanceWindow.visible = false
                        }

                        background: Rectangle {
                            color: parent.down ? Qt.darker(primaryColor, 1.2) : primaryColor
                            radius: 4
                        }

                        contentItem: Text {
                            text: parent.text
                            color: "white"
                            font.pixelSize: 14
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }
    }

    // =============================================
    // FONCTIONS DE COMMUNICATION
    // =============================================

    // Envoie une requête combinée au serveur avec toutes les valeurs
    function sendCombinedRequest(isRestartClicked) {
        var restartValue = isRestartClicked ? 1 : 0
        // Construction de l'URL avec tous les paramètres
        var url = serverAddress + "?gaz=" + gazValue / 50 +
                "&lacet=" + lacetValue /300 +
                "&tangage=" + tangageValue /300 +
                "&roulis=" + roulisValue /300 +
                "&restart=" + restartValue +
                "&rth_distance=" + rthDistance
        console.log("Sending request: " + url)
        // Envoi de la requête HTTP
        var xhr = new XMLHttpRequest()
        xhr.open("GET", url, true)
        xhr.send()
        console.log("Gaz:", gazValue, "Lacet:", lacetValue, "Tangage:", tangageValue, "Roulis:", roulisValue);
    }

    // Timer pour vérifier périodiquement la connexion au serveur
    Timer {
        interval: 5000 // Toutes les 5 secondes
        running: true
        repeat: true
        onTriggered: {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "http://172.18.59.133:8080/?", true);
            xhr.onreadystatechange = function() {
                if (xhr.readyState === XMLHttpRequest.DONE) {
                    if (xhr.status === 200) {
                        isConnected = true;
                    } else {
                        isConnected = false;
                    }
                }
            }
            xhr.send();
        }
    }
}
