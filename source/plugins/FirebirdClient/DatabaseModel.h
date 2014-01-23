//FirebirdClient
//Copyright (C) 2014  Sven Gijsen
//
//This file is part of StimulGL.
//StimulGL is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


#ifndef _DATABASE_MODEL_H_
#define _DATABASE_MODEL_H_

#include "FireBirdDatabase.h"

namespace Model
{ 
    class DatabaseModel : public QObject
    {
        Q_OBJECT

    public:
        DatabaseModel();
        ~DatabaseModel();

    public:
        void Open();
        bool Create();
        void AddJob(const QString& jobName );
        void SelectJobs(QStringList& jobs );

    private:
        void Close();
        QString GetFullPath(const QString& path);

    private:
        FireBirdDatabase fireBirdDatabase_;
        static QString userName_;
        static QString password_;
        static QString resourceString_;

        Q_DISABLE_COPY(DatabaseModel);
    };
}

#endif // _DATABASE_MODEL_H_
