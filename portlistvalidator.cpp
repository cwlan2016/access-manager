#include "portlistvalidator.h"

#include "converters.h"

PortListValidator::PortListValidator(int countPorts) :
    mCountPorts(countPorts)
{
}

QValidator::State PortListValidator::validate(QString &input, int &pos) const
{
    if (input.isEmpty())
        return Intermediate;

    if (!input[0].isDigit())
        return Invalid;

    if (input[pos - 1] == '-' || input[pos - 1] == ',') {
        if (input[pos - 2] == '-' || input[pos - 2] == ',') {
            return Invalid;
        } else if (input[pos - 1] == '-') {
            int posComma = input.lastIndexOf(",");
            int posDash = input.lastIndexOf(QRegExp("-[0-9]"));

            if (((posComma == -1) && (posDash != -1))
                    || (posDash > posComma))
                return Invalid;
        }

        return Intermediate;
    }

    if (!input[pos - 1].isDigit())
        return Invalid;

    QStringList rangeStringList = input.split(QRegExp(","), QString::SkipEmptyParts);

    auto it = rangeStringList.constBegin();
    auto end = rangeStringList.constEnd();
    for (; it != end; ++it) {
        QStringList valueStringList = (*it).split('-', QString::SkipEmptyParts);

        if (valueStringList.size() == 2) {
            if ((valueStringList.at(0).toInt() > mCountPorts)
                    || (valueStringList.at(1).toInt() > mCountPorts))
                return Invalid;

            if (valueStringList.at(1).toInt() < valueStringList.at(0).toInt())
                return Intermediate;
        } else {
            if (valueStringList.at(0).toInt() > mCountPorts)
                return Invalid;
        }
    }

    return Acceptable;
}
