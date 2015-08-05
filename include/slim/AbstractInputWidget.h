#ifndef SLIM_ABSTRACTINPUTWIDGET_H_
#define SLIM_ABSTRACTINPUTWIDGET_H_

#include <QLayout>
#include "Widget.h"

namespace slim
{
  class AbstractInputWidget : public Widget
  {
    Q_OBJECT
  public:
    AbstractInputWidget(QWidget* = nullptr);

  public slots:
    virtual void setEditorVisible(bool);
    void showEditor();
    void hideEditor();

  signals:
    void valueChanged();
  protected:
    virtual void mouseDoubleClickEvent(QMouseEvent*);
    virtual void mousePressEvent(QMouseEvent*);
    virtual void keyPressEvent(QKeyEvent*);


    void drawHandle(QPainter& _p, double _pos, double _heightFactor = 1.0) const;
    
    void drawTrack(QPainter& _p, double _pos) const;

    void drawTrack(QPainter& _p, double _left, double _right) const;

  protected:
    void init();

    template<typename T>
    T* createEditor()
    {
      if (editor_) return nullptr;

      editor_ = new T(this);
      T* _e = editorAs<T>();
      _e->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);
      _e->setStyleSheet("background : transparent");
      return _e;
    }

    template<typename T>
    T* editorAs()
    {
      return static_cast<T*>(editor_);
    }

    template<typename T>
    T const* editorAs() const
    {
      return static_cast<T const*>(editor_);
    }

    QWidget* editor_;
    QLabel* valueLabel_;
  };
}

#endif /* SLIM_ABSTRACTINPUTWIDGET_H_ */
