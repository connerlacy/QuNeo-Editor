#ifndef MIDIDEVICEACCESS_H
#define MIDIDEVICEACCESS_H

#include <QtGui>

#include <QApplication>
#include <QObject>
#include <vector>
#include <string>
#include <utility> // for pair
#include <sysexformat.h>
#include "RtMidi.h"
//#include "copypastehandler.h"

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
    vector<unsigned char> sysExFirmwareData; //char array address for sysex firmware

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

    //--------RtMidi Midi Services Vars--------//
    RtMidiIn *midiIn;  //reference to our app's input port
    RtMidiOut *midiOut; //reference to our app's output port
    vector<pair<int, string> > quNeoDests; //vector of endpoint destination references (our devices' input ports)
    vector<pair<int, string> > quNeoSources; //vector of endpoint source references (our devices' output ports)
    int selectedDevice;

    int sourceCount;
    int destCount;

//    //-----Internal helper functions, not necessary for SIGS/SLOTS---------//

    void connectDevice(); //connects a single device


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

    void processSysex(vector<unsigned char> *message);

signals:
    void clearDeviceMenu(); //clears the device menu
    void populateDeviceMenu(QStringList); //adds devices to device menu
    void sigFirmwareCurrent(bool); //emits whether or not fw is current after checking
    void sigFwBytesLeft(int);
    void sigUpdateAllPresetsCount(int);
    void sigSetVersions(QString, QString);
    void sigQuNeoConnected(bool);

public slots:
    void slotSetCurrentPreset(QString); //sets preset var internally
    void slotSelectDevice(QString); //activates the selected device
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

};

#endif // MIDIDEVICEACCESS_H
