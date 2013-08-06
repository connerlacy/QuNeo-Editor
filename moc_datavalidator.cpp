/****************************************************************************
** Meta object code from reading C++ file 'datavalidator.h'
**
** Created: Tue Aug 6 13:54:31 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "data/datavalidator.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'datavalidator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DataValidator[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      45,   14,   14,   14, 0x0a,
      60,   14,   14,   14, 0x0a,
      76,   14,   14,   14, 0x0a,
      92,   14,   14,   14, 0x0a,
     119,   14,  107,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DataValidator[] = {
    "DataValidator\0\0slotConstructDefultParamMap()\0"
    "slotLoadJSON()\0slotParseJSON()\0"
    "slotCheckJSON()\0slotSaveJSON()\0"
    "QVariantMap\0slotValidatePreset(QVariantMap)\0"
};

void DataValidator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DataValidator *_t = static_cast<DataValidator *>(_o);
        switch (_id) {
        case 0: _t->slotConstructDefultParamMap(); break;
        case 1: _t->slotLoadJSON(); break;
        case 2: _t->slotParseJSON(); break;
        case 3: _t->slotCheckJSON(); break;
        case 4: _t->slotSaveJSON(); break;
        case 5: { QVariantMap _r = _t->slotValidatePreset((*reinterpret_cast< QVariantMap(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DataValidator::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DataValidator::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DataValidator,
      qt_meta_data_DataValidator, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DataValidator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DataValidator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DataValidator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataValidator))
        return static_cast<void*>(const_cast< DataValidator*>(this));
    return QWidget::qt_metacast(_clname);
}

int DataValidator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
