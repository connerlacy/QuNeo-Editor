#include "midideviceaccess.h"
#include <QDebug>

//sysEx array for querying the board's ID
char checkFwSysExData[] = {
    0xF0,0x7E,0x7F,0x06,0x01,0xF7
};

char swapLedsSysExData[] = {0xF0, 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x50, 0x02, 0x44, 0x5D, 0x00, 0x10, 0xF7};

char toggleProgramChangeInSysExData[] = {0xF0, 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x50, 0x06, 0x04, 0x59, 0x00, 0x30, 0xF7};
char toggleProgramChangeOutSysExData[] ={0xF0, 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x50, 0x05, 0x34, 0x3A, 0x00, 0x30, 0xF7};

//sysEx array for entering bootloader mode
char enterBootloaderData[] = {
    0xF0, 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x11, 0x00, 0x5A, 0x62, 0x00, 0x30,
    0x10, 0xF7};

//var for determining if board has been set to bootloader mode
bool inBootloader = false;

//determines whether sysex message has been completely sent
bool firmwareSent = true;

//determines whether device has reconnected or not
bool deviceReconnected = true;

//----Callback Declarations and Class Pointer----//
MidiDeviceAccess* callbackPointer; //var for non class callbacks to access class
//void getMIDINotificationVirtual(const MIDINotification *message, void *refCon); //notification of when MIDI state changes (new controllers)
//void sysExComplete(MIDISysexSendRequest*); //called when sysex event has been completely sent
//void incomingMidi(const MIDIPacketList *pktlist, void *readProcRefCon, void *srcConnRefCon); //called upon incoming midi with connected port
//bool sysEx = false; //determines whether or not incomding midi is of sysEx type


MidiDeviceAccess::MidiDeviceAccess(QVariantMap* presetMapsCopy,QObject *parent) :
    QObject(parent)
{
    //these array vals should be set to the most recent fw and bootloader version
    // versionArray[0] = 2; //bootloader LSB
    //versionArray[1] = 1; //booloader MSB

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
//    selectedDevice = NULL;

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

    //create new char array of the above byte array size
    sysExFirmwareData = new char[sysExFirmwareBytes.size()];

    //assigne bytes to array, necessary for sending midi according to mac midi services
    sysExFirmwareData = sysExFirmwareBytes.data();

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
      RtMidiOut midiIn(RtMidi::LINUX_ALSA);
      sourceCount = midiIn.getPortCount();

    for (int i = 0 ; i < sourceCount ; ++i)
    {
        string portName = midiIn.getPortName(i);
        std::transform(portName.begin(), portName.end(), portName.begin(), ::toupper);
        if(string::npos != portName.find("QUNEO")) {
            quNeoSources.push_back(pair<int, string>(i, portName));
        }
    }

//    //Destinations (same procedure as above)
      RtMidiOut midiOut(RtMidi::LINUX_ALSA);
      destCount = midiOut.getPortCount();

    for (int i = 0 ; i < destCount ; ++i)
    {
        string portName = midiOut.getPortName(i);
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

//        //if not dests, put none in the menu
        deviceMenu->insertItem(0, QString("None"));

        emit sigQuNeoConnected(false);

    }
    else
    {
        //enumerate quneos in menu
        for(std::vector<pair<int, string> >::const_iterator iterator = quNeoDests.begin(); iterator != quNeoDests.end(); ++iterator)
        {
            deviceMenu->insertItem(iterator->first, QString::fromStdString(iterator->second) );
            emit sigQuNeoConnected(true);
        }
    }
//    // emit sigSetVersions(editorVersion, boardVersion);

}

//QString MidiDeviceAccess::getDisplayName(MIDIObjectRef object) {

//    // Returns the display name of a given MIDIObjectRef as an NSString
//    CFStringRef name = nil; //place holder for name

//    if(noErr != MIDIObjectGetStringProperty(object, kMIDIPropertyDisplayName, &name)){//get the name using midi services function
//        return nil;
//    }

//    return QString(cFStringRefToQString(name)); //return the name
//}

//QString MidiDeviceAccess::cFStringRefToQString(CFStringRef ref){

//    //this function just translates a CFStringRef into a QString
//    CFRange range;
//    range.location = 0;
//    range.length = CFStringGetLength(ref);
//    QString result(range.length, QChar(0));

//    UniChar *chars = new UniChar[range.length];
//    CFStringGetCharacters(ref, range, chars);
//    //[nsstr getCharacters:chars range:range];
//    result = QString::fromUtf16(chars, range.length);
//    delete[] chars;
//    return result;

//}

//void MidiDeviceAccess::connectDevice(){ //makes a connection between a device and our app's i/o ports

//    qDebug() << "_____connectDevice() called_____";

//    //dispose of client (and ports/connections) if it exists
//    //if(appyClientRef){
//    MIDIClientDispose(appyClientRef);
//    //}

//    //create client (CoreMIDI's reference to this app)
//    MIDIClientCreate(CFSTR("appy"), getMIDINotificationVirtual, this, &appyClientRef);

//    //create input and output ports for our client
//    MIDIInputPortCreate(appyClientRef, CFSTR("appyInPort"), incomingMidi, this, &appyInPortRef);
//    MIDIOutputPortCreate(appyClientRef, CFSTR("appyOutPort"), &appyOutPortRef);

//    //if source vector contains any QuNeo sources, then iterate through them and connect
//    if(selectedDevice)
//    {
//        for(int i=0; i<quNeoSources.size(); i++)
//        {
//            //connect source to our client's input port
//            MIDIPortConnectSource(appyInPortRef, quNeoSources.at(i), &quNeoSources.at(i));
//        }

//        //if not in bootloader mode, send sysex check fw message upon device connection/selection
//        if(!inBootloader){
//            slotCheckFirmwareVersion();
//        }
//    }
//}

void MidiDeviceAccess::slotSelectDevice(QString index){

//    qDebug() << "_____ slotSelectDevice() called_____" << index;

//    //if selected device contains QuNeo, then iterate through dests and assign index to selected device
//    if(index.contains(QString("QuNeo"))){
//        int i = index.remove(0,6).toInt() - 1;
//        selectedDevice = quNeoDests.at(i);

//    } else {
//        selectedDevice = NULL;
//    }
//    //after device selection connect device (regardless of whether selected device is NULL, this is taken care of in connectDevice())
//    connectDevice();

}

void MidiDeviceAccess::slotUpdateAllPresets(){ //used for updating all presets at once


//    if(deviceMenu->currentText() == "QuNeo 1"){
//        for(int i= 0; i< 16; i++){

//            //encode current preset data
//            sysExFormat->slotEncodePreset(i);

//            //create char array of the most recently encoded preset using sysExFormat
//            char *presetSysExData = new char(sysExFormat->presetSysExByteArray.size());

//            //assign sysExFormat data to char array
//            presetSysExData = sysExFormat->presetSysExByteArray.data();

//            //if there is a selected device, send down preset data via sysex protocol
//            if(selectedDevice){

//                //create new sysex event/request for preset
//                updateAllPresetSysExReq = new MIDISysexSendRequest;

//                //set event/request params
//                updateAllPresetSysExReq->destination = selectedDevice;
//                updateAllPresetSysExReq->data = (const Byte *)presetSysExData;
//                updateAllPresetSysExReq->bytesToSend = sysExFormat->presetSysExByteArray.size();
//                updateAllPresetSysExReq->complete = false;
//                updateAllPresetSysExReq->completionProc = &sysExComplete;
//                updateAllPresetSysExReq->completionRefCon = presetSysExData;

//                //send the syesex data
//                MIDISendSysex(updateAllPresetSysExReq);

//                while(updateAllPresetSysExReq->bytesToSend > 0){
//                    qDebug() << "bytes to send update all presets" << updateAllPresetSysExReq->bytesToSend;
//                }

//                //qDebug() << "done with preset" << i;
//                emit sigUpdateAllPresetsCount(i);
//            }
//        }

//        slotLoadPreset();
//        emit sigUpdateAllPresetsCount(16);
//    }

}

void MidiDeviceAccess::slotUpdateSinglePreset(){

//    if(deviceMenu->currentText() == "QuNeo 1"){

//        //encode current preset
//        sysExFormat->slotEncodePreset(currentPreset);

//        //create new char array using most recently encoded preset
//        char *presetSysExData = new char(sysExFormat->presetSysExByteArray.size());

//        //assign encoded data into char array
//        presetSysExData = sysExFormat->presetSysExByteArray.data();

//        //if there is a selected device
//        if(selectedDevice){

//            //create new sysex event/request
//            updateSinglePresetSysExReq = new MIDISysexSendRequest;

//            //set event/request params
//            updateSinglePresetSysExReq->destination = selectedDevice;
//            updateSinglePresetSysExReq->data = (const Byte *)presetSysExData;
//            updateSinglePresetSysExReq->bytesToSend = sysExFormat->presetSysExByteArray.size();
//            updateSinglePresetSysExReq->complete = false;
//            updateSinglePresetSysExReq->completionProc = &sysExComplete;
//            updateSinglePresetSysExReq->completionRefCon = presetSysExData;

//            //send the syesex data
//            MIDISendSysex(updateSinglePresetSysExReq);
//        }

//        //chill while the sysex gets down...

//        int bytes = 0;

//        while(updateSinglePresetSysExReq->bytesToSend > 0){

//            if(bytes != updateSinglePresetSysExReq->bytesToSend)
//            {
//                bytes = updateSinglePresetSysExReq->bytesToSend;
//                qDebug() << "bytes left to send single preset" << bytes;
//            }
//        }

//        //load the currently selected preset
//        slotLoadPreset();
//    }

}

void MidiDeviceAccess::slotLoadPreset(){


//    char* loadPresetSysExData = new char[loadPresetSize[currentPreset]];
//    loadPresetSysExData = loadPresetData[currentPreset];

//    //if there is a selected device
//    if(selectedDevice){

//        //create new sysex event/request
//        loadPresetSysExReq = new MIDISysexSendRequest;

//        //set event/request params
//        loadPresetSysExReq->destination = selectedDevice;
//        loadPresetSysExReq->data = (const Byte *)loadPresetSysExData;
//        loadPresetSysExReq->bytesToSend = loadPresetSize[currentPreset];
//        loadPresetSysExReq->complete = false;
//        loadPresetSysExReq->completionProc = &sysExComplete;
//        loadPresetSysExReq->completionRefCon = loadPresetSysExData;

//        //send the syesex data
//        MIDISendSysex(loadPresetSysExReq);
//    }

//    int bytes = 0;

//    while(loadPresetSysExReq->bytesToSend > 0){

//        if(bytes != loadPresetSysExReq->bytesToSend)
//        {
//            bytes = loadPresetSysExReq->bytesToSend;
//            qDebug() << "load pesets byes to send" << bytes;
//        }
//    }



}

void MidiDeviceAccess::slotUpdateFirmware(){//this function puts the board into bootloader mode************


//    //if selected device...
//    if(selectedDevice){

//        //create new sysex event/request
//        enterBootloaderSysExReq = new MIDISysexSendRequest;

//        //currentSysExMessage = enterBootloaderSysExReq;

//        //set event/request params
//        enterBootloaderSysExReq->destination = selectedDevice;
//        enterBootloaderSysExReq->data = (const Byte *)enterBootloaderData;
//        enterBootloaderSysExReq->bytesToSend = 17;
//        enterBootloaderSysExReq->complete = false;
//        enterBootloaderSysExReq->completionProc = &sysExComplete;
//        enterBootloaderSysExReq->completionRefCon = enterBootloaderData;

//        //send the syesex data
//        MIDISendSysex(enterBootloaderSysExReq);
//        //qDebug() << "address of msg" << enterBootloaderSysExReq;
//    }

//    QTimer::singleShot(5000, callbackPointer, SLOT(slotDownloadFw()));
}

void MidiDeviceAccess::slotDownloadFw(){//this function sends the actual firmware data**********

//    if(selectedDevice){

//        firmwareSent = false;

//        //create new sysex event/request
//        downloadFwSysExReq = new MIDISysexSendRequest;

//        //set event/request params
//        downloadFwSysExReq->destination = selectedDevice;
//        downloadFwSysExReq->data = (const Byte *)sysExFirmwareData;
//        downloadFwSysExReq->bytesToSend = sysExFirmwareBytes.size();
//        downloadFwSysExReq->complete = false;
//        downloadFwSysExReq->completionProc = &sysExComplete;
//        downloadFwSysExReq->completionRefCon = sysExFirmwareData;

//        //send the syesex data
//        MIDISendSysex(downloadFwSysExReq);
//        //qDebug() << "address of msg" << midiSysexSendRequest;
//    }

//    while(!downloadFwSysExReq->complete){
//        bytesLeft = downloadFwSysExReq->bytesToSend;
//        emit sigFwBytesLeft(bytesLeft);
//    }

//    //emit sigFwBytesLeft((0));


}

void MidiDeviceAccess::slotCheckFirmwareVersion(){//this function checks versions with sysEx

//    checkFwSysEx = new MIDISysexSendRequest;

//    //currentSysExMessage = checkFwSysEx;

//    //set event/request params
//    checkFwSysEx->destination = selectedDevice;
//    checkFwSysEx->data = (const Byte *)checkFwSysExData;
//    checkFwSysEx->bytesToSend = 6;
//    checkFwSysEx->complete = false;
//    checkFwSysEx->completionProc = &sysExComplete;
//    checkFwSysEx->completionRefCon = checkFwSysExData;

//    //send the syesex data
//    MIDISendSysex(checkFwSysEx);

}

void MidiDeviceAccess::slotSwapLeds(){//this function checks versions with sysEx

//    swapLEDsSysExReq = new MIDISysexSendRequest;

//    //currentSysExMessage = checkFwSysEx;

//    //set event/request params
//    swapLEDsSysExReq->destination = selectedDevice;
//    swapLEDsSysExReq->data = (const Byte *)swapLedsSysExData;
//    swapLEDsSysExReq->bytesToSend = 17;
//    swapLEDsSysExReq->complete = false;
//    swapLEDsSysExReq->completionProc = &sysExComplete;
//    swapLEDsSysExReq->completionRefCon = swapLedsSysExData;

//    //send the syesex data
//    MIDISendSysex(swapLEDsSysExReq);

}


void MidiDeviceAccess::slotSetCurrentPreset(QString selected){
//    if(selected.contains("Preset ")){ //remove prepended preset
//        if(selected.contains(" *")){    //remove asterisk if recalling unsaved/modified preset
//            selected.remove(" *");
//        }
//        currentPreset = selected.remove(0,7).toInt() -1; //reduce string to number
//        //qDebug() << "Current Preset" << currentPreset;
//    }
}

void MidiDeviceAccess::slotProcessSysExRx(int val){

//    //if not in bootloader...
//    if(!inBootloader){
//        //qDebug() << "sysex data" << val;
//        //qDebug("sysEx %x", val);

//        if(val == 247){ //if val is terminating sysex, append list with it
//            sysExMsg.append(247);

//            //if device ID, set fw and bootloader vars
//            if(sysExMsg.size() == 17 && sysExMsg.at(3) == 6 && sysExMsg.at(4) == 2){


//                qDebug() << "boot LSB" << sysExMsg.at(12);
//                qDebug() << "boot MSB" << sysExMsg.at(13);
//                qDebug() << "fw LSB" << sysExMsg.at(14);
//                qDebug() << "fw MSB" << sysExMsg.at(15);


//                //set version vars from sysex message
//                bootloaderVersionLSB = sysExMsg.at(12);
//                bootloaderVersionMSB = sysExMsg.at(13);
//                fwVersionLSB = sysExMsg.at(14);
//                fwVersionMSB = sysExMsg.at(15);

//                boardVersion = QString("%1.%2.%3").arg(fwVersionMSB/16).arg(fwVersionMSB%16).arg(fwVersionLSB);
//                boardVersionBoot = QString("%1.%2").arg(bootloaderVersionMSB).arg(bootloaderVersionLSB);

//                //qDebug() << "fw:" << fwVersionMSB << fwVersionLSB;
//                //qDebug() << "bootloader:" << bootloaderVersionMSB << bootloaderVersionLSB;

//                //check version vars against version array
//                if(//versionArray[0] != bootloaderVersionLSB ||
//                        //versionArray[1] != bootloaderVersionMSB ||
//                        versionArray[2] != fwVersionLSB ||
//                        versionArray[3] != fwVersionMSB%16 ||
//                        versionArray[4] != fwVersionMSB/16){
//                    emit sigFirmwareCurrent(false);
//                    //qDebug() << "version good" << false;
//                } else {
//                    //qDebug() << "version good" << true;
//                    emit sigFirmwareCurrent(true);
//                }
//            }
//            sysExMsg.clear();
//        } else {
//            sysExMsg.append(val);
//        }
//    }
}

void MidiDeviceAccess::slotSendToggleProgramChangeOutput(){

//    toggleProgramChangeOutSysExReq = new MIDISysexSendRequest;

//    //currentSysExMessage = checkFwSysEx;

//    //set event/request params
//    toggleProgramChangeOutSysExReq->destination = selectedDevice;
//    toggleProgramChangeOutSysExReq->data = (const Byte *)toggleProgramChangeOutSysExData;
//    toggleProgramChangeOutSysExReq->bytesToSend = 17;
//    toggleProgramChangeOutSysExReq->complete = false;
//    toggleProgramChangeOutSysExReq->completionProc = &sysExComplete;
//    toggleProgramChangeOutSysExReq->completionRefCon = toggleProgramChangeOutSysExData;

//    //send the syesex data
//    MIDISendSysex(toggleProgramChangeOutSysExReq);


}

void MidiDeviceAccess::slotSendToggleProgramChangeInput(){

//    toggleProgramChangeInSysExReq = new MIDISysexSendRequest;

//    //currentSysExMessage = checkFwSysEx;

//    //set event/request params
//    toggleProgramChangeInSysExReq->destination = selectedDevice;
//    toggleProgramChangeInSysExReq->data = (const Byte *)toggleProgramChangeInSysExData;
//    toggleProgramChangeInSysExReq->bytesToSend = 17;
//    toggleProgramChangeInSysExReq->complete = false;
//    toggleProgramChangeInSysExReq->completionProc = &sysExComplete;
//    toggleProgramChangeInSysExReq->completionRefCon = toggleProgramChangeInSysExData;

//    //send the syesex data
//    MIDISendSysex(toggleProgramChangeInSysExReq);
}

//-------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
//--------------------------------------------Callback Definitions---------------------------------------//
//-------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//


//these functions are used to communicate with apple services midi, and cannot be part of a class
//see http://developer.apple.com/library/ios/#documentation/CoreMidi/Reference/MIDIServices_Reference/Reference/reference.html
//void getMIDINotificationVirtual(const MIDINotification *message, void *refCon){

//    //get midi notifications-- if the setup changes...
//    if(message->messageID == 1){
//        qDebug("MIDI Setup Changed");
//        callbackPointer->getSourcesDests();
//    }
//}

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

//void incomingMidi(const MIDIPacketList *pktlist, void *readProcRefCon, void *srcConnRefCon){

//    //iterate through midi packets and process according to type
//    const MIDIPacket *packet = &pktlist->packet[0];

//    //for number packets in packet list
//    for(int i =0; i < pktlist->numPackets; i++){

//        //for length of packet
//        for(int j = 0; j < packet->length; j++){

//            //if packet data is sysEx start...
//            if(packet->data[j] == 240){

//                //start to send vals for sysex processing
//                callbackPointer->slotProcessSysExRx(packet->data[j]);

//                //set sysEx message type to true to process rest of sysex
//                sysEx = true;

//                //if packet is terminating sysex...
//            } else if(packet->data[j] == 247){

//                //send terminating val to sysex processing
//                callbackPointer->slotProcessSysExRx(packet->data[j]);

//                //set sysEx message type to false to cease processing
//                sysEx = false;

//            } else {

//                //if sysEx is true, send all input to slot processing
//                if(sysEx){
//                    callbackPointer->slotProcessSysExRx(packet->data[j]);
//                } else if(packet->data[j] != 247){
//                    //here's where non sysEx midi data gets read
//                    //qDebug() << "midi data" << packet->data[j];
//                }
//            }
//        }
//        //advance packet in midi packet list
//        packet = MIDIPacketNext(packet);
//    }
//}
