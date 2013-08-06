/****************************************************************************
** Meta object code from reading C++ file 'midideviceaccess.h'
**
** Created: Tue Aug 6 13:54:31 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "midi/rtmidi/midideviceaccess.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'midideviceaccess.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MidiDeviceAccess[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,
      36,   17,   17,   17, 0x05,
      68,   17,   17,   17, 0x05,
      93,   17,   17,   17, 0x05,
     113,   17,   17,   17, 0x05,
     145,  143,   17,   17, 0x05,
     177,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     201,   17,   17,   17, 0x0a,
     231,   17,   17,   17, 0x0a,
     257,   17,   17,   17, 0x0a,
     280,   17,   17,   17, 0x0a,
     305,   17,   17,   17, 0x0a,
     326,   17,   17,   17, 0x0a,
     353,   17,   17,   17, 0x0a,
     377,   17,   17,   17, 0x0a,
     394,   17,   17,   17, 0x0a,
     411,   17,   17,   17, 0x0a,
     426,   17,   17,   17, 0x0a,
     444,   17,   17,   17, 0x0a,
     480,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MidiDeviceAccess[] = {
    "MidiDeviceAccess\0\0clearDeviceMenu()\0"
    "populateDeviceMenu(QStringList)\0"
    "sigFirmwareCurrent(bool)\0sigFwBytesLeft(int)\0"
    "sigUpdateAllPresetsCount(int)\0,\0"
    "sigSetVersions(QString,QString)\0"
    "sigQuNeoConnected(bool)\0"
    "slotSetCurrentPreset(QString)\0"
    "slotSelectDevice(QString)\0"
    "slotUpdateAllPresets()\0slotUpdateSinglePreset()\0"
    "slotUpdateFirmware()\0slotCheckFirmwareVersion()\0"
    "slotProcessSysExRx(int)\0slotDownloadFw()\0"
    "slotLoadPreset()\0slotSwapLeds()\0"
    "getSourcesDests()\0slotSendToggleProgramChangeOutput()\0"
    "slotSendToggleProgramChangeInput()\0"
};

void MidiDeviceAccess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MidiDeviceAccess *_t = static_cast<MidiDeviceAccess *>(_o);
        switch (_id) {
        case 0: _t->clearDeviceMenu(); break;
        case 1: _t->populateDeviceMenu((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 2: _t->sigFirmwareCurrent((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->sigFwBytesLeft((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->sigUpdateAllPresetsCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->sigSetVersions((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->sigQuNeoConnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->slotSetCurrentPreset((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->slotSelectDevice((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->slotUpdateAllPresets(); break;
        case 10: _t->slotUpdateSinglePreset(); break;
        case 11: _t->slotUpdateFirmware(); break;
        case 12: _t->slotCheckFirmwareVersion(); break;
        case 13: _t->slotProcessSysExRx((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->slotDownloadFw(); break;
        case 15: _t->slotLoadPreset(); break;
        case 16: _t->slotSwapLeds(); break;
        case 17: _t->getSourcesDests(); break;
        case 18: _t->slotSendToggleProgramChangeOutput(); break;
        case 19: _t->slotSendToggleProgramChangeInput(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MidiDeviceAccess::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MidiDeviceAccess::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MidiDeviceAccess,
      qt_meta_data_MidiDeviceAccess, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MidiDeviceAccess::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MidiDeviceAccess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MidiDeviceAccess::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MidiDeviceAccess))
        return static_cast<void*>(const_cast< MidiDeviceAccess*>(this));
    return QObject::qt_metacast(_clname);
}

int MidiDeviceAccess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void MidiDeviceAccess::clearDeviceMenu()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void MidiDeviceAccess::populateDeviceMenu(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MidiDeviceAccess::sigFirmwareCurrent(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MidiDeviceAccess::sigFwBytesLeft(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MidiDeviceAccess::sigUpdateAllPresetsCount(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MidiDeviceAccess::sigSetVersions(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MidiDeviceAccess::sigQuNeoConnected(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
