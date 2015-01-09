/****************************************************************************
** Meta object code from reading C++ file 'clamp-protocol-editor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "clamp-protocol-editor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clamp-protocol-editor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ClampProtocolEditor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,   20,   43,   20, 0x0a,
      66,   20,   20,   20, 0x0a,
      88,   20,   20,   20, 0x0a,
     104,   20,   20,   20, 0x0a,
     121,   20,   20,   20, 0x0a,
     141,  139,   20,   20, 0x0a,
     179,   20,   20,   20, 0x0a,
     218,   20,   20,   20, 0x08,
     231,   20,   20,   20, 0x08,
     247,   20,   20,   20, 0x08,
     257,   20,   20,   20, 0x08,
     270,   20,   20,   20, 0x08,
     283,   20,   20,   20, 0x08,
     315,   20,   20,   20, 0x08,
     340,   20,   20,   20, 0x08,
     359,   20,   20,   20, 0x08,
     373,  139,   20,   20, 0x08,
     402,  139,   20,   20, 0x08,
     447,   20,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ClampProtocolEditor[] = {
    "ClampProtocolEditor\0\0protocolTableScroll()\0"
    "QString\0loadProtocol()\0loadProtocol(QString)\0"
    "clearProtocol()\0exportProtocol()\0"
    "previewProtocol()\0,\0"
    "protocolTable_currentChanged(int,int)\0"
    "protocolTable_verticalSliderReleased()\0"
    "addSegment()\0deleteSegment()\0addStep()\0"
    "insertStep()\0deleteStep()\0"
    "updateSegment(QListWidgetItem*)\0"
    "updateSegmentSweeps(int)\0updateTableLabel()\0"
    "updateTable()\0updateStepAttribute(int,int)\0"
    "updateStepType(int,ProtocolStep::stepType_t)\0"
    "saveProtocol()\0"
};

void ClampProtocolEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ClampProtocolEditor *_t = static_cast<ClampProtocolEditor *>(_o);
        switch (_id) {
        case 0: _t->protocolTableScroll(); break;
        case 1: { QString _r = _t->loadProtocol();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 2: _t->loadProtocol((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->clearProtocol(); break;
        case 4: _t->exportProtocol(); break;
        case 5: _t->previewProtocol(); break;
        case 6: _t->protocolTable_currentChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->protocolTable_verticalSliderReleased(); break;
        case 8: _t->addSegment(); break;
        case 9: _t->deleteSegment(); break;
        case 10: _t->addStep(); break;
        case 11: _t->insertStep(); break;
        case 12: _t->deleteStep(); break;
        case 13: _t->updateSegment((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 14: _t->updateSegmentSweeps((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->updateTableLabel(); break;
        case 16: _t->updateTable(); break;
        case 17: _t->updateStepAttribute((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->updateStepType((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< ProtocolStep::stepType_t(*)>(_a[2]))); break;
        case 19: _t->saveProtocol(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ClampProtocolEditor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ClampProtocolEditor::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ClampProtocolEditor,
      qt_meta_data_ClampProtocolEditor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ClampProtocolEditor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ClampProtocolEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ClampProtocolEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ClampProtocolEditor))
        return static_cast<void*>(const_cast< ClampProtocolEditor*>(this));
    return QWidget::qt_metacast(_clname);
}

int ClampProtocolEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void ClampProtocolEditor::protocolTableScroll()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
