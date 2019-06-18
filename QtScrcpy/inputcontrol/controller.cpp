#include <QCoreApplication>

#include "controller.h"
#include "videosocket.h"
#include "controlevent.h"

Controller::Controller(QObject* parent) : QObject(parent)
{

}

Controller::~Controller()
{

}

void Controller::setControlSocket(QTcpSocket* controlSocket)
{
    m_controlSocket = controlSocket;
}

void Controller::postControlEvent(ControlEvent *controlEvent)
{
    if (controlEvent) {
        QCoreApplication::postEvent(this, controlEvent);
    }
}

void Controller::test(QRect rc)
{
    ControlEvent* controlEvent = new ControlEvent(ControlEvent::CET_MOUSE);
    controlEvent->setMouseEventData(AMOTION_EVENT_ACTION_DOWN, AMOTION_EVENT_BUTTON_PRIMARY, rc);
    postControlEvent(controlEvent);
}

bool Controller::event(QEvent *event)
{
    if (event && event->type() == ControlEvent::Control) {
        ControlEvent* controlEvent = dynamic_cast<ControlEvent*>(event);
        if (controlEvent) {
            sendControl(controlEvent->serializeData());
        }
        return true;
    }
    return QObject::event(event);
}

bool Controller::sendControl(const QByteArray &buffer)
{
    if (buffer.isEmpty()) {
        return false;
    }
    qint32 len = 0;
    if (m_controlSocket) {
        len = m_controlSocket->write(buffer.data(), buffer.length());
    }
    return len == buffer.length() ? true : false;
}
