//ExperimentManagerplugin
//Copyright (C) 2013  Sven Gijsen
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

#ifndef PARAMETERPROPERTYEXTENSIONS_H
#define PARAMETERPROPERTYEXTENSIONS_H

#include "qtvariantproperty.h"
#include <QFrame>
#include <QComboBox>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QPushButton>

#define EXPERIMENT_PARAMETER_ARRAYSEP_CHAR		","

class StringArrayPropertyType{};
Q_DECLARE_METATYPE(StringArrayPropertyType)

class StringArrayPropertyWidget : public QFrame 
{
	Q_OBJECT

signals:
	void PropertyWidgetChanged(const QString&);

public:
	StringArrayPropertyWidget(QWidget *parent = NULL);
	~StringArrayPropertyWidget();

	void setText(const QString &sText);

private:
	QHBoxLayout *layout;
	QTextEdit *textEdit;
	QPushButton *button;
};

class RotationDirectionPropertyType{};
Q_DECLARE_METATYPE(RotationDirectionPropertyType)

class RotationDirectionPropertyWidget : public QComboBox 
{
	Q_OBJECT

signals:
	void PropertyWidgetChanged(const QString&);

public:
	RotationDirectionPropertyWidget(QWidget *parent = NULL);

};

////////////////////////////////////////////////////////////////////////////////////////////////////

class VariantExtensionPropertyManager :	public QtVariantPropertyManager
{
	Q_OBJECT

signals:
	void valueChanged(QtProperty *, const QString &);
	void filterChanged(QtProperty *, const QString &);

public:

	enum RotationDirectionEnum
	{
		ROTATION_DIR_COUNTERCLOCKWISE	= -1,
		ROTATION_DIR_UNDEFINED			=  0,
		ROTATION_DIR_CLOCKWISE			=  1
	};

	VariantExtensionPropertyManager(QObject *parent = NULL);
	~VariantExtensionPropertyManager();

	static int rotationDirectionTypeId()
	{
		return qMetaTypeId<RotationDirectionPropertyType>();
	}
	static int stringArrayTypeId()
	{
		return qMetaTypeId<StringArrayPropertyType>();
	}

	static QString rotationDirectionString(enum RotationDirectionEnum);
	bool isPropertyTypeSupported(int propertyType) const;
	bool hasCustomPropertyType(const QtProperty *property) const;
	bool isManagedCustomPropertyType(const QtProperty *property) const;
	QVariant value(const QtProperty *property) const;

	//QString filter(const QtProperty *property) const
	//{
	//	QString a = "";
	//	if(isManagedCustomPropertyType(property))
	//	{
	//		a = "";
	//	}		
	//	return a;
	//};

public slots:
	void setValue(QtProperty *property, const QVariant &val);

	//void setFilter(QtProperty *property, const QString &filter)
	//{
	//	if(isManagedCustomPropertyType(property))
	//	{
	//		QString a = "";
	//	}			
	//	return;
	//};

protected:
	QString valueText(const QtProperty *property) const;
	void initializeProperty(QtProperty *property);
	void uninitializeProperty(QtProperty *property);

private:
	struct Data 
	{
		QString value;
		//QString filter;
	};
	QMap<const QtProperty *, Data> theValues;
	static QMap<int, QString> mRotationDirection;
};

//////////////////////////////////////////////////////////////////////////////////

class VariantExtensionPropertyFactory :	public QtVariantEditorFactory
{
	Q_OBJECT

public:
	VariantExtensionPropertyFactory(QObject *parent = NULL) : QtVariantEditorFactory(parent){};
	~VariantExtensionPropertyFactory(){};

private slots:
	void slotSetValue(const QString &val);
	void slotEditorDestroyed(QObject *obj);

private:
	QMap<QtProperty *, QList<QWidget *>> createdEditors;
	QMap<QWidget *, QtProperty *> editorToProperty;

protected:
	QWidget *createEditor(QtVariantPropertyManager *manager, QtProperty *property, QWidget *parent);
};

#endif

