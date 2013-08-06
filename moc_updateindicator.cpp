/****************************************************************************
** Meta object code from reading C++ file 'updateindicator.h'
**
** Created: Tue Aug 6 13:54:31 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/labels/updateindicator.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'updateindicator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UpdateIndicator[] = {

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
      35,   22,   17,   16, 0x0a,
      56,   16,   17,   16, 0x0a,
      92,   85,   17,   16, 0x0a,
     117,   16,   16,   16, 0x0a,
     142,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_UpdateIndicator[] = {
    "UpdateIndicator\0\0bool\0presetNumber\0"
    "slotCompareMaps(int)\0slotCheckForUnsavedPresets()\0"
    "preset\0slotCheckThisPreset(int)\0"
    "slotPresetModified(bool)\0slotFlasher()\0"
};

void UpdateIndicator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UpdateIndicator *_t = static_cast<UpdateIndicator *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->slotCompareMaps((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { bool _r = _t->slotCheckForUnsavedPresets();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = _t->slotCheckThisPreset((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: _t->slotPresetModified((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->slotFlasher(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UpdateIndicator::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UpdateIndicator::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UpdateIndicator,
      qt_meta_data_UpdateIndicator, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UpdateIndicator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UpdateIndicator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UpdateIndicator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UpdateIndicator))
        return static_cast<void*>(const_cast< UpdateIndicator*>(this));
    return QWidget::qt_metacast(_clname);
}

int UpdateIndicator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
