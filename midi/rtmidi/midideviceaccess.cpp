#include "midideviceaccess.h"
#include <QDebug>
#include <unistd.h>
#include <stdlib.h>

//sysEx array for querying the board's ID
unsigned char checkFwSysExData[] = {
    0xF0,0x7E,0x7F,0x06,0x01,0xF7
};

unsigned char swapLedsSysExData[] = {0xF0, 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x50, 0x02, 0x44, 0x5D, 0x00, 0x10, 0xF7};

unsigned char toggleProgramChangeInSysExData[] = {0xF0, 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x50, 0x06, 0x04, 0x59, 0x00, 0x30, 0xF7};
unsigned char toggleProgramChangeOutSysExData[] ={0xF0, 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x50, 0x05, 0x34, 0x3A, 0x00, 0x30, 0xF7};

//sysEx array for entering bootloader mode
unsigned char enterBootloaderData[] = {
    0xF0, 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x11, 0x00, 0x5A, 0x62, 0x00, 0x30,
    0x10, 0xF7};

//var for determining if board has been set to bootloader mode
bool inBootloader = false;

//determines whether sysex message has been completely sent
bool firmwareSent = true;

//determines whether device has reconnected or not
bool deviceReconnected = true;

// until we use c++11
template <typename T, size_t N> T* begin(T(&arr)[N]) { return &arr[0]; }
template <typename T, size_t N> T* end(T(&arr)[N]) { return &arr[0]+N; }

//----Helper/convinience functons ----------------//
void sendHugeSysex(RtMidiOut *midiOut, vector<unsigned char> message);
void sendSysex(RtMidiOut *midiOut, vector<unsigned char> message);
std::vector<pair<int, string > > enumeratePorts(RtMidiOut& midiOut);

//----Callback Declarations and Class Pointer----//
MidiDeviceAccess* callbackPointer; //var for non class callbacks to access class
void rtMidiCallback( double deltatime, std::vector< unsigned char > *message, void *userData );


MidiDeviceAccess::MidiDeviceAccess(QVariantMap* presetMapsCopy,QObject *parent) :
    QObject(parent)
{
    //these array vals should be set to the most recent fw and bootloader version
    versionArray[0] = 3; //bootloader LSB
    versionArray[1] = 1; //booloader MSB
    versionArray[2] = 19; //fw LSB
    versionArray[3] = 2; //fw MSB 2
    versionArray[4] = 1; //fw MSB 1

    mainWindow = parent; //assign mainwindow to parent

    boardVersion = QString("Not Connected");
    editorVersion = QString("%1.%2.%3").
            arg(versionArray[4]).
            arg(versionArray[3]).
            arg(versionArray[2]);

    boardVersionBoot = QString("Not Connected");
    editorVersionBoot = QString("%1.%2").arg(versionArray[1]).arg(versionArray[0]);

    sysExFormat = new SysExFormat(presetMapsCopy, 0); //create new instance of sysExFormat to format presets

    //find device menu, connect it, set initial slot to "None"
    deviceMenu = mainWindow->findChild<QComboBox *>("deviceMenu");
    connect(deviceMenu, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotSelectDevice(QString)));
    deviceMenu->setEnabled(false);
    selectedDevice = -1;

    //initialize to preset 0, just like everything else, connect preset menu to gather preset num
    currentPreset = 0;
    presetMenu = mainWindow->findChild<QComboBox *>("presetMenu");
    connect(presetMenu, SIGNAL(activated(QString)), this, SLOT(slotSetCurrentPreset(QString)));

    //find update button, connect it, used for single preset
    updateDeviceButton = mainWindow->findChild<QPushButton *>("updateButton");
    connect(updateDeviceButton, SIGNAL(clicked()), this, SLOT(slotUpdateSinglePreset()));

    //find update button, connect it, used for update all
    updateAllButton = mainWindow->findChild<QPushButton *>("updateAll");
    connect(updateAllButton, SIGNAL(clicked()), this, SLOT(slotUpdateAllPresets()));

    //pointer to this instance of the class, used in callback functions which are not part of the class
    callbackPointer = this;

    //load sysEx firmware file from app package
    pathSysex = QCoreApplication::applicationDirPath();
    pathSysex.remove(pathSysex.length() - 5, pathSysex.length());
    pathSysex.append("Resources/Quneo.syx");
    //qDebug() << QString("path: %1").arg(pathSysex);
    //sysExFirmware = new QFile(pathSysex);
    sysExFirmware = new QFile(":Quneo/sysex/resources/sysex/Quneo.syx");
    if(sysExFirmware->open(QIODevice::ReadOnly)){
        //qDebug("sysex open");
    } else {
        //qDebug("couldn't find sysex");
    }

    //read all data from file, and store in a QByteArray
    sysExFirmwareBytes = sysExFirmware->readAll();
    //qDebug() <<"sysex size:" << sysExFirmwareBytes.size();

    //assign bytes to array, necessary for sending midi according to mac midi services
    sysExFirmwareData = vector<unsigned char>(sysExFirmwareBytes.data(), sysExFirmwareBytes.data() + sysExFirmwareBytes.size());

    //******* Load Preset SysEx Files ********//
    for(int i = 0; i<16; i++){
        loadPreset[i] = new QFile(QString(":Quneo/sysex/resources/sysex/loadPresets/loadPreset%1.syx").arg(i));
        if(loadPreset[i]->open(QIODevice::ReadOnly)){
            //qDebug() << "load preset open" << i;
        } else {
            //qDebug() << "could not open load preset" << i;
        }

        loadPresetBytes[i] = loadPreset[i]->readAll();
        loadPresetSize[i] = loadPresetBytes[i].size();
        loadPresetData[i] = new char[loadPresetSize[i]];
        loadPresetData[i] = loadPresetBytes[i].data();
    }

    midiIn = new RtMidiIn(RtMidi::LINUX_ALSA, "Q-Editor");
    midiOut = new RtMidiOut(RtMidi::LINUX_ALSA, "Q-Editor");
    //get sources and dests, and store in vector
    getSourcesDests();


}

void MidiDeviceAccess::getSourcesDests()
{

    boardVersion = QString("Not Connected");
    boardVersionBoot = QString("Not Connected");

    sourceCount = 0;
    destCount = 0;

    //clear source/dest vectors
    quNeoSources.clear();
    quNeoDests.clear();

    //Sources
      sourceCount = midiIn->getPortCount();

    for (int i = 0 ; i < sourceCount ; ++i)
    {
        string portName = midiIn->getPortName(i);
        std::transform(portName.begin(), portName.end(), portName.begin(), ::toupper);
        if(string::npos != portName.find("QUNEO")) {
            quNeoSources.push_back(pair<int, string>(i, portName));
        }
    }

//    //Destinations (same procedure as above)
      destCount = midiOut->getPortCount();

    for (int i = 0 ; i < destCount ; ++i)
    {
        string portName = midiOut->getPortName(i);
        std::transform(portName.begin(), portName.end(), portName.begin(), ::toupper);
        if(string::npos != portName.find("QUNEO")) {
            quNeoDests.push_back(pair<int, string>(i, portName));
        }
    }

//    //clear and repopulate device menu
    deviceMenu->clear();

    //if there are any dests add them to the menu, here's where we might want to limit number
    if(quNeoDests.size() == 0)
    {

        //if not dests, put none in the menu
        deviceMenu->insertItem(0, QString("None"));

        emit sigQuNeoConnected(false);

    }
    else if(quNeoDests.size() > 1)
    {

        //MIDIRestart();
        //getSourcesDests();


        msgBox.setText("You have too many QuNeos plugged in.");
        msgBox.setInformativeText("Please edit 1 QuNeo at a time.");
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowModality(Qt::NonModal);
        int ret = msgBox.exec();

        if(ret == QMessageBox::Ok)
        {
            qDebug() << "ok hit";

            QTimer::singleShot(2000, this, SLOT(getSourcesDests()));
        }

    }
    else
    {
        //enumerate quneos in menu
        for(std::vector<pair<int, string> >::const_iterator iterator = quNeoDests.begin(); iterator != quNeoDests.end(); ++iterator)
        {
            deviceMenu->insertItem((*iterator).first, QString::fromStdString((*iterator).second) );
            emit sigQuNeoConnected(true);
        }
    }
    // emit sigSetVersions(editorVersion, boardVersion);

}


void MidiDeviceAccess::connectDevice(){ //makes a connection between a device and our app's i/o ports

    qDebug() << "_____connectDevice() called_____";

    // disconnect any open connections
    midiOut->closePort();
    midiIn->cancelCallback();
    midiIn->closePort();

    if(selectedDevice>-1)
    {
        midiOut->openPort(selectedDevice, "out");
        qDebug() << "opened output to " << QString::fromStdString(midiOut->getPortName(selectedDevice));
        //if source vector contains any QuNeo sources, then iterate through them and connect
        for(vector<pair<int,string> >::const_iterator src_device = quNeoSources.begin(); src_device != quNeoSources.end(); ++src_device)
        {
            //connect source to our client's input port
            //FIXME : will end on last port - this is only ok currently as we only allow one QUNEO 
            //to be attached.
            midiIn->openPort((*src_device).first, "in");
            midiIn->setCallback(&rtMidiCallback, callbackPointer);
            midiIn->ignoreTypes(false, true, true);
            
        }
        //if not in bootloader mode, send sysex check fw message upon device connection/selection
        if(!inBootloader){
            slotCheckFirmwareVersion();
        }
    }
}

void MidiDeviceAccess::slotSelectDevice(QString index){

    qDebug() << "_____ slotSelectDevice() called_____" << index;

    selectedDevice = -1;
    //if selected device contains QuNeo, then iterate through dests and assign index to selected device
    if(index.contains(QString("QuNeo"), Qt::CaseInsensitive)){
        for(vector<pair<int,string> >::const_iterator device = quNeoDests.begin(); device != quNeoDests.end(); ++device) {
            if( 0==index.toStdString().compare((*device).second) ){
                selectedDevice = (*device).first;
            }
        }
    } 
    //after device selection connect device (regardless of whether selected device is NULL, this is taken care of in connectDevice())
    connectDevice();

}

void MidiDeviceAccess::slotUpdateAllPresets() { //used for updating all presets at once

//    if(deviceMenu->currentText() == "QuNeo 1"){
        for(int i= 0; i< 16; i++){

            //encode current preset data
            sysExFormat->slotEncodePreset(i);

            //create char array of the most recently encoded preset using sysExFormat
            char *presetSysExData = new char(sysExFormat->presetSysExByteArray.size());

            //assign sysExFormat data to char array
            presetSysExData = sysExFormat->presetSysExByteArray.data();

            //if there is a selected device, send down preset data via sysex protocol
            if(selectedDevice){
                vector<unsigned char> message(presetSysExData, presetSysExData + sysExFormat->presetSysExByteArray.size());
                sendSysex(midiOut, message);
                qDebug("check fw sent");
            }
        }

        slotLoadPreset();
        emit sigUpdateAllPresetsCount(16);
//    }

}

void MidiDeviceAccess::slotUpdateSinglePreset(){

//    if(deviceMenu->currentText() == "QuNeo 1"){

        //encode current preset
        sysExFormat->slotEncodePreset(currentPreset);

        //create new char array using most recently encoded preset
        char *presetSysExData = new char(sysExFormat->presetSysExByteArray.size());

        //assign encoded data into char array
        presetSysExData = sysExFormat->presetSysExByteArray.data();

        //if there is a selected device
        if(selectedDevice){
                vector<unsigned char> message(presetSysExData, presetSysExData + sysExFormat->presetSysExByteArray.size());
                sendSysex(midiOut, message);
                qDebug("Preset Sysex Sent - Current Preset");

        }
        //load the currently selected preset
        slotLoadPreset();
//    }

}

void MidiDeviceAccess::slotLoadPreset() {


    char* loadPresetSysExData = new char[loadPresetSize[currentPreset]];
    loadPresetSysExData = loadPresetData[currentPreset];

    //if there is a selected device
    if (selectedDevice) {

        vector<unsigned char> message(loadPresetSysExData, loadPresetSysExData + loadPresetSize[currentPreset]);
        sendSysex(midiOut, message);
        qDebug("Preset Sysex Sent - Current Preset");

    }

}

void MidiDeviceAccess::slotUpdateFirmware(){//this function puts the board into bootloader mode************


//    //if selected device...
//    if(selectedDevice){
//        vector<unsigned char> message(begin(enterBootloaderData), end(enterBootloaderData));
//        sendSysex(midiOut, message);
//        inBootloader = true;
//    }
//
//    QTimer::singleShot(5000, callbackPointer, SLOT(slotDownloadFw()));
}

void MidiDeviceAccess::slotDownloadFw(){//this function sends the actual firmware data**********

//    if(selectedDevice){
//        sendSysex(midiOut, sysExFirmwareData);
//
//        firmwareSent = false;
//
////        //create new sysex event/request
////        downloadFwSysExReq = new MIDISysexSendRequest;
//
////        //set event/request params
////        downloadFwSysExReq->destination = selectedDevice;
////        downloadFwSysExReq->data = (const Byte *)sysExFirmwareData;
////        downloadFwSysExReq->bytesToSend = sysExFirmwareBytes.size();
////        downloadFwSysExReq->complete = false;
////        downloadFwSysExReq->completionProc = &sysExComplete;
////        downloadFwSysExReq->completionRefCon = sysExFirmwareData;
//
////        //send the syesex data
////        MIDISendSysex(downloadFwSysExReq);
////        //qDebug() << "address of msg" << midiSysexSendRequest;
//          firmwareSent = true;
//          qDebug("fw download sent!");
//          inBootloader = false;
//
////    while(!downloadFwSysExReq->complete){
////        bytesLeft = downloadFwSysExReq->bytesToSend;
////        emit sigFwBytesLeft(bytesLeft);
////    }
//
//    emit sigFwBytesLeft((0));
//    }
//

}

void MidiDeviceAccess::slotCheckFirmwareVersion(){//this function checks versions with sysEx
    vector<unsigned char> message(begin(checkFwSysExData), end(checkFwSysExData));
    sendSysex(midiOut, message);
    qDebug("check fw sent");
}

void MidiDeviceAccess::slotSwapLeds(){

    vector<unsigned char> message(begin(swapLedsSysExData), end(swapLedsSysExData));
    sendSysex(midiOut, message);
}

void MidiDeviceAccess::slotSetCurrentPreset(QString selected){
    if(selected.contains("Preset ")){ //remove prepended preset
        if(selected.contains(" *")){    //remove asterisk if recalling unsaved/modified preset
            selected.remove(" *");
        }
        currentPreset = selected.remove(0,7).toInt() -1; //reduce string to number
        //qDebug() << "Current Preset" << currentPreset;
    }
}

void MidiDeviceAccess::slotSendToggleProgramChangeOutput(){
    vector<unsigned char> message(begin(toggleProgramChangeOutSysExData), end(toggleProgramChangeOutSysExData));
    sendSysex(midiOut, message);
}

void MidiDeviceAccess::slotSendToggleProgramChangeInput(){
    vector<unsigned char> message(begin(toggleProgramChangeInSysExData), end(toggleProgramChangeInSysExData));
    sendSysex(midiOut, message);
}

void MidiDeviceAccess::processSysex(std::vector< unsigned char > *message) {
    if (message->size() == 17 && message->at(3) == 6 && message->at(4) == 2) {
        // firmware inquery response

        qDebug() << "boot LSB" << message->at(12);
        qDebug() << "boot MSB" << message->at(13);
        qDebug() << "fw LSB" << message->at(14);
        qDebug() << "fw MSB" << message->at(15);


        //set version vars from sysex message
        bootloaderVersionLSB = message->at(12);
        bootloaderVersionMSB = message->at(13);
        fwVersionLSB = message->at(14);
        fwVersionMSB = message->at(15);

        boardVersion = QString("%1.%2.%3").arg(fwVersionMSB / 16).arg(fwVersionMSB % 16).arg(fwVersionLSB);
        boardVersionBoot = QString("%1.%2").arg(bootloaderVersionMSB).arg(bootloaderVersionLSB);

        //qDebug() << "fw:" << fwVersionMSB << fwVersionLSB;
        //qDebug() << "bootloader:" << bootloaderVersionMSB << bootloaderVersionLSB;

        //check version vars against version array
        if (//versionArray[0] != bootloaderVersionLSB ||
                //versionArray[1] != bootloaderVersionMSB ||
                versionArray[2] != fwVersionLSB ||
                versionArray[3] != fwVersionMSB % 16 ||
                versionArray[4] != fwVersionMSB / 16) {
            emit sigFirmwareCurrent(false);
            //qDebug() << "version good" << false;
        } else {
            //qDebug() << "version good" << true;
            emit sigFirmwareCurrent(true);
        }
    }

}
//-------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
//--------------------------------------------Callback Definitions---------------------------------------//
//-------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//

void rtMidiCallback( double deltatime, std::vector< unsigned char > *message, void *userData )
{
    if(240 == (int)message->at(0)) {
        ((MidiDeviceAccess*)userData)->processSysex(message);
    } else {
        // currently we ignore anything that is not sysex
        unsigned int nBytes = message->size();
        for ( unsigned int i=0; i<nBytes; i++ )
          qDebug() << "Byte " << i << " = " << (int)message->at(i) << ", ";
        if ( nBytes > 0 )
          qDebug() << "stamp = " << deltatime ;
        
    }
}

    
/*
 * RtMidi Helper Functions
 */
void sendHugeSysex(RtMidiOut *midiOut, vector<unsigned char> message) {
    char buffer [256];
    int pos=0;
    while ( pos + 1024 < message.size()) {
        sendSysex(midiOut, vector<unsigned char>(message.begin() + pos, message.begin() + pos + 1024));
        pos += 1024;
        sprintf(buffer, "%d", pos);
        qDebug(buffer);
    }
    sendSysex(midiOut, vector<unsigned char>(message.begin() + pos, message.end()));
    
    return;
}
void sendSysex(RtMidiOut *midiOut, vector<unsigned char> message) {
    try {
        midiOut->sendMessage(&message);
    } catch (RtError &error) {
//        cout << "An error occurred sending messge:" << endl;
        //TODO: should probably send signal for UI to display error.
        error.printMessage();
    }
    return;
}
std::vector<pair<int, string > > enumeratePorts(RtMidiOut *midiOut) {
    std::vector < pair<int, string > > portSpecs;
    int numPorts = midiOut->getPortCount();
    for (int portId = 0; portId < numPorts; portId++) {
        portSpecs.push_back(pair<int, string>(portId, midiOut->getPortName(portId)));
    }
    return portSpecs;
}


//void sysExComplete(MIDISysexSendRequest* request)
//{

//    //called after sysex messages are completely sent
//    if(request == enterBootloaderSysExReq)
//    {
//        inBootloader = true;
//        //qDebug("enter bootloader sent");
//    }
//    else if(request == checkFwSysEx)
//    {
//        qDebug("check fw sent");
//    }
//    else if(request == downloadFwSysExReq)
//    {
//        firmwareSent = true;
//        qDebug("fw download sent!");
//        inBootloader = false;
//    }
//}

