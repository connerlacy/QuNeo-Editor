/****************************************************************************
** Meta object code from reading C++ file 'sysexformat.h'
**
** Created: Tue Aug 6 13:54:31 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "midi/sysexformat.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sysexformat.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SysExFormat[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   12,   12,   12, 0x0a,
      66,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SysExFormat[] = {
    "SysExFormat\0\0sigSysExByteArray(QByteArray*)\0"
    "slotEncodePreset(int)\0slotWriteFile(int)\0"
};

void SysExFormat::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SysExFormat *_t = static_cast<SysExFormat *>(_o);
        switch (_id) {
        case 0: _t->sigSysExByteArray((*reinterpret_cast< QByteArray*(*)>(_a[1]))); break;
        case 1: _t->slotEncodePreset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->slotWriteFile((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SysExFormat::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SysExFormat::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SysExFormat,
      qt_meta_data_SysExFormat, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SysExFormat::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SysExFormat::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SysExFormat::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SysExFormat))
        return static_cast<void*>(const_cast< SysExFormat*>(this));
    return QObject::qt_metacast(_clname);
}

int SysExFormat::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void SysExFormat::sigSysExByteArray(QByteArray * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
