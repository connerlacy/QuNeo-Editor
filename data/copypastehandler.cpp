#include "copypastehandler.h"


CopyPasteHandler::CopyPasteHandler(PresetHandler *presetHandle, MidiDeviceAccess *mda, QWidget *mainWindow, DataValidator* dV,QObject *parent) :
    QObject(parent)
{
    //----- Preset Handler -----//
    handlerOfPresets = presetHandle;

    midiDeviceAccess = mda;
    mWindow = mainWindow;
    dataValidator = dV;


    menuBar = new QMenuBar(mainWindow);

    #ifdef Q_OS_MAC

    #else
    menuBar->resize(mWindow->width(), 25);
    #endif

    fwProgressOpen = false;

    connect(mWindow, SIGNAL(sigFwProgressDialogOpen(bool)), this, SLOT(slotProgressDialogOpen(bool)));
    connect(mda, SIGNAL(sigQuNeoConnected(bool)), this, SLOT(slotQuNeoConnected(bool)));

    //aboutScreen.setText("QuNeo | Keith McMillen Instruments\n\nSoftware Firmware Version: \nHardware Firmware Version: ");

    //----- FILE MENU ------//
    fileMenu = menuBar->addMenu(tr("&File"));
    importPreset = new QAction(tr("&Import Preset..."), this);
    exportPreset = new QAction(tr("&Export Preset..."), this);
    exportAllPresets = new QAction(tr("&Export All Presets..."), this);
    savePreset = new QAction(tr("&Save Preset"), this);
    saveAllPresets = new QAction(tr("&Save All Presets"), this);

    fileMenu->addAction(savePreset);
    fileMenu->addAction(saveAllPresets);
    fileMenu->addAction(importPreset);
    fileMenu->addAction(exportPreset);
    fileMenu->addAction(exportAllPresets);

    connect(exportPreset, SIGNAL(triggered()), this, SLOT(slotExportPreset()));
    connect(importPreset, SIGNAL(triggered()), this, SLOT(slotImportPreset()));
    connect(savePreset, SIGNAL(triggered()), handlerOfPresets, SLOT(slotSave()));
    connect(saveAllPresets, SIGNAL(triggered()), handlerOfPresets, SLOT(slotSaveAllPresets()));
    connect(exportAllPresets, SIGNAL(triggered()), this, SLOT(slotExportAllPresets()));

    //------ EDIT MENU -------//
    editMenu = menuBar->addMenu(tr("&Edit"));
    pasteAct = new QAction(tr("&Paste to Current Preset"), this);
    copyAct = new QAction(tr("&Copy Current Preset"), this);
    copySensorAct = new QAction(tr("&Copy Sensor"), this);
    pasteSensorAct = new QAction(tr("&Paste Sensor"), this);
    clearPreset = new QAction(tr("&Clear Current Preset"), this);
    revertFactoryAct = new QAction(tr("&Revert Current Preset to Factory"), this);
    revertAllPresets = new QAction(tr("&Revert All Presets to Factory"), this);

    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(copySensorAct);
    editMenu->addAction(pasteSensorAct);
    editMenu->addSeparator();
    editMenu->addAction(clearPreset);
    editMenu->addAction(revertFactoryAct);
    editMenu->addAction(revertAllPresets);
    editMenu->addSeparator();


    connect(copyAct, SIGNAL(triggered()), this, SLOT(copyPreset()));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(pastePreset()));
    connect(copySensorAct, SIGNAL(triggered()), this, SLOT(slotCopySensor()));
    connect(pasteSensorAct, SIGNAL(triggered()), this, SLOT(slotPasteSensor()));
    connect(revertFactoryAct, SIGNAL(triggered()), this, SLOT(slotLoadFactoryCurrent()));
    connect(revertAllPresets, SIGNAL(triggered()), this, SLOT(slotLoadFactoryAll()));
    connect(clearPreset, SIGNAL(triggered()), this, SLOT(slotImportBlankPreset()));

    // Key Commands
    copySensorAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    pasteSensorAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    copyAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C));
    pasteAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_V));
    savePreset->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    saveAllPresets->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));

    //qDebug() << "copied preset size" << presetCopiedMap.keys();
    pasteAct->setDisabled(true);
    pasteSensorAct->setDisabled(true);
    copySensorAct->setDisabled(true);
    copyAct->setDisabled(true);
    revertFactoryAct->setDisabled(true);
    clearPreset->setDisabled(true);
    revertAllPresets->setDisabled(true);
    importPreset->setDisabled(true);
    exportPreset->setDisabled(true);
    exportAllPresets->setDisabled(true);
    savePreset->setDisabled(true);
    saveAllPresets->setDisabled(true);

    //---- DRUM STYLES ----//
    drumStylesMenu = menuBar->addMenu(tr("&Drum Styles"));
    styleDefault = new QAction(tr("Default"), this);
    styleMachine = new QAction(tr("Machine"), this);
    styleAkaMP = new QAction(tr("AkaMP"), this);
    styleKorPad = new QAction(tr("KorPad"), this);
    styleFullLevel = new QAction(tr("Full Level"), this);
    drumStylesMenu->addAction(styleDefault);
    drumStylesMenu->addAction(styleMachine);
    drumStylesMenu->addAction(styleAkaMP);
    drumStylesMenu->addAction(styleKorPad);
    drumStylesMenu->addAction(styleFullLevel);
    connect(styleDefault, SIGNAL(triggered()), handlerOfPresets->padEditPane, SLOT(slotStyleDefault()));
    connect(styleMachine, SIGNAL(triggered()), handlerOfPresets->padEditPane, SLOT(slotStyleMachine()));
    connect(styleAkaMP, SIGNAL(triggered()), handlerOfPresets->padEditPane, SLOT(slotStyleAkaMP()));
    connect(styleKorPad, SIGNAL(triggered()), handlerOfPresets->padEditPane, SLOT(slotStyleKorPad()));
    connect(styleFullLevel, SIGNAL(triggered()), handlerOfPresets->padEditPane, SLOT(slotStyleFullLevel()));

    //------ HARDWARE MENU -------//
    hardwareMenu = menuBar->addMenu(tr("&Hardware"));

    updateFirmware = new QAction(tr("&Update Firmware..."), this);
    connect(updateFirmware, SIGNAL(triggered()), this, SLOT(slotUpdateFirmware()));
    hardwareMenu->addAction(updateFirmware);
    hardwareMenu->addSeparator();

    swapPadLeds = new QAction(tr("&Swap Pad LEDs"), this);
    connect(swapPadLeds, SIGNAL(triggered()), mda, SLOT(slotSwapLeds()));
    hardwareMenu->addAction(swapPadLeds);

    toggleProgramChangeOut = new QAction(tr("&Program Change Out"), this);
    connect(toggleProgramChangeOut, SIGNAL(triggered()), mda, SLOT(slotSendToggleProgramChangeOutput()));
    hardwareMenu->addAction(toggleProgramChangeOut);

    toggleProgramChangeIn = new QAction(tr("&Program Change In"), this);
    connect(toggleProgramChangeIn, SIGNAL(triggered()), mda, SLOT(slotSendToggleProgramChangeInput()));
    hardwareMenu->addAction(toggleProgramChangeIn);

    //----- HELP MENU ------//
    helpMenu = menuBar->addMenu(tr("&Help"));
    aboutQuNeo = new QAction(tr("&About QuNeo"), this);
    quNeoManual = new QAction(tr("&Documentaion"), this);
    helpMenu->addAction(aboutQuNeo);
    helpMenu->addAction(quNeoManual);

    connect(aboutQuNeo, SIGNAL(triggered()), this, SLOT(slotOpenAbout()));
    connect(quNeoManual, SIGNAL(triggered()), this, SLOT(slotOpenDocumentation()));

    slotSetCurrentSensor(QString("buttonButton10"));
}


void CopyPasteHandler::copyPreset(){

    presetCopiedMap = handlerOfPresets->presetMapsCopy.value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap();
    copiedFrom = handlerOfPresets->currentPreset;
    qDebug() << "copied preset size" << presetCopiedMap.keys();
    if(presetCopiedMap.size()){
        pasteAct->setDisabled(false);
    }
}

void CopyPasteHandler::pastePreset(){
    handlerOfPresets->presetMapsCopy.insert(QString("Preset %1").arg(handlerOfPresets->currentPreset), presetCopiedMap);
    handlerOfPresets->slotRecallPreset(QString("Preset %1").arg(handlerOfPresets->currentPreset + 1));
    handlerOfPresets->slotCheckPresets();
}

void CopyPasteHandler::slotExportPreset(){

    QString filename = QFileDialog::getSaveFileName(mWindow, tr("Save Preset"), QString("./"),tr("QuNeo Preset Files (*.quneopreset)"));
    presetFile = new QFile(filename);

    presetFile->open(QIODevice::WriteOnly);

    presetByteArray = serializer.serialize(handlerOfPresets->presetMapsCopy.value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap());
    presetFile->resize(0);
    presetFile->write(presetByteArray);
    presetFile->close();
    presetByteArray.clear();
}

void CopyPasteHandler::slotExportAllPresets(){

    //get user's destination path for the 16 files.
    foldername = QFileDialog::getExistingDirectory(mWindow, tr("Select or Create a Folder for QuNeo Preset Files"),
                                                    QString("./"),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    //save each preset to a file within path held in foldername.
for(int i=0; i<16; i++){

    //load preset i
    presetByteArray = serializer.serialize(handlerOfPresets->presetMapsCopy.value(QString("Preset %1").arg(i)).toMap());
    //Get preset i's name.
    QString presetFileName = handlerOfPresets->presetMapsCopy.value(QString("Preset %1").arg(i)).toMap().value(QString("presetName")).toString();

    //validate preset name for use in creating destination file path
    if (presetFileName.contains(QString("/"))){
        //presetFileName.remove(QString("/"));
        presetFileName.replace(QString("/"), QString("_"));
    }

    //Construct destination file name using parameters presetName and foldername.
    //Open it with a Qfile and create if it doesn't exist.
    qDebug()<<QString("%1/%2.quneopreset").arg(foldername).arg(presetFileName);
    presetFile = new QFile(QString("%1/%2.quneopreset").arg(foldername).arg(presetFileName));

    //open it, clear it, write preset i into it.
    presetFile->open(QIODevice::WriteOnly);
    presetFile->resize(0);
    presetFile->write(presetByteArray);

    //cleanup. close file, clear byte array for next one.
    presetFile->close();
    presetByteArray.clear();
}

}

void CopyPasteHandler::slotImportPreset(){

    QString filename = NULL;
    filename = QFileDialog::getOpenFileName(mWindow, tr("Import Preset"), QString("./"),tr("QuNeo Preset Files (*.quneopreset)"));
    if(filename != NULL){
       presetFile = new QFile(filename);
        presetFile->open(QIODevice::ReadOnly);
        presetByteArray = presetFile->readAll();

        importedPresetMap = dataValidator->slotValidatePreset(parser.parse(presetByteArray, &ok).toMap());

        handlerOfPresets->presetMapsCopy.insert(QString("Preset %1").arg(handlerOfPresets->currentPreset), importedPresetMap);
        handlerOfPresets->slotRecallPreset(QString("Preset %1").arg(handlerOfPresets->currentPreset + 1));
        handlerOfPresets->slotCheckPresets();
        presetByteArray.clear();
        presetFile->close();
        importedPresetMap.clear();
    } else {
        qDebug("nothing selected");
    }



}

void CopyPasteHandler::slotImportBlankPreset(){

    //pathBlankPreset = QCoreApplication::applicationDirPath();
    //pathBlankPreset.remove(pathBlankPreset.length() - 5, pathBlankPreset.length());
    //pathBlankPreset.append("Resources/blank.quneopreset");
    //presetFile = new QFile(pathBlankPreset);
    presetFile = new QFile(":Quneo/preset/resources/blank.quneopreset");
    if(presetFile->open(QIODevice::ReadOnly)){
        qDebug("blank preset open");
    } else {
        qDebug("blank preset NOT OPEN");
    }

    presetByteArray = presetFile->readAll();
    importedPresetMap = dataValidator->slotValidatePreset(parser.parse(presetByteArray, &ok).toMap());

    //importedPresetMap = parser.parse(presetByteArray, &ok).toMap();

    handlerOfPresets->presetMapsCopy.insert(QString("Preset %1").arg(handlerOfPresets->currentPreset), importedPresetMap);
    handlerOfPresets->slotRecallPreset(QString("Preset %1").arg(handlerOfPresets->currentPreset + 1));
    handlerOfPresets->slotCheckPresets();
    presetByteArray.clear();
    presetFile->close();
    importedPresetMap.clear();
}

void CopyPasteHandler::slotOpenAbout(){
    aboutScreen.setText(QString("QuNeo | Keith McMillen Instruments\n\n\nExpected Firmware Version: %1\nFound Firmware Version: %2\n\nExpected Bootloader Version: %4\nFound Bootloader Version: %3")
                        .arg(midiDeviceAccess->editorVersion)
                        .arg(midiDeviceAccess->boardVersion)
                        .arg(midiDeviceAccess->boardVersionBoot)
                        .arg(midiDeviceAccess->editorVersionBoot));
    aboutScreen.open();
}

void CopyPasteHandler::slotOpenDocumentation(){

#ifdef Q_OS_MAC

    QString docFilePath = QCoreApplication::applicationDirPath();
    docFilePath.remove(docFilePath.length() - 5, docFilePath.length());
    docFilePath.append("Resources/help.txt");

    documentationLinkFile = new QFile(docFilePath);

    if(documentationLinkFile->open(QIODevice::ReadOnly)){
        //qDebug("open!");
      } else {
        //qDebug() << docFilePath;
    }

    QTextStream docTextStream(documentationLinkFile);
    QString docUrl;
    //while(!docTextStream.atEnd()){
        docUrl = docTextStream.readLine();
    //}

    qDebug() << "URL loaded?" << docUrl;

        QDesktopServices::openUrl(QUrl(docUrl));

#else

    documentationLinkFile = new QFile("./help.txt");

    if(documentationLinkFile->open(QIODevice::ReadOnly)){
        //qDebug("open!");
      } else {
        //qDebug() << docFilePath;
    }

    QTextStream docTextStream(documentationLinkFile);
    QString docUrl;
    //while(!docTextStream.atEnd()){
        docUrl = docTextStream.readLine();
    //}

    //qDebug() << "URL loaded?" << docUrl;

        QDesktopServices::openUrl(QUrl(docUrl));


#endif
}

void CopyPasteHandler::slotLoadFactoryAll(){

    QString factoryPath = QCoreApplication::applicationDirPath(); //get bundle path;

#if defined(Q_OS_MAC) && !defined(QT_DEBUG)
    factoryPath.remove(factoryPath.length() - 5, factoryPath.length());
    factoryPath.append("Resources/presets/QuNeo_FactoryPresets.json");
#else
    factoryPath = QString("./presets/QuNeo_FactoryPresets.json");
#endif

    presetFile = new QFile(factoryPath);
    presetFile->open(QIODevice::ReadOnly);
    presetByteArray = presetFile->readAll();
    importedPresetMap = parser.parse(presetByteArray, &ok).toMap();
    handlerOfPresets->presetMapsCopy = importedPresetMap["QuNeo Presets"].toMap();
    handlerOfPresets->slotRecallPreset(QString("Preset %1").arg(handlerOfPresets->currentPreset + 1));
    for(int i = 0; i < 16; i++){
        if(handlerOfPresets->updateIndicator->slotCompareMaps(i)){
            handlerOfPresets->updateIndicator->presetModified[i] = true;
            handlerOfPresets->presetMenu->setItemText(i, QString("Preset %1 *").arg(i+1));
        }
    }
    //handlerOfPresets->slotCheckPresets();
    presetByteArray.clear();
    presetFile->close();
    importedPresetMap.clear();
}

void CopyPasteHandler::slotLoadFactoryCurrent(){

    QString factoryPath = QCoreApplication::applicationDirPath(); //get bundle path;

#if defined(Q_OS_MAC) && !defined(QT_DEBUG)
    factoryPath.remove(factoryPath.length() - 5, factoryPath.length());
    factoryPath.append("Resources/presets/QuNeo_FactoryPresets.json");
#else
    factoryPath = QString("./presets/QuNeo_FactoryPresets.json");
#endif


    presetFile = new QFile(factoryPath);
    presetFile->open(QIODevice::ReadOnly);
    presetByteArray = presetFile->readAll();
    importedPresetMap = parser.parse(presetByteArray, &ok).toMap();
    //handlerOfPresets->presetMapsCopy.value("QuNeo Presets").toMap().value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap() = importedPresetMap.value("QuNeo Presets").toMap().value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap();
    handlerOfPresets->presetMapsCopy.insert(QString("Preset %1").arg(handlerOfPresets->currentPreset), importedPresetMap.value("QuNeo Presets").toMap().value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap());
    handlerOfPresets->slotRecallPreset(QString("Preset %1").arg(handlerOfPresets->currentPreset + 1));
    for(int i = 0; i < 16; i++){
        if(handlerOfPresets->updateIndicator->slotCompareMaps(i)){
            handlerOfPresets->updateIndicator->presetModified[i] = true;
        }
    }
    handlerOfPresets->slotCheckPresets();
    presetByteArray.clear();
    presetFile->close();
    importedPresetMap.clear();
}

void CopyPasteHandler::slotCopySensor(){

    qDebug() << "hi";
   copiedSensorType = currentSensorType;
   qDebug() << copiedSensorType;

    if (currentSensor.contains("Pad")){
    padSensorCopiedMap = handlerOfPresets->presetMapsCopy.value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap().
            value("ComponentSettings").toMap().value(QString("%1").arg(currentSensorType)).toMap().
            value(QString("%1").arg(currentSensor)).toMap();
    if(padSensorCopiedMap.size()){
        pasteSensorAct->setDisabled(false);
    }
    }
    if (currentSensor.contains("VSlider")){
    vSliderSensorCopiedMap = handlerOfPresets->presetMapsCopy.value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap().
            value("ComponentSettings").toMap().value(QString("%1").arg(currentSensorType)).toMap().
            value(QString("%1").arg(currentSensor)).toMap();
    if(vSliderSensorCopiedMap.size()){
        pasteSensorAct->setDisabled(false);
    }
    }
    if (currentSensor.contains("HSlider")){
    hSliderSensorCopiedMap = handlerOfPresets->presetMapsCopy.value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap().
            value("ComponentSettings").toMap().value(QString("%1").arg(currentSensorType)).toMap().
            value(QString("%1").arg(currentSensor)).toMap();
    if(hSliderSensorCopiedMap.size()){
        pasteSensorAct->setDisabled(false);
    }
    }
    if (currentSensor.contains("LongSlider")){
    lSliderSensorCopiedMap = handlerOfPresets->presetMapsCopy.value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap().
            value("ComponentSettings").toMap().value(QString("%1").arg(currentSensorType)).toMap().
            value(QString("%1").arg(currentSensor)).toMap();
    if(lSliderSensorCopiedMap.size()){
        pasteSensorAct->setDisabled(false);
    }
    }
    if (currentSensor.contains("Rotary")){
    rotarySensorCopiedMap = handlerOfPresets->presetMapsCopy.value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap().
            value("ComponentSettings").toMap().value(QString("%1").arg(currentSensorType)).toMap().
            value(QString("%1").arg(currentSensor)).toMap();
    if(rotarySensorCopiedMap.size()){
        pasteSensorAct->setDisabled(false);
    }
    }
    if (currentSensor.contains("Transport")){
    transportSensorCopiedMap = handlerOfPresets->presetMapsCopy.value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap().
            value("ComponentSettings").toMap().value(QString("%1").arg(currentSensorType)).toMap().
            value(QString("%1").arg(currentSensor)).toMap();
    if(transportSensorCopiedMap.size()){
        pasteSensorAct->setDisabled(false);
    }
    }
    if (currentSensor.contains("LeftRight")){
    leftrightSensorCopiedMap = handlerOfPresets->presetMapsCopy.value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap().
            value("ComponentSettings").toMap().value(QString("%1").arg(currentSensorType)).toMap().
            value(QString("%1").arg(currentSensor)).toMap();
    if(leftrightSensorCopiedMap.size()){
        pasteSensorAct->setDisabled(false);
    }
    }
    if (currentSensor.contains("Rhombus")){
    rhombusSensorCopiedMap = handlerOfPresets->presetMapsCopy.value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap().
            value("ComponentSettings").toMap().value(QString("%1").arg(currentSensorType)).toMap().
            value(QString("%1").arg(currentSensor)).toMap();
    if(rhombusSensorCopiedMap.size()){
        pasteSensorAct->setDisabled(false);
    }
    }
    if (currentSensor.contains("UpDown")){
    updownSensorCopiedMap = handlerOfPresets->presetMapsCopy.value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap().
            value("ComponentSettings").toMap().value(QString("%1").arg(currentSensorType)).toMap().
            value(QString("%1").arg(currentSensor)).toMap();
    if(updownSensorCopiedMap.size()){
        pasteSensorAct->setDisabled(false);
    }
    }
}

void CopyPasteHandler::slotPasteSensor(){
    //Load all pad data into paramTypeMap.
    handlerOfPresets->typeNumMap = handlerOfPresets->presetMapsCopy.value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap().
            value("ComponentSettings").toMap().value(QString("%1").arg(currentSensorType)).toMap().
            value(QString("%1").arg(currentSensor)).toMap(); //pad#
    handlerOfPresets->paramTypeMap = handlerOfPresets->presetMapsCopy.value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap().
            value("ComponentSettings").toMap().value(QString("%1").arg(currentSensorType)).toMap();//Pads
    handlerOfPresets->settingsTypeMap = handlerOfPresets->presetMapsCopy.value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap().
            value("ComponentSettings").toMap();//ComponentSettings
    handlerOfPresets->presetNumMap = handlerOfPresets->presetMapsCopy.value(QString("Preset %1").arg(handlerOfPresets->currentPreset)).toMap();//Preset#


    if (currentSensor.contains("Pad")){
        handlerOfPresets->paramTypeMap.insert(QString("%1").arg(currentSensor), padSensorCopiedMap);
    }

    if (currentSensor.contains("VSlider")){

    handlerOfPresets->paramTypeMap.insert(QString("%1").arg(currentSensor), vSliderSensorCopiedMap);
    }

    if (currentSensor.contains("HSlider")){

    handlerOfPresets->paramTypeMap.insert(QString("%1").arg(currentSensor), hSliderSensorCopiedMap);
    }

    if (currentSensor.contains("LongSlider")){

    handlerOfPresets->paramTypeMap.insert(QString("%1").arg(currentSensor), lSliderSensorCopiedMap);
    }
    if (currentSensor.contains("Rotary")){
    handlerOfPresets->paramTypeMap.insert(QString("%1").arg(currentSensor), rotarySensorCopiedMap);
    }
    if (currentSensor.contains("Transport")){

    handlerOfPresets->paramTypeMap.insert(QString("%1").arg(currentSensor), transportSensorCopiedMap);
    }
    if (currentSensor.contains("LeftRight")){
    handlerOfPresets->paramTypeMap.insert(QString("%1").arg(currentSensor), leftrightSensorCopiedMap);
    }
    if (currentSensor.contains("Rhombus")){
    handlerOfPresets->paramTypeMap.insert(QString("%1").arg(currentSensor), rhombusSensorCopiedMap);
    }
    if (currentSensor.contains("UpDown")){
    handlerOfPresets->paramTypeMap.insert(QString("%1").arg(currentSensor), updownSensorCopiedMap);
    }

    //insert the new parameter value into the sensor's typeNumMap and then insert that into the tree of maps.

    handlerOfPresets->settingsTypeMap.insert(QString("%1").arg(currentSensorType), handlerOfPresets->paramTypeMap);
    handlerOfPresets->presetNumMap.insert(QString("ComponentSettings"), handlerOfPresets->settingsTypeMap);
    handlerOfPresets->presetMapsCopy.insert(QString("Preset %1").arg(handlerOfPresets->currentPreset), handlerOfPresets->presetNumMap);

    //don't save pasted sensor data, but recall the preset to display the new data and make the update indicator flash.
    handlerOfPresets->slotRecallPreset(QString("Preset %1").arg(handlerOfPresets->currentPreset + 1));
    handlerOfPresets->slotCheckPresets();

}

void CopyPasteHandler::slotSetCurrentSensor(QString sensorTypeName){
    //this function parses the signal sent by each button class with the name of the clicked sensor.
    //from this string we derive the name of the sensor group, and the name of the current sensor as it appears in the json file
    //these variables are currentSensorType (Pads or HSliders for example) and currentSensor (Pad14 or HSlider0)

        if(sensorTypeName.contains("hSlider")){
    currentSensorType = QString("HSliders");
    }
        if(sensorTypeName.contains("vSlider")){
    currentSensorType = QString("VSliders");
    }
        if(sensorTypeName.contains("lSlider")){
    currentSensorType = QString("LongSliders");
    }
        if(sensorTypeName.contains("buttonButton")){
    currentSensorNumber = sensorTypeName;
    currentSensorNumber = currentSensorNumber.remove(0, 12);
    if(currentSensorNumber.contains("1") && !currentSensorNumber.contains("0")){
    currentSensorType = QString("TransportButtons");
    currentSensor = QString("TransportButton0");
        }
    if(currentSensorNumber.contains("2")){
    currentSensorType = QString("TransportButtons");
    currentSensor = QString("TransportButton1");
        }
    if(currentSensorNumber.contains("3")){
    currentSensorType = QString("TransportButtons");
    currentSensor = QString("TransportButton2");
        }

    if(currentSensorNumber.contains("4") || currentSensorNumber.contains("5") || currentSensorNumber.contains("6") || currentSensorNumber.contains("7")){
    currentSensorType = QString("LeftRightButtons");
    if(currentSensorNumber.contains("4")){
        currentSensorNumber = QString("0");
    }
    if(currentSensorNumber.contains("5")){
        currentSensorNumber = QString("1");
    }
    if(currentSensorNumber.contains("6")){
        currentSensorNumber = QString("2");
    }
    if(currentSensorNumber.contains("7")){
        currentSensorNumber = QString("3");
    }
    currentSensor = QString("LeftRightButton%1").arg(currentSensorNumber);
        }
    if(currentSensorNumber.contains("8")){
    currentSensorType = QString("RhombusButtons");
    currentSensor = QString("RhombusButton0");
        }
    if(currentSensorNumber.contains("9") || currentSensorNumber.contains("10")){
    currentSensorType = QString("UpDownButtons");
    if(currentSensorNumber.contains("9")){
        currentSensorNumber = QString("0");
    }
    if(currentSensorNumber.contains("10")){
        currentSensorNumber = QString("1");
    }
    currentSensor = QString("UpDownButton%1").arg(currentSensorNumber);
        }
    }
        if(sensorTypeName.contains("rotary")){
    currentSensorType = QString("Rotaries");
    currentSensor = QString("Rotary");
    currentSensorNumber = sensorTypeName;
    currentSensorNumber = currentSensorNumber.remove(0, 12);
    currentSensor = QString("%1%2").arg(currentSensor).arg(currentSensorNumber);
    }
        if(sensorTypeName.contains("pad")){
    currentSensorType = QString("Pads");
    currentSensor = QString("Pad");
    currentSensorNumber = sensorTypeName;
    currentSensorNumber = currentSensorNumber.remove(0, 9);
    currentSensor = QString("%1%2").arg(currentSensor).arg(currentSensorNumber);

    }

        if(!sensorTypeName.contains("pad") && !sensorTypeName.contains("rotary") && !sensorTypeName.contains("buttonButton")){
            currentSensor = currentSensorType;
            currentSensor = currentSensor.remove(currentSensor.length() - 1, currentSensor.length());
        }
        if(!sensorTypeName.contains("pad") && !sensorTypeName.contains("rotary") && !sensorTypeName.contains("buttonButton")){
            currentSensorNumber = sensorTypeName;
            currentSensorNumber = currentSensorNumber.remove(0, currentSensorNumber.length() - 1);
            currentSensor = QString("%1%2").arg(currentSensor).arg(currentSensorNumber);
        }

        //qDebug() << "current sensor type" << currentSensorType;
        //qDebug() << "copied sensor type" << copiedSensorType;

        if(currentSensorType != copiedSensorType){
            pasteSensorAct->setDisabled(true);

        } else {
            pasteSensorAct->setDisabled(false);
        }

}

void CopyPasteHandler::slotUpdateFirmware(){
    emit sigUpdateFirmware(false);
    qDebug() << "FW COPYPASTE UPDATE CALLED";
}

void CopyPasteHandler::slotQuNeoConnected (bool connected){

    if(!fwProgressOpen){

    if(connected){
        updateFirmware->setEnabled(true);
        swapPadLeds->setEnabled(true);
        toggleProgramChangeIn->setEnabled(true);
        toggleProgramChangeOut->setEnabled(true);
    } else {
        updateFirmware->setEnabled(false);
        swapPadLeds->setEnabled(false);
        toggleProgramChangeIn->setEnabled(false);
        toggleProgramChangeOut->setEnabled(false);
    }
    }
}

void CopyPasteHandler::slotProgressDialogOpen(bool open){

    fwProgressOpen = open;
    qDebug() << open;

}
