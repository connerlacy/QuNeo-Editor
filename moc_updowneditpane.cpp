/****************************************************************************
** Meta object code from reading C++ file 'updowneditpane.h'
**
** Created: Tue Aug 6 13:54:31 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/editPanes/updowneditpane.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'updowneditpane.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UpdownEditPane[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      78,   16,   15,   15, 0x05,
     134,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     158,   15,   15,   15, 0x0a,
     178,   15,   15,   15, 0x0a,
     200,   15,   15,   15, 0x0a,
     222,   15,   15,   15, 0x0a,
     238,   15,   15,   15, 0x0a,
     259,   15,   15,   15, 0x0a,
     301,  281,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_UpdownEditPane[] = {
    "UpdownEditPane\0\0"
    "presetNum,settingsType,paramType,typeNum,paramName,paramValue\0"
    "signalValueChanged(int,QString,QString,int,QString,int)\0"
    "signalToLabels(QString)\0slotEvents(QString)\0"
    "slotRecallPreset(int)\0slotValueChanged(int)\0"
    "slotEnable(int)\0slotBankControl(int)\0"
    "slotToLabels(QString)\0oldObject,nowObject\0"
    "slotFocusParam(QWidget*,QWidget*)\0"
};

void UpdownEditPane::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UpdownEditPane *_t = static_cast<UpdownEditPane *>(_o);
        switch (_id) {
        case 0: _t->signalValueChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 1: _t->signalToLabels((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->slotEvents((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->slotRecallPreset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->slotValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->slotEnable((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->slotBankControl((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->slotToLabels((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->slotFocusParam((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UpdownEditPane::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UpdownEditPane::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UpdownEditPane,
      qt_meta_data_UpdownEditPane, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UpdownEditPane::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UpdownEditPane::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UpdownEditPane::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UpdownEditPane))
        return static_cast<void*>(const_cast< UpdownEditPane*>(this));
    return QWidget::qt_metacast(_clname);
}

int UpdownEditPane::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void UpdownEditPane::signalValueChanged(int _t1, QString _t2, QString _t3, int _t4, QString _t5, int _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UpdownEditPane::signalToLabels(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
