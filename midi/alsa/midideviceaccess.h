#ifndef MIDIDEVICEACCESS_H
#define MIDIDEVICEACCESS_H

#include <QtGui>

#include <QApplication>
#include <QObject>
#include <vector>
#include <string>
#include <utility> // for pair
#include <sysexformat.h>
#include <alsa/asoundlib.h>
#include "midioutworker.h"

//#include "copypastehandler.h"

#define DOWNLOAD_FIRMWARE 1
#define ENTER_BOOTLOADER 2
#define NEXT_SYSEX 4

using std::vector;
using std::pair;
using std::string;


class MidiDeviceAccess : public QObject
{
    Q_OBJECT

public:
    explicit MidiDeviceAccess(QVariantMap*, QObject *parent = 0);

    //---------SysEx Formatting and File Loading-----------//
    SysExFormat *sysExFormat; //instance of sysex formatter
    QString pathSysex; //path of sysex firmware file in build
    QFile *sysExFirmware; //sysex firmware file
    QByteArray sysExFirmwareBytes; //sysex firmware as byte array

    //---------Bootloader and Firmware Versioning vars--------//
    int fwVersionLSB; //stores LSB of FW version when query returns in slotProcessSysExRx(int);
    int fwVersionMSB; //stores MSB of FW version when query returns in slotProcessSysExRx(int);
    int bootloaderVersionLSB; //stores LSB of bootloader version when query returns in slotProcessSysExRx(int);
    int bootloaderVersionMSB; //stores MSB of bootloader version when query returns in slotProcessSysExRx(int);
    int versionArray[5]; //array containing the most recent verison vars above. above vars are compared with these arrya elements
    bool firmwareUpToDate; //bool describing whether or not the bootloader and fw version are current

    //--------Ui Objects and vars associated with MIDI Functionality-------//
    QObject *mainWindow; //pointer to main window
    QComboBox* deviceMenu; //pointer to device menu
    QPushButton* updateDeviceButton; //name is confusing-- actually updates a single preset
    QPushButton* updateAllButton;   //updates all presets, currently disabled
    QComboBox *presetMenu; //used for gathering current preset for updating
    int currentPreset; //preset to update

    //--------ALSA Midi Services Vars--------//
    QThread* workerThread;
    MidiOutWorker* worker;
    snd_seq_t * sequencerHandle;
    snd_seq_event_t* sequencerEvent;

    int outPort, inPort;
    QByteArray tempBuffer;

    QSocketNotifier* seqNotifier;

    struct AlsaPort {
        int client;
        int port;
        QString clientName;
        QString portName;
        AlsaPort() : client(-1), port(-1), clientName("non"), portName("non") {}
        AlsaPort(int client, int port, QString clientName, QString portName) :client(client), port(port), clientName(clientName), portName(portName) {}
    };

    vector<AlsaPort> quNeoPorts; //vector of endpoint source references (our devices' output ports)
    int selectedDevice;

    //-----Internal helper functions, not necessary for SIGS/SLOTS---------//
    void connectDevice(bool connect=true); //connects a single device

    //------- Load Preset Files ------///
    QFile* loadPreset[16];
    QByteArray loadPresetBytes[16];
    char *loadPresetData[16];
    int loadPresetSize[16];


    QMessageBox msgBox; //for too many quneos message

    QString editorVersion;
    QString boardVersion;
    QString editorVersionBoot;
    QString boardVersionBoot;

    void processSysex(QByteArray message);

    void doConnect(snd_seq_port_subscribe_t* subs, int error, bool connect);
    void doConnect(bool connect, snd_seq_port_subscribe_t* subs);
signals:
    void clearDeviceMenu(); //clears the device menu
    void populateDeviceMenu(QStringList); //adds devices to device menu
    void sigFirmwareCurrent(bool); //emits whether or not fw is current after checking
    void sigFwBytesLeft(int);
    void sigUpdateAllPresetsCount(int);
    void sigSetVersions(QString, QString);
    void sigQuNeoConnected(bool);
    void sysex(QByteArray, int sysexCompleteId=0);

public slots:
    void slotSetCurrentPreset(QString); //sets preset var internally
    void slotSelectDevice(int); //activates the selected device
    void slotUpdateAllPresets(); //will update all presets
    void slotUpdateSinglePreset(); //updates current preset
    void slotUpdateFirmware(); //puts board into bootloader mode, waits 5 seconds, then updates fw
    void slotCheckFirmwareVersion(); //checks fw and bootloader versions
    void slotDownloadFw(); //called 5s after board enters bootloader
    void slotLoadPreset(); //activates preset on the board after an update
    void slotSwapLeds(); //sends sysex message to swap leds

    void getSourcesDests();//gathers references to QuNeo endpoints (device ports)

    void slotSendToggleProgramChangeOutput();
    void slotSendToggleProgramChangeInput();
    void processInput();
    void sysExComplete(int completeId);
    void midiOutProgress(int bytesSent, int toSend, int completeId);

};

#endif // MIDIDEVICEACCESS_H
