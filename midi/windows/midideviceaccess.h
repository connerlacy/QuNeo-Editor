#ifndef MIDIDEVICEACCESS_H
#define MIDIDEVICEACCESS_H

#include <QApplication>
#include <QtGui>
#include <QObject>
#include <QTimer>
#include <Windows.h>
#include <MMSystem.h>
#include <Dbt.h>
#include "sysexformat.h"

class MidiDeviceAccess : public QObject
{
    Q_OBJECT
public:
    explicit MidiDeviceAccess(QVariantMap*,QObject *parent = 0);

    SysExFormat *sysExFormat;
    int currentPreset;
    int fwVersionLSB; //stores LSB of FW version when query returns in slotProcessSysExRx(int);
    int fwVersionMSB; //stores MSB of FW version when query returns in slotProcessSysExRx(int);
    int bootloaderVersionLSB; //stores LSB of bootloader version when query returns in slotProcessSysExRx(int);
    int bootloaderVersionMSB;
    int versionArray[5];
    int isQuNeo;
    QList<int> sysExMsg;    //used to process sysEx messages from start to finish in slotProcessSysExRx(int);

    QTimer *timer;
    QTimer updatingAllTimer;

    QObject* mainWindow;

    QComboBox* deviceMenu;
    QPushButton* updateSingleButton;
    QPushButton* updateAllButton;
    int presetByteSize;

    QVariantMap deviceOutList;//stores USB Audio Devices (including QuNeos)
    int openOutDeviceIndex;
    QVariantMap deviceInList;
    int openInDeviceIndex;

    QVariantMap quNeoDeviceOutList; //stores QuNeo Devices, after USB Audio Device Checking
    QVariantMap quNeoDeviceInList;
    QString deviceBeingChecked;

    QFile *sysExFirmware;
    QByteArray sysExFirmwareBytes;
    char *sysExFirmwareData;
    int firmwareByteSize;

    QFile* loadPreset[16];
    QByteArray loadPresetBytes[16];
    char *loadPresetData[16];
    int loadPresetSize[16];

    QMessageBox deviceNumWarning;

    MIDIOUTCAPS mocs;
    HMIDIOUT outHandle;
    HANDLE sysExOutBuffer;
    MIDIHDR sysExOutHdr;
    UINT err;
    int numOutDevices;

    MIDIINCAPS mics;
    HMIDIIN inHandle;
    HANDLE sysExInBuffer;
    MIDIHDR sysExInHdr;
    int numInDevices;

    HANDLE hBuffer;

    QString boardVersion;
    QString editorVersion;
    QString editorVersionBoot;
    QString boardVersionBoot;

    //char sysEx[] = {0xF0, 0x7F, 0x7F, 0x04, 0x04, 0x01, 0x7F, 0x7F, 0xF7};

signals:
    void sigFirmwareCurrent(bool);
    void sigFwBytesLeft(int);
    void sigUpdateAllPresetsCount(int);
   // void sigSetVersions(QString,QString);
    void sigQuNeoConnected(bool);

public slots:
    void slotCheckDevices(); //checks for USB Audio devices if midi setup has changed
    void slotCheckDevicesPulse(); //executes timer for checking the midi setup
    void slotSetCurrentPreset(QString); //gets current preset number
    void slotSelectDevice(QString);//not needed anymore with device number limit
    void slotUpdateAllPresets();
    void slotUpdateSinglePreset();
    void slotLoadPreset(); //loads updated preste onto board
    void slotUpdateFirmware(); //puts board into bootloader
    void slotDownloadFirmware(); // downloads firmare
    void slotCheckFirmwareVersion(); //sends device query
    void slotProcessSysEx(QByteArray); //processes sysex, if this becomes too large, make new class
    void slotIsDeviceQuneo(QString); //sends device id query, in device check mode
    void slotPopulateDeviceMenu(); //checks the number of quneos connected, and populates menu
    void slotSwapLeds(); //sends sysex message to swap leds

    //these open and close midi ports/devices
    void slotCloseMidiIn();
    void slotCloseMidiOut();
    void slotOpenMidiIn(int);
    void slotOpenMidiOut(int);

    void slotSendToggleProgramChangeOutput();
    void slotSendToggleProgramChangeInput();

};

#endif // MIDIDEVICEACCESS_H
