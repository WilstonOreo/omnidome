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
       * @detail Similar to VBoxLayout, except that widgets with a PREVIEW Role 
       *         have a size with aspect ratio of associated tuning
       **/
      class TuningLayout: public QLayout
      {
        Q_OBJECT
      public:
        /// Role of a widget defines resize behaviour
        enum class Role {
          TITLE, // Fixed size
          PARAMETER, // Fixed size
          PREVIEW // Size with aspect ratio of tuning
        };
        
        typedef std::vector<std::pair<QWidget*,Role>> widgetgroup_type;

        TuningLayout(Tuning* parent);
        ~TuningLayout();

        /// Add item, with PARAMETER Role
        void addItem(QLayoutItem *item);

        /// Add widget with PARAMETER Role
        void addWidget(QWidget *widget);

        /// Add widget with Role 
        void addWidget(QWidget *widget, Role);

        /// Return number of widgets
        int count() const;

        /// Get index of given widget
        int indexOf(QWidget* widget) const; 
        
        /// Return item at index
        QLayoutItem* itemAt(int index) const;
        
        /// Remove widget at index
        QLayoutItem* takeAt(int index);

        /// Set geometry of all widgets
        void setGeometry(const QRect &rect);

        /// Clears layout and inserts the given widgets 
        void setWidgets(widgetgroup_type const& _widgets);

        /// Clear layout
        void clear();

        /// Return minimum size
        QSize minimumSize() const;
        
        /// Return size hint, is equal to minimum size
        QSize sizeHint() const;
        
      private:
        omni::proj::Tuning* tuning();
        omni::proj::Tuning const* tuning() const;

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

        /// Add a layout item with Role
        void add(QLayoutItem* _item, Role _role);

        enum SizeType { MINIMUMSIZE, SIZEHINT };
        QSize calculateSize(SizeType sizeType) const;

        std::vector<ItemWrapper> items_;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_TUNINGLAYOUT_H_ */
