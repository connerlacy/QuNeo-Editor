/****************************************************************************
** Meta object code from reading C++ file 'padlabels.h'
**
** Created: Tue Aug 6 13:54:31 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/labels/padlabels.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'padlabels.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PadLabels[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      42,   11,   10,   10, 0x0a,
      91,   80,   10,   10, 0x0a,
     116,   10,   10,   10, 0x0a,
     137,   80,   10,   10, 0x0a,
     162,   11,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PadLabels[] = {
    "PadLabels\0\0objectName,componentNum,preset\0"
    "slotDisplayParameter(QString,int,int)\0"
    "objectName\0slotDisplayGrid(QString)\0"
    "slotDisplayChannel()\0slotDisplayDrum(QString)\0"
    "slotDisplayTranspose(QString,int,int)\0"
};

void PadLabels::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PadLabels *_t = static_cast<PadLabels *>(_o);
        switch (_id) {
        case 0: _t->slotDisplayParameter((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->slotDisplayGrid((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->slotDisplayChannel(); break;
        case 3: _t->slotDisplayDrum((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->slotDisplayTranspose((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PadLabels::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PadLabels::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PadLabels,
      qt_meta_data_PadLabels, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PadLabels::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PadLabels::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PadLabels::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PadLabels))
        return static_cast<void*>(const_cast< PadLabels*>(this));
    return QWidget::qt_metacast(_clname);
}

int PadLabels::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
