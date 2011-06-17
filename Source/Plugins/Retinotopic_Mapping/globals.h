#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>
#include <QTextStream>

// globals
//extern bool DEBUG;
extern int my_random(int x);
extern void my_randomize();
extern QString combinePathAndFileName(const QString& path, const QString& filename);
extern bool readStringInQuotes(QString &token, QTextStream &ar);

#endif // GLOBALS_H
