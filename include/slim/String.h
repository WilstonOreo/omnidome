/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
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

#ifndef SLIM_STRING_H_
#define SLIM_STRING_H_

#include <QLineEdit>
#include "AbstractInputWidget.h"

namespace slim
{
  class String :
    public AbstractInputWidget
  {
    Q_OBJECT
  public:
    typedef QLineEdit editor_type;

    String(QWidget* = nullptr);
    String(QString const& _label, QString const& _text, QWidget* = nullptr);
    ~String();

    QString value() const;

  public slots:
    void setValue(QString const&);
    void setEditorVisible(bool);
  signals:
    void valueChanged();

  protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);

  private:
    void init();
    bool moving_;
  };
}

#endif /* SLIM_STRING_H_ */
