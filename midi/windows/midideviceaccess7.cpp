#include "midi/windows/midideviceaccess.h"

MidiDeviceAccess* callbackPointer;
void CALLBACK midiInCallback(HMIDIIN, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);
void CALLBACK midiOutCallback(HMIDIOUT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);
void Main_OnDeviceChange(HWND hwn, WPARAM wParam, LPARAM lParam);

char checkFwSysExData[] = {0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7};

char swapLedsSysExData[] = {0xF0, 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x50, 0x02, 0x44, 0x5D, 0x00, 0x10, 0xF7};

char toggleProgramChangeInSysExData[] = {0xF0, 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x50, 0x06, 0x04, 0x59, 0x00, 0x30, 0xF7};
char toggleProgramChangeOutSysExData[] ={0xF0, 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x50, 0x05, 0x34, 0x3A, 0x00, 0x30, 0xF7};

//sysEx array for entering bootloader mode
char enterBootloaderData[] = {0xF0, 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x11, 0x00, 0x5A, 0x62, 0x00, 0x30, 0xF7};

//var for determining if board has been set to bootloader mode
bool inBootloader = 0;
bool inDeviceCheck = false;
int  updatingAllCount = 0;
bool updatingAll = false;

MidiDeviceAccess::MidiDeviceAccess(QVariantMap* presetMapsCopy,QObject *parent) :
    QObject(parent)
{
    currentPreset = 0;

    versionArray[0] = 3;
    versionArray[1] = 1;
    versionArray[2] = 24;
    versionArray[3] = 2;
    versionArray[4] = 1;

    isQuNeo = 0x1E;

    sysExFormat = new SysExFormat(presetMapsCopy, 0);
    mainWindow = parent;

    boardVersion = QString("Not Connected");
    editorVersion = QString("%1.%2.%3").
            arg(versionArray[4]).
            arg(versionArray[3]).
            arg(versionArray[2]);

    boardVersionBoot = QString("Not Connected");
    editorVersionBoot = QString("%1.%2").
            arg(versionArray[1]).
            arg(versionArray[0]);

    deviceMenu = mainWindow->findChild<QComboBox *>("deviceMenu");
    connect(deviceMenu, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotSelectDevice(QString)));
    deviceMenu->setEnabled(false);

    updateSingleButton = mainWindow->findChild<QPushButton *>("updateButton");
    connect(updateSingleButton, SIGNAL(clicked()), this, SLOT(slotUpdateSinglePreset()));

    updateAllButton = mainWindow->findChild<QPushButton *>("updateAll");
    connect(updateAllButton, SIGNAL(clicked()), this, SLOT(slotUpdateAllPresets()));

    //-------------------Load Firmware SysEx and Enter Bootloader SysEx-------------------//

    //******Firmware*******//
    sysExFirmware = new QFile(":Quneo/sysex/resources/sysex/Quneo.syx");
    if(sysExFirmware->open(QIODevice::ReadOnly)){
      //  qDebug("sysExOpen");
    }
    sysExFirmwareBytes = sysExFirmware->readAll();
    //qDebug() << "sysex size" << sysExFirmwareBytes.size();
    firmwareByteSize = sysExFirmwareBytes.size();
    sysExFirmwareData = new char[sysExFirmwareBytes.size()];
    sysExFirmwareData = sysExFirmwareBytes.data();


    //******* Load Preset SysEx Files ********//
    for(int i = 0; i<16; i++){
        loadPreset[i] = new QFile(QString(":Quneo/sysex/resources/sysex/loadPresets/loadPreset%1.syx").arg(i));
        if(loadPreset[i]->open(QIODevice::ReadOnly)){
            qDebug() << "load preset open" << i;
        } else {
            qDebug() << "could not open load preset" << i;
        }

        loadPresetBytes[i] = loadPreset[i]->readAll();
        loadPresetSize[i] = loadPresetBytes[i].size();
        loadPresetData[i] = new char[loadPresetSize[i]];
        loadPresetData[i] = loadPresetBytes[i].data();
    }

    numInDevices = 0;
    numOutDevices = 0;

    //set up timer to check for new devices (easier than using windows' window message system)
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotCheckDevicesPulse()));
    timer->start(10);

    //timer for iterating through all presets (just calls single update)
    updatingAllTimer.setInterval(500);
    connect(&updatingAllTimer, SIGNAL(timeout()), this, SLOT(slotUpdateSinglePreset()));

    //for(int i=0; i <16; i++){
    //sysExFormat->slotEncodePreset(i);
    //}
}

void MidiDeviceAccess::slotCheckDevicesPulse(){

        slotCheckDevices();
}

void MidiDeviceAccess::slotCheckDevices(){

   // timer->stop();
    //qDebug() << "Check devices";
    //-------------- Get / Process Input Devices --------------//

    //if number of devices on this current check differs from last, get clear and populate quneo dev list
    if(numInDevices != midiInGetNumDevs() || numOutDevices != midiOutGetNumDevs()){

        boardVersion = QString("Not Connected");

        if(!inBootloader){

        qDebug() << "MIDI CHANGED**********************";

        slotCloseMidiOut();
        slotCloseMidiIn();

        QString str; //string for storing device names

        deviceOutList.clear(); //clear device out list
        deviceInList.clear(); //clear device in list
        quNeoDeviceOutList.clear();
        quNeoDeviceInList.clear();
        deviceBeingChecked.clear();
        deviceMenu->clear(); //clear device menu

        numInDevices = midiInGetNumDevs(); //reset input device count to current count

        //iterate through number of input devices
        for(int i = 0; i < numInDevices; i++){

            str.clear();//clear string (display name)
            midiInGetDevCaps(i, &mics, sizeof(MIDIINCAPS));//get device properties of current device index

            int j = 0; //initialize string index
            while(mics.szPname[j] != '\0'){//iterate through chars in current device name
                QChar ch = mics.szPname[j]; //get the char
                str.insert(j, ch); //assigne char to string
                j++; //advance count
            }

            if(str.contains("QUNEO")){ //if string is a ubs audio device
                deviceInList.insert(str, i); //insert it into the device list map with stored midi device index
            }
        }

        //-------------- Get / Process Output Devices --------------//
        numOutDevices = midiOutGetNumDevs(); //reset previous outdevice count to current count

        //iterate through output devices
        for(int i = 0; i < numOutDevices; i++){

            str.clear();//clear string
            midiOutGetDevCaps(i, &mocs, sizeof(MIDIINCAPS));//get device properties of current device index

            int j = 0; //initialize string counter
            while(mocs.szPname[j] != '\0'){ //get characters until end of string is reached
                QChar ch = mocs.szPname[j]; //assign char
                str.insert(j, ch); //insert char into string
                j++; //advance index
            }

            if(str.contains("QUNEO")){ //if device is USBAD (xp) add to out list
                deviceOutList.insert(str, i); //insert string into device out list
            }
        }


        //-------------- Check for QuNeos --------------//
        if(deviceOutList.isEmpty()){ //if there is nothing inside the outlist...
            deviceMenu->addItem("None"); //populate menu with none
            //emit sigSetVersions(editorVersion, boardVersion);
            emit sigQuNeoConnected(false);
        } else {

            /*//iterate through usb audio devices and check to see if they're quneos
            for(QVariantMap::iterator i = deviceOutList.begin();
                i != deviceOutList.end(); i++){ //if there is stuff in out list...

                inDeviceCheck = true; //set that we are in a device check (this controls while loop and waits for response)

                deviceBeingChecked = i.key(); //set current device being checked string, this is a global var

                //query device



                int timeout = 0;
                while(inDeviceCheck && (timeout < 20)){
                    slotIsDeviceQuneo(deviceBeingChecked);
                    qDebug() << "device being checked" << deviceBeingChecked;
                    Sleep(200);
                    qDebug() << "while loop timeout (< 20)" << timeout;
                    timeout++;
                }

            }*/

            //get a quneo count / populate menu if only one plugged in restart timer
            quNeoDeviceInList = deviceInList;
            quNeoDeviceOutList = deviceOutList;
            slotPopulateDeviceMenu();
            emit sigQuNeoConnected(true);


            }
        }
    }

    //timer->start();


}

void MidiDeviceAccess::slotOpenMidiIn(int index){

    DWORD   err;

       /* Is it not yet open? */
       if (!inHandle)
       {
          /* Open MIDI Input and set Windows to call my
          midiInputEvt() callback function. You may prefer
          to have something other than CALLBACK_FUNCTION. Also,
          I open device 0. You may want to give the user a choice */
          if (!(err = midiInOpen(&inHandle, index, (DWORD)midiInCallback, 0, CALLBACK_FUNCTION)))
          {
             /* Start recording Midi and return if SUCCESS */
             if (!(err = midiInStart(inHandle)))
             {
                //return(0);
             }

          /* ============== ERROR ============== */

          /* Close MIDI In and zero handle */
          slotCloseMidiIn();


       }

          /* Return the error */
          qDebug() << "OPEN MIDI IN ERR:"<< (err);

    }
}

void MidiDeviceAccess::slotOpenMidiOut(int index){

    DWORD   err;

       /* Is it not yet open? */
       if (!outHandle)
       {
          /* Open MIDI Output. */
          if (!(err = midiOutOpen(&outHandle, index, (DWORD_PTR)midiOutCallback, (DWORD_PTR)this, CALLBACK_FUNCTION)))
          {
             //return(0);
          }

          /* ============== ERROR ============== */
       }

       /* Return the error */
       qDebug() << "OPEN MIDI OUT ERR:"<< (err);

}

void MidiDeviceAccess::slotCloseMidiIn(){

    DWORD   err;

       /* Is the device open? */
       if ((err = (DWORD)inHandle))
       {
          /* Unqueue any buffers we added. If you don't
          input System Exclusive, you won't need this */
          midiInReset(inHandle);

          /* Close device */
          if (!(err = midiInClose(inHandle)))
          {
             /* Clear handle so that it's safe to call closeMidiIn() anytime */
             inHandle = 0;
          }
       }

       qDebug() << "CLOSE MIDI IN ERR" << err;
}

void MidiDeviceAccess::slotCloseMidiOut(){
                  DWORD   err;

                    /* Is the device open? */
                    if ((err = (DWORD)outHandle))
                    {
                       /* If you have any system exclusive buffers that
                       you sent via midiOutLongMsg(), and which are still being output,
                       you may need to wait for their MIDIERR_STILLPLAYING flags to be
                       cleared before you close the device. Some drivers won't close with
                       pending output, and will give an error. */

                       /* Close device */
                       if (!(err = midiOutClose(outHandle)))
                       {
                          /* Clear handle so that it's safe to call closeMidiOut() anytime */
                          outHandle = 0;
                       }
                    }


                  qDebug() << "CLOSE MIDI OUT ERR" << err;
}

void MidiDeviceAccess::slotIsDeviceQuneo(QString deviceName){


    //--------- Manage Output Port ---------//
    //upon new selection close current in/out ports (basically a midi reset reset)
    slotCloseMidiIn();
    slotCloseMidiOut();

    //Sleep(500);

    if(deviceOutList.contains(deviceName)){//if device outlist contains string (which is should if sent to this function

        midiOutOpen( &outHandle, //open up output at handle
                    deviceOutList.value(deviceName).toInt(), //device index, get from variant map
                    (DWORD_PTR)midiOutCallback, //set call back function
                    (DWORD_PTR)this, //reference passed to callback
                    CALLBACK_FUNCTION); //specify we're using a callback

        openOutDeviceIndex = deviceOutList.value(deviceName).toInt(); //store currently open output index

        qDebug() << "open out device:" << openOutDeviceIndex;
    }


    //--------- Manage Input Port ---------//
    //close in port currently open at app handle

    if(deviceInList.contains(deviceName)){//if inlist contains string...


       MMRESULT errM =  midiInOpen(&inHandle, //open up input at app in handle
                       deviceInList.value(deviceName).toInt(), //get device in index stored at string in map
                       (DWORD_PTR)midiInCallback, //use this midi in callback
                       (DWORD_PTR)this, //pass this class address too the call back as a reference
                       CALLBACK_FUNCTION); //specify we're using a callback

        openInDeviceIndex = deviceInList.value(deviceName).toInt(); //store currently open input index
        qDebug() << "open in device:" << openInDeviceIndex;

        //---- Manage SysEx Input Stuff ----//
        MMRESULT err;

        sysExInBuffer = GlobalAlloc(GHND, 500); //allocate sysex input buffer
        if(sysExInBuffer){ //if exists...
            sysExInHdr.lpData = (LPSTR)GlobalLock(sysExInBuffer); //set pointer to our buffer in header struct
            if(sysExInHdr.lpData){ //if above pointer successfully set...
                sysExInHdr.dwBufferLength = 500; //allocate an input of 500 byte length
                sysExInHdr.dwFlags = 0; //no flags

                err = midiInPrepareHeader(inHandle, &sysExInHdr, sizeof(MIDIHDR)); //prepare the header (return MMRESULT err)
                if(err == MMSYSERR_NOERROR){ //if not error...
                    err = midiInAddBuffer(inHandle, &sysExInHdr, sizeof(MIDIHDR)); //add the buffer to our current device
                    if(err == MMSYSERR_NOERROR){ //if no error...
                        err = midiInStart(inHandle); //start midi input***** (there's no midi out start)
                        if(err != MMSYSERR_NOERROR){ //if error in starting...
                            qDebug("couldn't open midi in"); //print error
                        } else {
                            qDebug("device open"); //if successful, print device open
                            slotCheckFirmwareVersion(); // call actual firmware check
                        }
                    }
                }
            }
        }
    }
}

void MidiDeviceAccess::slotCheckFirmwareVersion(){

    sysExOutBuffer = GlobalAlloc(GHND, 6);
    sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
    sysExOutHdr.dwBufferLength = 6;
    sysExOutHdr.dwFlags = 0;

    err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

    if(!err){

        memcpy(sysExOutHdr.lpData, &checkFwSysExData[0], 6);

        err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

        if(err){
            char errMsg[120];
            midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
            qDebug()<<"err:" << errMsg;
        }
    }

    // while(sysExOutHdr.dwBytesRecorded < sysExOutHdr.dwBufferLength){
         //qDebug() << "offset" << sysExOutHdr.;
     //}

}

void MidiDeviceAccess::slotSwapLeds(){

    sysExOutBuffer = GlobalAlloc(GHND, 17);
    sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
    sysExOutHdr.dwBufferLength = 17;
    sysExOutHdr.dwFlags = 0;

    err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

    if(!err){

        memcpy(sysExOutHdr.lpData, &swapLedsSysExData[0], 17);

        err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

        if(err){
            char errMsg[120];
            midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
            qDebug()<<"err:" << errMsg;
        }
    }
}

void MidiDeviceAccess::slotSendToggleProgramChangeInput(){

    sysExOutBuffer = GlobalAlloc(GHND, 17);
    sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
    sysExOutHdr.dwBufferLength = 17;
    sysExOutHdr.dwFlags = 0;

    err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

    if(!err){

        memcpy(sysExOutHdr.lpData, &toggleProgramChangeInSysExData[0], 17);

        err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

        if(err){
            char errMsg[120];
            midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
            qDebug()<<"err:" << errMsg;
        }
    }
}

void MidiDeviceAccess::slotSendToggleProgramChangeOutput(){

    sysExOutBuffer = GlobalAlloc(GHND, 17);
    sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
    sysExOutHdr.dwBufferLength = 17;
    sysExOutHdr.dwFlags = 0;

    err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

    if(!err){

        memcpy(sysExOutHdr.lpData, &toggleProgramChangeOutSysExData[0], 17);

        err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

        if(err){
            char errMsg[120];
            midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
            qDebug()<<"err:" << errMsg;
        }
    }
}

void MidiDeviceAccess::slotPopulateDeviceMenu(){



    qDebug() << "q count" << quNeoDeviceOutList.size();

   if(quNeoDeviceOutList.size() > 1){
        QMessageBox::StandardButton ret;

        ret = QMessageBox::warning((QWidget*)mainWindow, QString("Application"),
                     QString("You have too many QuNeos plugged in."));

        if(ret == QMessageBox::Ok){
            numInDevices = -1;
            numOutDevices = -1;
            quNeoDeviceOutList.clear();
            quNeoDeviceInList.clear();
            slotCheckDevices();
        }



   } else if(quNeoDeviceOutList.size() != 0){
       inDeviceCheck = false;
        for(QVariantMap::iterator i = quNeoDeviceOutList.begin();
            i != quNeoDeviceOutList.end(); i++){ //if there is stuff in out list...
            deviceMenu->addItem(i.key()); //put it in the menu (which will also activate it and call slotSelectDevice)
        }

        qDebug() << "CHECK FIRMWARE AFTER MENU POP";
        QTimer::singleShot(1000, this, SLOT(slotCheckFirmwareVersion()));




   } else {
   emit sigQuNeoConnected(false);
   }


    //timer->start();


}

void MidiDeviceAccess::slotSelectDevice(QString str){



    //--------- Manage Output Port ---------//
    //upon new selection close current in/out ports (basically a midi reset reset)
    slotCloseMidiIn();
    slotCloseMidiOut();

    //Sleep(500);

    if(deviceOutList.contains(str)){//if device outlist contains string (which is should if sent to this function

        midiOutOpen( &outHandle, //open up output at handle
                    deviceOutList.value(str).toInt(), //device index, get from variant map
                    (DWORD_PTR)midiOutCallback, //set call back function
                    (DWORD_PTR)this, //reference passed to callback
                    CALLBACK_FUNCTION); //specify we're using a callback

        openOutDeviceIndex = deviceOutList.value(str).toInt(); //store currently open output index

        qDebug() << "open out device:" << openOutDeviceIndex;

    }


    //--------- Manage Input Port ---------//
    //close in port currently open at app handle

    if(deviceInList.contains(str)){//if inlist contains string...


       MMRESULT errM =  midiInOpen(&inHandle, //open up input at app in handle
                       deviceInList.value(str).toInt(), //get device in index stored at string in map
                       (DWORD_PTR)midiInCallback, //use this midi in callback
                       (DWORD_PTR)this, //pass this class address too the call back as a reference
                       CALLBACK_FUNCTION); //specify we're using a callback

        openInDeviceIndex = deviceInList.value(str).toInt(); //store currently open input index
        qDebug() << "open in device:" << openInDeviceIndex;

        //---- Manage SysEx Input Stuff ----//
        MMRESULT err;

        sysExInBuffer = GlobalAlloc(GHND, 500); //allocate sysex input buffer
        if(sysExInBuffer){ //if exists...
            sysExInHdr.lpData = (LPSTR)GlobalLock(sysExInBuffer); //set pointer to our buffer in header struct
            if(sysExInHdr.lpData){ //if above pointer successfully set...
                sysExInHdr.dwBufferLength = 500; //allocate an input of 500 byte length
                sysExInHdr.dwFlags = 0; //no flags

                err = midiInPrepareHeader(inHandle, &sysExInHdr, sizeof(MIDIHDR)); //prepare the header (return MMRESULT err)
                if(err == MMSYSERR_NOERROR){ //if not error...
                    err = midiInAddBuffer(inHandle, &sysExInHdr, sizeof(MIDIHDR)); //add the buffer to our current device
                    if(err == MMSYSERR_NOERROR){ //if no error...
                        err = midiInStart(inHandle); //start midi input***** (there's no midi out start)
                        if(err != MMSYSERR_NOERROR){ //if error in starting...
                            qDebug("couldn't open midi in"); //print error
                        } else {
                            qDebug("device open"); //if successful, print device open
                            //slotCheckFirmwareVersion(); // call actual firmware check
                        }
                    }
                }
            }
        }
    }

}

void MidiDeviceAccess::slotUpdateSinglePreset(){

    if(deviceMenu->currentText() == "QUNEO"){
    if(!updatingAll){

    sysExFormat->slotEncodePreset(currentPreset);
    qDebug("UPDATE SINGLE PRESET CALLED");

    char *presetSysExData = new char(sysExFormat->presetSysExByteArray.size());
    presetSysExData = sysExFormat->presetSysExByteArray.data();

    sysExOutBuffer = GlobalAlloc(GHND, sysExFormat->presetSysExByteArray.size());
    sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
    sysExOutHdr.dwBufferLength = sysExFormat->presetSysExByteArray.size();
    sysExOutHdr.dwFlags = 0;


    err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

        if(!err){

            memcpy(sysExOutHdr.lpData, &presetSysExData[0], sysExFormat->presetSysExByteArray.size());

            err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

            if(err){
                char errMsg[120];
                midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
                qDebug()<<"err:" << errMsg;
            }
        }
        QTimer::singleShot(500, this, SLOT(slotLoadPreset()));
    } else {

        sysExFormat->slotEncodePreset(updatingAllCount);

        char *presetSysExData = new char(sysExFormat->presetSysExByteArray.size());
        presetSysExData = sysExFormat->presetSysExByteArray.data();

        sysExOutBuffer = GlobalAlloc(GHND, sysExFormat->presetSysExByteArray.size());
        sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
        sysExOutHdr.dwBufferLength = sysExFormat->presetSysExByteArray.size();
        sysExOutHdr.dwFlags = 0;

        qDebug() << "UPDATE ALL PRESET" << updatingAllCount;
        err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

            if(!err){

                memcpy(sysExOutHdr.lpData, &presetSysExData[0], sysExFormat->presetSysExByteArray.size());

                err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

                if(err){
                    char errMsg[120];
                    midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
                    qDebug()<<"err:" << errMsg;
                }
            }

            if(updatingAllCount < 15){
                emit sigUpdateAllPresetsCount(updatingAllCount+1);
                updatingAllCount++;
            } else {
                updatingAll = false;
                updatingAllTimer.stop();
                updatingAllCount = 0;
                emit sigUpdateAllPresetsCount(16);
                QTimer::singleShot(250, this, SLOT(slotLoadPreset()));
            }

    }
    }

}

void MidiDeviceAccess::slotLoadPreset(){

    qDebug() << "LOAD PRESET CALLED" << "size:" << loadPresetSize[currentPreset];
    qDebug() << "current preset" << currentPreset;

    char* data = new char[loadPresetSize[currentPreset]];
    data = loadPresetData[currentPreset];

    sysExOutBuffer = GlobalAlloc(GHND, loadPresetSize[currentPreset]);
    sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
    sysExOutHdr.dwBufferLength = loadPresetSize[currentPreset];
    sysExOutHdr.dwFlags = 0;


    MMRESULT err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

    if(!err){

        memcpy(sysExOutHdr.lpData, &data[0], loadPresetSize[currentPreset]);

        err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

        if(err){
            char errMsg[120];
            midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
            qDebug()<<"err:" << errMsg;
        }
    } else {
        qDebug() << "load preset err" << err;
    }

}

void MidiDeviceAccess::slotUpdateAllPresets(){
    if(deviceMenu->currentText() == "QUNEO"){
    updatingAll = true;
    updatingAllTimer.start();
    }
}

void MidiDeviceAccess::slotUpdateFirmware(){

    qDebug() << "UPDATE FIRMWARE CALLED";

        sysExOutBuffer = GlobalAlloc(GHND, 17);
        sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
        sysExOutHdr.dwBufferLength = 17;
        sysExOutHdr.dwFlags = 0;

        err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

        if(!err){

            memcpy(sysExOutHdr.lpData, &enterBootloaderData[0], 17);

            err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

            if(err){
                char errMsg[120];
                midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
                qDebug()<<"err:" << errMsg;
            }
        } else {
            qDebug() << "err:::::bootloader "<< err;
        }
}

void MidiDeviceAccess::slotDownloadFirmware(){

    qDebug() << "DOWNLOAD FIRMWARE CALLED";
    sysExOutBuffer = GlobalAlloc(GHND, firmwareByteSize);
    sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
    sysExOutHdr.dwBufferLength = firmwareByteSize;
    sysExOutHdr.dwFlags = 0;

    err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

    if(!err){

        memcpy(sysExOutHdr.lpData, &sysExFirmwareData[0], firmwareByteSize);

        err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

        if(err){
            char errMsg[120];
            midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
            qDebug()<<"err:" << errMsg;
        }
    } else {
        qDebug() << "err::::: "<< err;
    }

}

void MidiDeviceAccess::slotSetCurrentPreset(QString selected){
    if(selected.contains("Preset ")){ //remove prepended preset
        if(selected.contains(" *")){    //remove asterisk if recalling unsaved/modified preset
            selected.remove(" *");
        }

        currentPreset = selected.remove(0,7).toInt()-1;
        qDebug() << "Current Preset" << currentPreset;
    }
}

void MidiDeviceAccess::slotProcessSysEx(QByteArray ba){

    QChar ch;
    sysExMsg.clear();

    //qDebug() << "size" << ba.size();
    for(int i = 0; i < ba.size(); i++){
        ch = ba.at(i);
        sysExMsg.append(ch.unicode());
        qDebug("sysEx %x", ch.unicode());
    }

    if(sysExMsg.size() == 17 && sysExMsg.at(3) == 6 && sysExMsg.at(4) == 2){

        //set version vars from sysex message
        fwVersionLSB = sysExMsg.at(14);
        fwVersionMSB = sysExMsg.at(15);
        bootloaderVersionLSB = sysExMsg.at(12);
        bootloaderVersionMSB = sysExMsg.at(13);
        inBootloader = sysExMsg.at(9);

        boardVersion = QString("%1.%2.%3").arg(fwVersionMSB/16).arg(fwVersionMSB%16).arg(fwVersionLSB);
        boardVersionBoot = QString("%1.%2").arg(bootloaderVersionMSB).arg(bootloaderVersionLSB);

       // emit sigSetVersions(editorVersion, boardVersion);

        if(!inBootloader){ //if not in bootloader....

            if(!inDeviceCheck){ //if not in device check....
                if(//versionArray[0] != bootloaderVersionLSB || //check version vars against version array
                    //versionArray[1] != bootloaderVersionMSB ||
                    versionArray[2] != fwVersionLSB ||
                    versionArray[3] != fwVersionMSB%16 ||
                    versionArray[4] != fwVersionMSB/16){

                    emit sigFirmwareCurrent(false);
                    //qDebug() << "version good" << false;
                } else {
                    qDebug() << "version good" << true;
                    emit sigFirmwareCurrent(true);
                }

            } else { // if in device check...

                if(sysExMsg.at(5) == 0x00 && //if device code equals quneo device code//
                   sysExMsg.at(6) == 0x01 &&
                   sysExMsg.at(7) == 0x5F &&
                   sysExMsg.at(8) == 0x1E){
                    //insert string into quneo device list, otheriwse do nothing
                    quNeoDeviceOutList.insert(deviceBeingChecked, deviceOutList.value(deviceBeingChecked));
                    quNeoDeviceInList.insert(deviceBeingChecked, deviceInList.value(deviceBeingChecked));
                    qDebug() << "add Quneo TO LIST" << deviceBeingChecked;
                }
                inDeviceCheck = false;
            }
        } else {
            inDeviceCheck = false;
            inBootloader = false;
            slotDownloadFirmware();

        }
    }
}

void CALLBACK midiInCallback(HMIDIIN handle, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam, DWORD_PTR dwParam1){
    /*/qDebug() << MIM_DATA << uMsg;
    //qDebug() << dwParam;
    qDebug()    <<"status"<< ((dwParam) & 0xFF)
                << "data1" << ((dwParam>>8) & 0xFF)
                << "data2" << ((dwParam>>16) & 0xFF); //status byte*/

    MidiDeviceAccess *mda = (MidiDeviceAccess *) dwInstance;

    switch(uMsg){
    case MIM_OPEN:
        qDebug("MMOPEN");
        break;
    case MIM_CLOSE:
        qDebug("MIM_ClOSE");
        break;
    case MIM_DATA:
        qDebug("MIM_DATA");
        break;
    case MIM_LONGDATA:
        {
        //qDebug("MIM_LONGDATA");

        LPMIDIHDR lpMidiHdr = (LPMIDIHDR) dwParam;

        if(lpMidiHdr->dwBytesRecorded){

            //qDebug() << "sysEx len" << lpMidiHdr->dwBytesRecorded;
            //qDebug() << "data sys" << lpMidiHdr->lpData;

            mda->slotProcessSysEx(QByteArray(lpMidiHdr->lpData, lpMidiHdr->dwBytesRecorded));

            midiInAddBuffer(handle, lpMidiHdr, sizeof(MIDIHDR));
        }
       }

        break;
    default:
        qDebug("in callback");
        break;
    }
}

void CALLBACK midiOutCallback(HMIDIOUT handle, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam, DWORD_PTR dwParam1){
    qDebug() <<  "msg type" << uMsg << MOM_DONE << MOM_CLOSE << MOM_OPEN;

    MidiDeviceAccess *mda = (MidiDeviceAccess *) dwInstance;

    if(uMsg == MOM_DONE){
       midiOutUnprepareHeader(mda->outHandle, &mda->sysExOutHdr, sizeof(MIDIHDR));
       GlobalUnlock(mda->sysExOutBuffer);
       GlobalFree(mda->sysExOutBuffer);
    }
}

void Main_OnDeviceChange(HWND hwn, WPARAM wParam, LPARAM lParam){
    qDebug() << "device changed called";
}
