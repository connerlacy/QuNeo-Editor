/****************************************************************************
** Meta object code from reading C++ file 'hsliderbutton.h'
**
** Created: Tue Aug 6 13:54:30 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/buttons/hsliderbutton.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hsliderbutton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_HSliderButton[] = {

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
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   14,   14,   14, 0x0a,
      72,   64,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_HSliderButton[] = {
    "HSliderButton\0\0signalCurrentSensor(QString)\0"
    "slotEvents(QString)\0old,now\0"
    "slotCheckToConnectFocus(QWidget*,QWidget*)\0"
};

void HSliderButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HSliderButton *_t = static_cast<HSliderButton *>(_o);
        switch (_id) {
        case 0: _t->signalCurrentSensor((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->slotEvents((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->slotCheckToConnectFocus((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData HSliderButton::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HSliderButton::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HSliderButton,
      qt_meta_data_HSliderButton, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HSliderButton::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HSliderButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HSliderButton::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HSliderButton))
        return static_cast<void*>(const_cast< HSliderButton*>(this));
    return QObject::qt_metacast(_clname);
}

int HSliderButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void HSliderButton::signalCurrentSensor(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
