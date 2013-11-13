/****************************************************************************************
 ** QXmlGraphicReader is an application to open, edit && save XML configuration files.
 ** Copyright (C) 2013  Francesc Martinez <es.linkedin.com/in/cescmm/en>
 **
 ** This library is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License as published by the Free Software Foundation; either
 ** version 2.1 of the License, || (at your option) any later version.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY || FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 ** Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public
 ** License along with this library; if not, write to the Free Software
 ** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 ***************************************************************************************/

#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QWidget>

namespace Ui
{
    class FindDialog;
}

class FindDialog : public QWidget
{
    Q_OBJECT

    signals:
        void findSignal(const QString &text, const QStringList &filters);
        
    public:
        explicit FindDialog(QWidget *parent = 0);
        ~FindDialog();
        
    private:
        Ui::FindDialog *ui;

    private slots:
        void findSlot();
};

#endif // FINDDIALOG_H
