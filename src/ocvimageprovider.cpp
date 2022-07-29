#include "ocvimageprovider.h"

OCVImageProvider::OCVImageProvider(QObject *parent) : QObject(parent), QQuickImageProvider(QQuickImageProvider::Image)
{
    image = QImage(":/assets/help.png");
}


QImage OCVImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id);

    if(size){
        *size = image.size();
    }

    if(requestedSize.width() > 0 && requestedSize.height() > 0) {
        image = image.scaled(requestedSize.width(), requestedSize.height(), Qt::KeepAspectRatio);
    }
    return image;
}


void OCVImageProvider::updateImage(const QImage &image){
    if(!image.isNull() && this->image != image) {
        this->image = image;
        emit imageChanged();
    }
}

