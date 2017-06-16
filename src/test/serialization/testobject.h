#ifndef TESTOBJECT_H
#define TESTOBJECT_H

#include <qglobal.h>
#include <omni/Serializer.h>
#include <omni/property.h>

class NestedObject : public QObject, public omni::Serializer<NestedObject> {
    Q_OBJECT
    OMNI_PROPERTY_RW_DEFAULT(int,nestedInt,setNestedInt,0)
public:
    NestedObject(QObject* parent = nullptr) : QObject(parent) {
    }
    ~NestedObject() {
    }

signals:
    void nestedIntChanged();
};

class TestObject : public QObject, public omni::Serializer<TestObject> {
    Q_OBJECT
    OMNI_PROPERTY_RW_DEFAULT(int,intFoo,setIntFoo,0)
    OMNI_PROPERTY_RW_DEFAULT(qreal,realFoo,setRealFoo,0)
    OMNI_PROPERTY_OBJ(NestedObject,objFoo)
public:
    TestObject(QObject* parent = nullptr) :
        QObject(parent),
        objFoo_(new NestedObject(this)) {
    }
    ~TestObject() {
    }

signals:
    void intFooChanged();
    void realFooChanged();
};


#endif // TESTOBJECT_H
