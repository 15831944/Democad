/****************************************************************************
** Meta object code from reading C++ file 'ErrorMessage.h'
**
** Created: Mon Sep 26 14:56:11 2016
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ErrorMessage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ErrorMessage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ErrorMessage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x09,
      30,   13,   13,   13, 0x09,
      45,   13,   13,   13, 0x09,
      69,   13,   13,   13, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_ErrorMessage[] = {
    "ErrorMessage\0\0btnYesClicked()\0"
    "btnNoClicked()\0btnThirdChooseClicked()\0"
    "btnFouthChooseClicked()\0"
};

void ErrorMessage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ErrorMessage *_t = static_cast<ErrorMessage *>(_o);
        switch (_id) {
        case 0: _t->btnYesClicked(); break;
        case 1: _t->btnNoClicked(); break;
        case 2: _t->btnThirdChooseClicked(); break;
        case 3: _t->btnFouthChooseClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData ErrorMessage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ErrorMessage::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ErrorMessage,
      qt_meta_data_ErrorMessage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ErrorMessage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ErrorMessage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ErrorMessage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ErrorMessage))
        return static_cast<void*>(const_cast< ErrorMessage*>(this));
    return QDialog::qt_metacast(_clname);
}

int ErrorMessage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
