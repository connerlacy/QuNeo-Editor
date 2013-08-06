/****************************************************************************
** Meta object code from reading C++ file 'padeditpane.h'
**
** Created: Tue Aug 6 13:54:30 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/editPanes/padeditpane.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'padeditpane.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PadEditPane[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      75,   13,   12,   12, 0x05,
     131,   12,   12,   12, 0x05,
     186,  155,   12,   12, 0x05,
     234,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     262,   12,   12,   12, 0x0a,
     295,   12,   12,   12, 0x0a,
     315,   12,   12,   12, 0x0a,
     337,   12,   12,   12, 0x0a,
     359,   12,   12,   12, 0x0a,
     383,   12,   12,   12, 0x0a,
     425,  405,   12,   12, 0x0a,
     459,   12,   12,   12, 0x0a,
     508,  492,   12,   12, 0x0a,
     559,  542,   12,   12, 0x0a,
     592,   12,   12,   12, 0x0a,
     623,   12,   12,   12, 0x0a,
     644,   12,   12,   12, 0x0a,
     665,   12,   12,   12, 0x0a,
     684,   12,   12,   12, 0x0a,
     703,   12,   12,   12, 0x0a,
     722,   12,   12,   12, 0x0a,
     739,   12,   12,   12, 0x0a,
     757,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PadEditPane[] = {
    "PadEditPane\0\0"
    "presetNum,settingsType,paramType,typeNum,paramName,paramValue\0"
    "signalValueChanged(int,QString,QString,int,QString,int)\0"
    "signalToLabels(QString)\0"
    "presetNum,tableName,tableValue\0"
    "signalTableChanged(int,QString,QList<QVariant>)\0"
    "signalSelectPreset(QString)\0"
    "slotSelectVelocityTable(QString)\0"
    "slotEvents(QString)\0slotRecallPreset(int)\0"
    "slotValueChanged(int)\0slotEnableGridMode(int)\0"
    "slotToLabels(QString)\0oldObject,newObject\0"
    "slotFocusParam(QWidget*,QWidget*)\0"
    "slotCalculatePadSensitivity(int)\0"
    "onThreshChanged\0slotCalculateOffThreshFromOn(int)\0"
    "offThreshChanged\0slotCheckOffThreshAgainstOn(int)\0"
    "slotLoadVariableTable(QString)\0"
    "slotChangeXText(int)\0slotChangeYText(int)\0"
    "slotUpdateXYText()\0slotStyleDefault()\0"
    "slotStyleMachine()\0slotStyleAkaMP()\0"
    "slotStyleKorPad()\0slotStyleFullLevel()\0"
};

void PadEditPane::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PadEditPane *_t = static_cast<PadEditPane *>(_o);
        switch (_id) {
        case 0: _t->signalValueChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 1: _t->signalToLabels((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->signalTableChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QList<QVariant>(*)>(_a[3]))); break;
        case 3: _t->signalSelectPreset((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->slotSelectVelocityTable((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->slotEvents((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->slotRecallPreset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->slotValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->slotEnableGridMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->slotToLabels((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->slotFocusParam((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 11: _t->slotCalculatePadSensitivity((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->slotCalculateOffThreshFromOn((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->slotCheckOffThreshAgainstOn((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->slotLoadVariableTable((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->slotChangeXText((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->slotChangeYText((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->slotUpdateXYText(); break;
        case 18: _t->slotStyleDefault(); break;
        case 19: _t->slotStyleMachine(); break;
        case 20: _t->slotStyleAkaMP(); break;
        case 21: _t->slotStyleKorPad(); break;
        case 22: _t->slotStyleFullLevel(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PadEditPane::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PadEditPane::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PadEditPane,
      qt_meta_data_PadEditPane, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PadEditPane::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PadEditPane::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PadEditPane::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PadEditPane))
        return static_cast<void*>(const_cast< PadEditPane*>(this));
    return QWidget::qt_metacast(_clname);
}

int PadEditPane::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    return _id;
}

// SIGNAL 0
void PadEditPane::signalValueChanged(int _t1, QString _t2, QString _t3, int _t4, QString _t5, int _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PadEditPane::signalToLabels(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PadEditPane::signalTableChanged(int _t1, QString _t2, QList<QVariant> _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void PadEditPane::signalSelectPreset(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
