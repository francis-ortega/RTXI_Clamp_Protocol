/****************************************************************************
** Meta object code from reading C++ file 'clamp-protocol.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "clamp-protocol.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clamp-protocol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ClampProtocol[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      34,   14,   14,   14, 0x0a,
      55,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ClampProtocol[] = {
    "ClampProtocol\0\0loadProtocolFile()\0"
    "openProtocolEditor()\0openProtocolViewer()\0"
};

void ClampProtocol::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ClampProtocol *_t = static_cast<ClampProtocol *>(_o);
        switch (_id) {
        case 0: _t->loadProtocolFile(); break;
        case 1: _t->openProtocolEditor(); break;
        case 2: _t->openProtocolViewer(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData ClampProtocol::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ClampProtocol::staticMetaObject = {
    { &DefaultGUIModel::staticMetaObject, qt_meta_stringdata_ClampProtocol,
      qt_meta_data_ClampProtocol, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ClampProtocol::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ClampProtocol::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ClampProtocol::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ClampProtocol))
        return static_cast<void*>(const_cast< ClampProtocol*>(this));
    return DefaultGUIModel::qt_metacast(_clname);
}

int ClampProtocol::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DefaultGUIModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
