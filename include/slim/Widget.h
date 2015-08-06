#ifndef SLIM_WIDGET_H_
#define SLIM_WIDGET_H_

#include <QWidget>
#include <QLabel>
#include <QPalette>
#include <slim/ColorSet.h>

namespace slim
{
  class Widget : public QWidget
  {
    Q_OBJECT
  public:
    Widget(QWidget* = nullptr);
    Widget(QString const& _label, QWidget* = nullptr);
    virtual ~Widget() {}

    /// Return label text of widget
    QString label() const;

    /// Set label text of widget
    void setLabel(QString const&);

    ColorSet const& colorSet() const;

    double zoomFactor() const;

  protected:
    void drawBorder(QPainter&);
    void drawBorder(QPainter&,bool _focus);
    QLabel* label_;

  private:
    void init();

    ColorSet colorSet_;
    double zoomFactor_ = 1.0;
  };
}

#endif /* SLIM_WIDGET_H_ */
