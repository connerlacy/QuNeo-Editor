#include "midideviceaccess.h"
#include <QDebug>
#include <unistd.h>
#include <stdlib.h>
#include <ostream>

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

//determines whether device has reconnected or not
bool deviceReconnected = true;

// until we use c++11
template <typename T, size_t N> T* begin(T(&arr)[N]) { return &arr[0]; }
template <typename T, size_t N> T* end(T(&arr)[N]) { return &arr[0]+N; }

//----Helper/convinience functons ----------------//

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

    boardVersion = QString("Not Connected");
    boardVersionBoot = QString("Not Connected");

    sysExFormat = new SysExFormat(presetMapsCopy, 0); //create new instance of sysExFormat to format presets

    //find device menu, connect it, set initial slot to "None"
    deviceMenu = mainWindow->findChild<QComboBox *>("deviceMenu");
    connect(deviceMenu, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSelectDevice(int)));
    //deviceMenu->setEnabled(false);
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

    // initialize alsa
    sequencerHandle = NULL;
    if ( snd_seq_open( &sequencerHandle, "default", SND_SEQ_OPEN_INPUT|SND_SEQ_OPEN_OUTPUT, 0 ) < 0 ) {
        qDebug() << "Unable to access ALSA";
        return;
    }

    if ( sequencerHandle == NULL ) {
        qDebug() << "ALSA returned null handle";
        return;
    }

    snd_seq_set_client_name( sequencerHandle, "QuNeo Preset Editor" );

    outPort = snd_seq_create_simple_port(
                sequencerHandle,
                "out",
                SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,
                SND_SEQ_PORT_TYPE_APPLICATION
                );
    if ( outPort < 0 ) {
        qDebug() << "Unable to create ALSA midi port";
        return;
    }
    inPort = snd_seq_create_simple_port(
                sequencerHandle,
                "in",
                SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
                SND_SEQ_PORT_TYPE_APPLICATION
                );
    if ( outPort < 0 ) {
        qDebug() << "Unable to create ALSA midi port";
        return;
    }
    // setup notifiers for messages ready
    int npfd = snd_seq_poll_descriptors_count(sequencerHandle, POLLIN);
    struct pollfd* pfd = (struct pollfd *)alloca(npfd * sizeof(struct pollfd));
    snd_seq_poll_descriptors(sequencerHandle, pfd, npfd, POLLIN|POLLOUT);
    for(int x = 0;x < npfd;x++){
        int alsaEventFd = pfd[x].fd;
        // these should be cleaned up with parent (ie this!)
        QSocketNotifier*  notifier = new QSocketNotifier(alsaEventFd, QSocketNotifier::Read, this);
        connect(notifier, SIGNAL(activated(int)), this, SLOT(processInput()));
    }

    // set up midi send worker thread
    workerThread = new QThread;
    worker = new MidiOutWorker(sequencerHandle, outPort);
    worker->moveToThread(workerThread);
    connect(this, SIGNAL(sysex(QByteArray,int)), worker, SLOT(sendSysex(QByteArray,int)));
    connect(worker, SIGNAL(sysexComplete(int)), this, SLOT(sysExComplete(int)));
    connect(worker, SIGNAL(progress(int,int,int)), this, SLOT(midiOutProgress(int,int,int)));
    workerThread->start();

    getSourcesDests();
}

void MidiDeviceAccess::getSourcesDests()
{

    unsigned int bits = SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ|SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE; //

    //clear source/dest vectors
    quNeoPorts.clear();

    snd_seq_client_info_t *cinfo;
    snd_seq_port_info_t *pinfo;

    snd_seq_client_info_alloca(&cinfo);
    snd_seq_port_info_alloca(&pinfo);
    snd_seq_client_info_set_client(cinfo, -1);
    while (snd_seq_query_next_client(sequencerHandle, cinfo) >= 0) {
        /* reset query info */
        snd_seq_port_info_set_client(pinfo, snd_seq_client_info_get_client(cinfo));
        snd_seq_port_info_set_port(pinfo, -1);
        while (snd_seq_query_next_port(sequencerHandle, pinfo) >= 0) {
            if((snd_seq_port_info_get_capability(pinfo) & (bits)) == (bits)){
                // print_port
                QString portName(snd_seq_port_info_get_name(pinfo));
                if(portName.contains("QUNEO", Qt::CaseInsensitive)){
                    int client = snd_seq_client_info_get_client(cinfo);
                    int port = snd_seq_port_info_get_port(pinfo);
                    QString clientName(snd_seq_client_info_get_name(cinfo));
                    quNeoPorts.push_back(AlsaPort(client, port, clientName, portName));
                    qDebug() << client << ":" << port << " - " << clientName << ":" << portName;
                }

            }
        }
    }

    //clear and repopulate device menu
    deviceMenu->clear();

    //if there are any dests add them to the menu
    if(quNeoPorts.size() == 0) {
        //if not dests, put "none" in the menu
        deviceMenu->insertItem(0, QString("None"));
        emit sigQuNeoConnected(false);
    } else {
        //enumerate quneos in menu
        for(vector<AlsaPort>::size_type i = 0; i < quNeoPorts.size(); i++) {
            deviceMenu->insertItem(i, QString("%1:%2 QuNeo").arg(quNeoPorts[i].client).arg(quNeoPorts[i].port) );
        }
        emit sigQuNeoConnected(true);
    }
}


void MidiDeviceAccess::doConnect(bool connect, snd_seq_port_subscribe_t* subs)
{
    int error;
    if(connect && (snd_seq_get_port_subscription(sequencerHandle, subs) < 0)) {
        if ((error = snd_seq_subscribe_port(sequencerHandle, subs)) < 0) {
            // emit error (snd_strerror(error))
            qDebug() << "Alsa Error connecting send port" << snd_strerror(error) << endl;
        }
    } else if (!connect && (snd_seq_get_port_subscription(sequencerHandle, subs) == 0)){
        if ((error = snd_seq_unsubscribe_port(sequencerHandle, subs)) < 0) {
            // emit error (snd_strerror(error))
            qDebug() << "Alsa Error connecting send port" << snd_strerror(error) << endl;
        }
    }
}

void MidiDeviceAccess::connectDevice(bool connect){

    qDebug() << "_____connectDevice() called_____";

    if(-1 < selectedDevice) {
        snd_seq_addr_t device, inPortAddress, outPortAddress;

        device.client = quNeoPorts[selectedDevice].client;
        device.port = quNeoPorts[selectedDevice].port;

        inPortAddress.client = outPortAddress.client = snd_seq_client_id(sequencerHandle);
        inPortAddress.port = inPort;
        outPortAddress.port = outPort;

        qDebug() << "connecting to " << quNeoPorts[selectedDevice].client << ":" << quNeoPorts[selectedDevice].port << " - "<< quNeoPorts[selectedDevice].portName;

        snd_seq_port_subscribe_t* subs;
        snd_seq_port_subscribe_alloca(&subs);

        //send port
        snd_seq_port_subscribe_set_dest(subs, &device);
        snd_seq_port_subscribe_set_sender(subs, &outPortAddress);
        doConnect(connect, subs);

        //recieve port
        snd_seq_port_subscribe_set_dest(subs, &inPortAddress);
        snd_seq_port_subscribe_set_sender(subs, &device);
        doConnect(connect, subs);

        //if not in bootloader mode, send sysex check fw message upon device connection/selection
        if(!inBootloader){
            slotCheckFirmwareVersion();
        }
    }
}

void MidiDeviceAccess::slotSelectDevice(int index){

    qDebug() << "_____ slotSelectDevice() called_____" << index;

    connectDevice(false);
    selectedDevice = (-1 < index && index < quNeoPorts.size()) ? index : -1;
    connectDevice();

}

void MidiDeviceAccess::slotUpdateAllPresets() { //used for updating all presets at once

        for(int i= 0; i< 16; i++){
            //encode current preset data
            sysExFormat->slotEncodePreset(i);

            //if there is a selected device, send down preset data via sysex protocol
            if(-1 < selectedDevice){
                emit sysex(sysExFormat->presetSysExByteArray, (ALL_PRESETS_FLAG | (i+1)));
                qDebug("update preset %d", i);
            }
        }
        slotLoadPreset();
}

void MidiDeviceAccess::slotUpdateSinglePreset(){

        //encode current preset
        sysExFormat->slotEncodePreset(currentPreset);

        //if there is a selected device
        if(-1 < selectedDevice){
            emit sysex(sysExFormat->presetSysExByteArray);
            qDebug("Preset Sysex Sent - Current Preset");

        }
        //load the currently selected preset
        slotLoadPreset();

}

void MidiDeviceAccess::slotLoadPreset() {
    if (-1 < selectedDevice) {
        // todo: function to generate this..
        emit sysex(loadPresetBytes[currentPreset]);
        qDebug("Preset Sysex Sent - Current Preset");
    }
}

void MidiDeviceAccess::slotUpdateFirmware(){//this function puts the board into bootloader mode************
    if(-1 < selectedDevice){
        emit sysex(QByteArray::fromRawData((char*)(enterBootloaderData), sizeof(enterBootloaderData)), ENTER_BOOTLOADER);
    }
    QTimer::singleShot(5000, callbackPointer, SLOT(slotDownloadFw()));
}

void MidiDeviceAccess::slotDownloadFw(){//this function sends the actual firmware data**********
    if(-1 < selectedDevice){
        // will have disconnected with bootloader call
        inBootloader = true;
        connectDevice();
        emit sysex(sysExFirmwareBytes, DOWNLOAD_FIRMWARE);
    }
}

void MidiDeviceAccess::slotCheckFirmwareVersion(){//this function checks versions with sysEx
    emit sysex(QByteArray::fromRawData((char*)(checkFwSysExData), sizeof(checkFwSysExData)));
    qDebug("check fw sent");
}

void MidiDeviceAccess::slotSwapLeds(){
    emit sysex(QByteArray::fromRawData((char*)(swapLedsSysExData), sizeof(swapLedsSysExData)));
}

void MidiDeviceAccess::slotSetCurrentPreset(QString selected){
    if(selected.contains("Preset ")){ //remove prepended preset
        if(selected.contains(" *")){    //remove asterisk if recalling unsaved/modified preset
            selected.remove(" *");
        }
        currentPreset = selected.remove(0,7).toInt() -1; //reduce string to number
    }
}

void MidiDeviceAccess::slotSendToggleProgramChangeOutput(){
    emit sysex(QByteArray::fromRawData((char*)(toggleProgramChangeOutSysExData), sizeof(toggleProgramChangeOutSysExData)));
}

void MidiDeviceAccess::slotSendToggleProgramChangeInput(){
    emit sysex(QByteArray::fromRawData((char*)(toggleProgramChangeInSysExData), sizeof(toggleProgramChangeInSysExData)));
}


//-------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
//------------------------------------------------Midi Handling------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
void MidiDeviceAccess::processInput() {
    do {
        snd_seq_event_input( sequencerHandle, &sequencerEvent );
        // currently accessing the sequencerEvent directly, may want to change this to using encode/decode alsa funcs
        if ( sequencerEvent->type == SND_SEQ_EVENT_SYSEX ) {
            tempBuffer.append( QByteArray( (char *) sequencerEvent->data.ext.ptr, sequencerEvent->data.ext.len ) );

            // ALSA splits sysex messages into chunks, currently of 256 max bytes in size
            // Therefore the data needs collection before sending it to the main thread
            if ( tempBuffer.startsWith( 0xF0 ) && tempBuffer.endsWith( 0xF7 ) )
            {
                // buffer contains complete sysex message - process it
                processSysex(tempBuffer);
                qDebug("Sysex Arrived");
                tempBuffer.clear();
            }
            else
            {
                qDebug("Chunk Arrived");
            }
        } else {
            qDebug("non SysEx Arrived");
            // empty buffer - any interrupted sysex is invalid
            tempBuffer.clear();
            // TODO: add processing of non-sysex message
        }
        snd_seq_free_event( sequencerEvent );
    } while ( snd_seq_event_input_pending( sequencerHandle, 0 ) > 0 );

}

void MidiDeviceAccess::processSysex(QByteArray message) {
    if (message.size() == 17 && message.at(3) == 6 && message.at(4) == 2) {
        // firmware inquery response
        qDebug() << "boot LSB" << (unsigned char) message.at(12);
        qDebug() << "boot MSB" << (unsigned char) message.at(13);
        qDebug() << "fw LSB" << (unsigned char) message.at(14);
        qDebug() << "fw MSB" << (unsigned char) message.at(15);

        //set version vars from sysex message
        bootloaderVersionLSB = (unsigned char)message.at(12);
        bootloaderVersionMSB = (unsigned char)message.at(13);
        fwVersionLSB = (unsigned char)message.at(14);
        fwVersionMSB = (unsigned char)message.at(15);

        boardVersion = QString("%1.%2.%3").arg(fwVersionMSB / 16).arg(fwVersionMSB % 16).arg(fwVersionLSB);
        boardVersionBoot = QString("%1.%2").arg(bootloaderVersionMSB).arg(bootloaderVersionLSB);

        //check version vars against version array
        if (versionArray[2] != fwVersionLSB ||
                versionArray[3] != fwVersionMSB % 16 ||
                versionArray[4] != fwVersionMSB / 16) {
            emit sigFirmwareCurrent(false);
        } else {
            emit sigFirmwareCurrent(true);
        }
    }
}

//-------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
//--Slots for handling status signals from MidiOutWorker ------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
void MidiDeviceAccess::midiOutProgress(int bytesSent, int toSend, int completeId){
    if(DOWNLOAD_FIRMWARE == completeId) {
        emit sigFwBytesLeft(toSend - bytesSent);
    }
}
    
void MidiDeviceAccess::sysExComplete(int completeId)
{

    //called after sysex messages are completely sent
    if(completeId == -1) {
        inBootloader = true;
        qDebug("entered bootloader");
    } else if(completeId == DOWNLOAD_FIRMWARE) {
        qDebug("fw download sent!");
        inBootloader = false;
        QTimer::singleShot(2000, callbackPointer, SLOT(connectDevice()));

    } else if ((completeId & ALL_PRESETS_FLAG) == ALL_PRESETS_FLAG) {
        qDebug() << "preset completed:"<<(completeId ^ ALL_PRESETS_FLAG);
        emit sigUpdateAllPresetsCount(completeId ^ ALL_PRESETS_FLAG);
    }
}

