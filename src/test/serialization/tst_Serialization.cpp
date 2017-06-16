/* Copyright (c) 2014-2017 "Omnidome" by Michael Winkelmann
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <memory>
#include <QtTest/QtTest>

#include "testobject.h"


class TestSerialization: public QObject
{
    Q_OBJECT
private slots:

    void testQObject();

    void testObjEqual();
};


void TestSerialization::testQObject() {
    std::unique_ptr<TestObject> testObject(new TestObject);

    testObject->setIntFoo(42);
    testObject->setRealFoo(23.0);
    testObject->objFoo()->setNestedInt(12);
    QVariant v = testObject->toVariant();

    testObject->setIntFoo(23);
    testObject->setRealFoo(22.0);
    testObject->objFoo()->setNestedInt(23);

    testObject->fromVariant(v);

    QCOMPARE(42,testObject->intFoo());
    QCOMPARE(23.0,testObject->realFoo());
    QCOMPARE(12,testObject->objFoo()->nestedInt());
}

void TestSerialization::testObjEqual() {

    std::unique_ptr<TestObject> a(new TestObject);
    std::unique_ptr<TestObject> b(new TestObject);

    auto setProperties = [&](TestObject* o) {
        o->setIntFoo(42);
        o->setRealFoo(23);
        o->objFoo()->setNestedInt(42);
    };
    setProperties(a.get());
    setProperties(b.get());

    QCOMPARE(a->equal(b.get()),true);

    a->setIntFoo(23);
    QCOMPARE(a->equal(b.get()),false);

    // Test copy
    b->copy(a.get());
    QCOMPARE(a->equal(b.get()),true);
}

QTEST_MAIN(TestSerialization)
#include "tst_Serialization.moc"
