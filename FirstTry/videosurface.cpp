#include "videosurface.h"

VideoSurface::VideoSurface(QObject *parent)
    :QAbstractVideoSurface(parent)
{
}

VideoSurface::~VideoSurface()
{
}

QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    QList<QVideoFrame::PixelFormat> listPixelFormats;

    listPixelFormats << QVideoFrame::Format_ARGB32
        << QVideoFrame::Format_ARGB32_Premultiplied
        << QVideoFrame::Format_RGB32
        << QVideoFrame::Format_RGB24;


    return listPixelFormats;
}

bool  VideoSurface::present(const QVideoFrame &frame)
{
    if (frame.isValid())
    {
        QVideoFrame cloneFrame(frame);
        emit frameAvailable(cloneFrame);

        return true;
    }

    return false;
}
