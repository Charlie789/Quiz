#ifndef CUSTOMTYPES_H
#define CUSTOMTYPES_H

#include <QMetaType>

namespace CustomTypes {

enum RequestType {
    RequestHello,
    RequestResponse,
    RequestTest,
    RequestCategory,
    RequestAddCategory,
    RequestQuestion,
    RequestAnswerForQuestion,
    RequestAddQuestionVariant
};

}
Q_DECLARE_METATYPE(CustomTypes::RequestType)

#endif // CUSTOMTYPES_H
