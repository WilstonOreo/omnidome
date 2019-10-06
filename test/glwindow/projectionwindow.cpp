#include "projectionwindow.h"

#include <omni/Session.h>

namespace omni {

ProjectionWindow::ProjectionWindow(
        Session* session, QWindow* parent)
{
    setSurfaceType(QWindow::OpenGLSurface);
    set_session(session);

}

void ProjectionWindow::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

}

Session *ProjectionWindow::session() const
{
    return session_;
}

void ProjectionWindow::set_session(Session *session)
{
   // session_ = QPointer<Session>(session);
}

void ProjectionWindow::initialize()
{

}

bool ProjectionWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        render_now();
        return true;
    default:
        return QWindow::event(event);
    }
}

void ProjectionWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);
    if (isExposed())
        render_now();
}

void ProjectionWindow::render_later() {
    requestUpdate();
}

void ProjectionWindow::render_now() {

    if (!isExposed())
        return;

    bool needsInitialize = false;

    if (!context_) {
        context_ = new QOpenGLContext(this);
        context_->setFormat(requestedFormat());
        context_->create();
        needsInitialize = true;
    }

    context_->makeCurrent(this);

    if (needsInitialize) {
        initializeOpenGLFunctions();
        initialize();
    }

    render();

    context_->swapBuffers(this);

    render_later();
}


}
