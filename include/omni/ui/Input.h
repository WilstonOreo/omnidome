#ifndef OMNI_UI_INPUT_H_
#define OMNI_UI_INPUT_H_

#include <memory>
#include <QGroupBox>
#include <QStandardItemModel>
#include <omni/input/Interface.h>

namespace omni
{
  class Session;
  
  namespace ui
  {
    namespace Ui
    {
      class Input;
    }

    /// Input List widget
    class Input : public QGroupBox 
    {
      Q_OBJECT
    public:
      Input(QWidget* = nullptr);
      ~Input();

      Session const* session() const;
      void setSession(Session* _session);

    public slots:
      void addInput(QAction*);

      /// Removes all selected inputs
      void removeSelection();

      /// Removes all inputs
      void clear();

      /// Select input at model index
      void changeSelection(QModelIndex);

    signals:
      void inputChanged();

    private:
      /// Setup the item model with columns etc
      void prepareModel();

      bool showSettingsDialog(input::Interface*);

      /// Add an item to input list widget
      void addItem(input::Interface const*);
      
      Session* session_ = nullptr;
      std::unique_ptr<Ui::Input> ui_;
      std::unique_ptr<QStandardItemModel> model_;
    };
  }
}


#endif /* OMNI_UI_INPUT_H_ */
