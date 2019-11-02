/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
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
#include <QDialog>
#include <QStandardItemModel>
#include <omni/ui/global.h>
#include <omni/exception.h>

namespace omni {
  namespace ui {
    namespace Ui {
      class ExceptionList;
    }

    /// A list of all exceptions occured during a process
    class OMNIWIDGET_EXPORT ExceptionList : public QDialog {
      Q_OBJECT

      public:
        ExceptionList(QWidget * = nullptr);
        ~ExceptionList();

        /// Add exception to list
        void addException(Exception const&);

        /// Number of exceptions in list
        int exceptionCount() const;

      public slots:
        /// Clear list of exceptions
        void clear();

      private:
        void setupModel();

        std::unique_ptr<Ui::ExceptionList> ui_;
        std::unique_ptr<QStandardItemModel> model_;
    };

    // Function body template for a try...catch block with executing exception list widget
    template<typename EXCEPTION, typename F>
    void tryWithExceptionList(F f) {
      std::unique_ptr<ExceptionList> _widget(new ExceptionList);
      try {
        f();
      } catch(EXCEPTION& _e) {
        _widget->addException(_e);
      }
      if (_widget->exceptionCount() > 0) {
        _widget->exec();
      }
    }
  }
}
