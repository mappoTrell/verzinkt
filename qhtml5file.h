#ifndef QHTML5FILE_H
#define QHTML5FILE_H

#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <functional>
QT_BEGIN_NAMESPACE
namespace QHtml5File {
void load(const QString &accept, std::function<void(const QByteArray &, const QString &)> fileDataReady);
void save(const QByteArray &contents,const  QString &fileNameHint);
}
QT_END_NAMESPACE

#endif // QHTML5FILE_H
