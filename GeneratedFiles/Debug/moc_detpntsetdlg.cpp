/****************************************************************************
** Meta object code from reading C++ file 'detpntsetdlg.h'
**
** Created: Wed Oct 26 09:12:42 2016
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../detpntsetdlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'detpntsetdlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DetPntSetDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      38,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DetPntSetDlg[] = {
    "DetPntSetDlg\0\0slotHasArcChecked(bool)\0"
    "slotUploadAllIndex()\0"
};

void DetPntSetDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DetPntSetDlg *_t = static_cast<DetPntSetDlg *>(_o);
        switch (_id) {
        case 0: _t->slotHasArcChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->slotUploadAllIndex(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DetPntSetDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DetPntSetDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DetPntSetDlg,
      qt_meta_data_DetPntSetDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DetPntSetDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DetPntSetDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DetPntSetDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DetPntSetDlg))
        return static_cast<void*>(const_cast< DetPntSetDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int DetPntSetDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
