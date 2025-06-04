/****************************************************************************
** Meta object code from reading C++ file 'radiocommande.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../radiocommande.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'radiocommande.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_Joystick_t {
    uint offsetsAndSizes[8];
    char stringdata0[9];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_Joystick_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_Joystick_t qt_meta_stringdata_Joystick = {
    {
        QT_MOC_LITERAL(0, 8),  // "Joystick"
        QT_MOC_LITERAL(9, 13),  // "lireEvenement"
        QT_MOC_LITERAL(23, 0),  // ""
        QT_MOC_LITERAL(24, 14)   // "envoyerRequete"
    },
    "Joystick",
    "lireEvenement",
    "",
    "envoyerRequete"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_Joystick[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   26,    2, 0x08,    1 /* Private */,
       3,    0,   27,    2, 0x08,    2 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject Joystick::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Joystick.offsetsAndSizes,
    qt_meta_data_Joystick,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_Joystick_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Joystick, std::true_type>,
        // method 'lireEvenement'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'envoyerRequete'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Joystick::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Joystick *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->lireEvenement(); break;
        case 1: _t->envoyerRequete(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *Joystick::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Joystick::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Joystick.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Joystick::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
