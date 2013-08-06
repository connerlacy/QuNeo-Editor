/****************************************************************************
** Meta object code from reading C++ file 'presethandler.h'
**
** Created: Tue Aug 6 13:54:30 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "data/presethandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'presethandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PresetHandler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      42,   14,   14,   14, 0x0a,
      53,   14,   14,   14, 0x0a,
      74,   14,   14,   14, 0x0a,
     155,   93,   14,   14, 0x0a,
     209,   93,   14,   14, 0x0a,
     280,   14,   14,   14, 0x0a,
     311,   14,   14,   14, 0x0a,
     337,   14,   14,   14, 0x0a,
     356,   14,   14,   14, 0x0a,
     394,  376,   14,   14, 0x0a,
     451,  438,   14,   14, 0x0a,
     527,  481,   14,   14, 0x0a,
     584,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PresetHandler[] = {
    "PresetHandler\0\0signalPresetModified(bool)\0"
    "slotSave()\0slotSaveAllPresets()\0"
    "slotCheckPresets()\0"
    "presetNum,settingsType,paramType,typeNum,paramName,paramValue\0"
    "slotValueChanged(int,QString,QString,int,QString,int)\0"
    "slotPropogateValuesSelectMultiple(int,QString,QString,int,QString,int)\0"
    "slotPresetNameChanged(QString)\0"
    "slotRecallPreset(QString)\0slotRevertPreset()\0"
    "slotEvents(QString)\0connectDisconnect\0"
    "slotConnectDisconnectDisplayAllLabels(bool)\0"
    "sourceObject\0slotDisplayAllLabels(QString)\0"
    "presetNum,selectedTableID,selectedTableValues\0"
    "slotPadVelocityTableChanged(int,QString,QList<QVariant>)\0"
    "slotPresetNameValidator()\0"
};

void PresetHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PresetHandler *_t = static_cast<PresetHandler *>(_o);
        switch (_id) {
        case 0: _t->signalPresetModified((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->slotSave(); break;
        case 2: _t->slotSaveAllPresets(); break;
        case 3: _t->slotCheckPresets(); break;
        case 4: _t->slotValueChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 5: _t->slotPropogateValuesSelectMultiple((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 6: _t->slotPresetNameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->slotRecallPreset((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->slotRevertPreset(); break;
        case 9: _t->slotEvents((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->slotConnectDisconnectDisplayAllLabels((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->slotDisplayAllLabels((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->slotPadVelocityTableChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QList<QVariant>(*)>(_a[3]))); break;
        case 13: _t->slotPresetNameValidator(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PresetHandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PresetHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PresetHandler,
      qt_meta_data_PresetHandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PresetHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PresetHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PresetHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PresetHandler))
        return static_cast<void*>(const_cast< PresetHandler*>(this));
    return QObject::qt_metacast(_clname);
}

int PresetHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void PresetHandler::signalPresetModified(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
