#ifndef OMNI_UI_SCREENLIST_H_
#define OMNI_UI_SCREENLIST_H_

#include <QTreeView>
#include <QStandardItemModel>
#include <omni/util.hpp>
#include <tbd/property.h>
#include "Fullscreen.h"
#include "ScreenIdentifier.h"

namespace omni
{
  namespace ui
  {
    struct ScreenList : public QTreeView
    {
      Q_OBJECT
    public:
      ScreenList(QWidget* _parent = nullptr);
      ~ScreenList();

      /// Returns fullscreen widget of screen with id
      Fullscreen* fullscreen(size_t _id);
      Screen const* screen(size_t _id) const;

      /// Number of screens
      size_t count();

      TBD_PROPERTY_REF_RO(std::vector<proj::Screen>,screens)
    public slots:
      void updateScreens();
      void identify(bool);

    protected:

      void closeEvent(QCloseEvent* _event);

    private:
      template<class ITEM, class COLUMNS>
      void addItem(ITEM* _root, const COLUMNS& _columns)
      {
        QList<QStandardItem*> _row;
        for (auto& _item : _columns) 
        {
          _item->setEditable(false);
          _row << _item;
        }
        _root->appendRow(_row);
      }

      void clear();

      void prepareModel();
      static size_t numberSubScreens(const QRect& _screen);
      static std::pair<size_t,size_t> const* subscreenResolution(const QRect& _screen);

      std::vector<QtUniquePtr<Fullscreen>> fullscreenWidgets_;
      std::unique_ptr<QStandardItemModel> model_;
    };
  }
}

#endif /* OMNI_UI_SCREENLIST_H_ */
