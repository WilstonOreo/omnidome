#pragma once

#include <QPointer>
#include <QWindow>
#include <QOpenGLFunctions>

namespace omni {

class Session;

class ProjectionWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    ProjectionWindow(Session* session, QWindow* parent = nullptr);

    void render();

    Session* session() const;
    void set_session(Session* session);

    void initialize();

public slots:

    void render_later();
    void render_now();

protected:
    bool event(QEvent *event) override;

    void exposeEvent(QExposeEvent *event) override;

private:
    QOpenGLContext *context_ = nullptr;
    Session* session_;
};

}
