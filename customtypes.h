#ifndef CUSTOMTYPES_H
#define CUSTOMTYPES_H

#include <QMetaType>

namespace CustomTypes {

enum RequestType {
    RequestHello,
    RequestResponse,
    RequestTest,
    RequestCategory
};

}
Q_DECLARE_METATYPE(CustomTypes::RequestType)

#endif // CUSTOMTYPES_H
