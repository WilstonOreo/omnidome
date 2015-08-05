#ifndef OMNI_UI_PROJ_TUNINGLAYOUT_H_
#define OMNI_UI_PROJ_TUNINGLAYOUT_H_

#include <QLayout>
#include <omni/proj/Tuning.h>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      class Tuning;

      /**@brief Tuning Layout 
       **/
      class TuningLayout: public QLayout
      {
        Q_OBJECT
      public:
        enum class Role {
          TITLE,
          PREVIEW,
          PARAMETER
        };

        TuningLayout(Tuning* parent);
        ~TuningLayout();

        void addItem(QLayoutItem *item);
        void addWidget(QWidget *widget);
        void addWidget(QWidget *widget, Role = Role::PARAMETER);

        int count() const;

        int indexOf(QWidget* widget) const; 
        
        QLayoutItem* itemAt(int index) const;
        QLayoutItem* takeAt(int index);

        void setGeometry(const QRect &rect);

        /// Sets the name of the active group in the layout
        void setGroup(QString const& _groupName);

        /// Adds a new parameter group
        void addGroup(QString const& _groupName, std::vector<QWidget*> const& _widgets);
        void removeGroup(QString const& _groupName);

        QSize minimumSize() const;
        QSize sizeHint() const;
        
      private:
        omni::proj::Tuning* tuning();
        omni::proj::Tuning const* tuning() const;
        QString group_;
        std::map<QString,std::vector<QWidget*>> groups_;

        struct ItemWrapper 
        {
          ItemWrapper(QLayoutItem* _item = nullptr, Role _role = Role::PARAMETER) : 
            item_(_item),
            role_(_role) 
          {
          }

          QLayoutItem* item_;
          Role role_;

          QWidget* widget() const 
          {
            return item_->widget();
          }
        };

        std::vector<ItemWrapper> getGroupItems() const;
        
        void add(QLayoutItem* _item, Role _role);

        enum SizeType { MINIMUMSIZE, SIZEHINT };
        QSize calculateSize(SizeType sizeType) const;

        std::vector<ItemWrapper> items_;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_TUNINGLAYOUT_H_ */
