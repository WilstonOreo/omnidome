#include <omni/ui/About.h>

#include "ui_About.h"

using namespace omni::ui;

About::About(QWidget* _parent) : 
  QDialog(_parent),
  ui_(new Ui::About())
{
  ui_->setupUi(this);
}

About::~About()
{
}

