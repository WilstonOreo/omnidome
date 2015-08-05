#include <omni/ui/Input.h>

#include "ui_omni_ui_Input.h"

#include <QMenu>
#include <QDebug>
#include <QVBoxLayout>
#include <QErrorMessage>
#include <QFileDialog>

#include <omni/Session.h>
#include <omni/input/Interface.h>
#include <omni/input/Image.h>
#include <omni/input/TestImage.h>

namespace omni
{
  namespace ui
  {
    Input::Input(QWidget* _parent) :
      QGroupBox(_parent),
      ui_(new Ui::Input)
    {
      ui_->setupUi(this);

      // Make popup menu for adding inputs
      {
        QMenu* _menu = new QMenu();
        ui_->btnAddInput->setMenu(_menu);
        for (auto& _idInputClass : omni::input::Factory::classes())
        {
          QString _id = _idInputClass.first.str();
          QAction* _menuItem = new QAction(QIcon(QString(":/input/")+ _id + QString(".png")),_id,_menu);
          _menu->addAction(_menuItem);
        }
        connect(_menu,SIGNAL(triggered(QAction*)),this,SLOT(addInput(QAction*))); 
      }

      // Update preview when input has changed
      connect(this,SIGNAL(inputChanged()),ui_->preview,SLOT(update()));
      
      // Input can be changed in preview (e.g. ruler position), so update views as well 
      connect(ui_->preview,SIGNAL(inputChanged()),this,SIGNAL(inputChanged()));

      prepareModel();
      ui_->widget->setLayout(new QVBoxLayout);
    }

    Input::~Input()
    {
    }

    Session const* Input::session() const
    {
      return session_;
    }

    void Input::setSession(Session* _session)
    {
      session_=_session;
      ui_->preview->setSession(_session);
      prepareModel();
      for (auto& _input : session_->inputs()) 
      {
        addItem(_input.get());
      }
    }
      
    void Input::inputSelected(QString const&)
    {

    }
 
    void Input::addInput(QAction* _action)
    {
      if (!session_) return;

      auto* _input = session_->inputs().add(_action->text());
      if (!_input) return;

      if (showSettingsDialog(_input))
      {
        addItem(_input);
        session_->inputs().setCurrentIndex(session_->inputs().size()-1);
        emit inputChanged();
      }

    }

    void Input::removeSelection() 
    { 
      int _row = ui_->inputList->currentIndex().row();
      if (_row < 0 || _row >= session_->inputs().size()) return;

      session_->inputs().remove(_row);
      model_->removeRows(_row,1);
      changeSelection(model_->index(_row-1,0));
      
      emit inputChanged();
    }

    void Input::clear()
    {
      session_->inputs().clear();
      prepareModel();
      emit inputChanged();
    }
      
    void Input::changeSelection(QModelIndex _index)
    {
      if (!session_) return;

      int _row = _index.row();

      if (_row < 0 || _row >= session_->inputs().size())
      {
        session_->inputs().setCurrentIndex(0);
        return;
      }
 
      session_->inputs().setCurrentIndex(_index.row());
      emit inputChanged();
    }

    void Input::prepareModel() 
    {
      model_.reset(new QStandardItemModel());
      model_->clear();
      model_->setColumnCount(2);
      model_->setHeaderData(0,Qt::Horizontal,"Input Type");
      model_->setHeaderData(1,Qt::Horizontal,"Info");
      ui_->inputList->setModel(model_.get());
    }
      
    bool Input::showSettingsDialog(input::Interface* _input)
    {
      if (_input->getTypeId() == "Image") 
      {
        QString _filename = QFileDialog::getOpenFileName(this, "Open image file...", ".", "Image files (*.png *.jpg *.jpeg)");
        if (_filename.isEmpty()) return false;

        try {
          static_cast<input::Image*>(_input)->load(_filename);
        } catch (...) {
          QErrorMessage _errorMessageBox(this);
          _errorMessageBox.showMessage("Error loading image.");
          return false;
        }

        return true;

      } else if (_input->getTypeId() == "TestImage") {
        return true;
      }

      return true;
    }
 
    void Input::addItem(input::Interface const* _input)
    {
      if (!_input) return;

      QList<QStandardItem*> _row;
      QStandardItem* _item = new QStandardItem(_input->getTypeId());
      _item->setEditable(false);
      _row << _item;
      _item = new QStandardItem(_input->infoText());
      _item->setEditable(false);
      _row << _item;

      model_->invisibleRootItem()->appendRow(_row);
      ui_->inputList->resizeColumnToContents(0);
      ui_->inputList->resizeColumnToContents(1);
    }
  }
}
