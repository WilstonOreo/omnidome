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
