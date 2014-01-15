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
#include <QVBoxLayout>
#include <QComboBox>
#include <QDialog>
#include <QPlainTextEdit>
#include <QDialogButtonBox>

#define EXPERIMENT_PARAMETER_ARRAYSEP_CHAR		","

class StringArrayPropertyType{};
Q_DECLARE_METATYPE(StringArrayPropertyType)

class StringArrayPropertyWidget : public QComboBox 
{
	Q_OBJECT

signals:
	void PropertyWidgetChanged(const QString&);

public:
	StringArrayPropertyWidget(QWidget *parent = NULL);
	~StringArrayPropertyWidget();

	void setText(const QString &sText);
	void setSeperator(const QString &sSeperator);

private slots:
	void currentIndexChangedSlot(int nIndex);
	void checkAcception();

private:
	QDialog *tmpDialog;
	QVBoxLayout *tmpLayout;
	QPlainTextEdit *tmpTextEdit;
	QDialogButtonBox *buttonBox;

	QString sCurrentList;
	QString sCurrentSeperator;
};

class RotationDirectionPropertyType{};
Q_DECLARE_METATYPE(RotationDirectionPropertyType)
class MovementDirectionPropertyType{};
Q_DECLARE_METATYPE(MovementDirectionPropertyType)
class EccentricityDirectionPropertyType{};
Q_DECLARE_METATYPE(EccentricityDirectionPropertyType)

class RotationDirectionPropertyWidget : public QComboBox 
{
	Q_OBJECT

signals:
	void PropertyWidgetChanged(const QString&);

private slots:
	void currentIndexChangedSlot(int nIndex);

public:
	RotationDirectionPropertyWidget(QWidget *parent = NULL);

	enum RotationDirectionEnum
	{
		ROTATION_DIR_COUNTERCLOCKWISE	= -1,
		ROTATION_DIR_UNDEFINED			=  0,
		ROTATION_DIR_CLOCKWISE			=  1
	};

	static QString rotationDirectionString(enum RotationDirectionEnum);
	static RotationDirectionEnum rotationDirectionEnum(const QString &sName);
	void setValue(const QString &sText);

private:
	static QMap<int, RotationDirectionEnum> indexToEnumHash;
	static QMap<int, QString> mRotationDirection;
};

class MovementDirectionPropertyWidget : public QComboBox 
{
	Q_OBJECT

signals:
	void PropertyWidgetChanged(const QString&);

private slots:
	void currentIndexChangedSlot(int nIndex);

public:
	MovementDirectionPropertyWidget(QWidget *parent = NULL);

	enum MovementDirectionEnum
	{
		MOVEMENT_DIR_DOWNUP				= -1,
		MOVEMENT_DIR_UNDEFINED			=  0,
		MOVEMENT_DIR_UPDOWN				=  1
	};

	static QString movementDirectionString(enum MovementDirectionEnum eValue);
	static MovementDirectionEnum movementDirectionEnum(const QString &sName);
	void setValue(const QString &sText);

private:
	static QMap<int, MovementDirectionEnum> indexToEnumHash;
	static QMap<int, QString> mMovementDirection;
};

class EccentricityDirectionPropertyWidget : public QComboBox 
{
	Q_OBJECT

signals:
	void PropertyWidgetChanged(const QString&);

private slots:
	void currentIndexChangedSlot(int nIndex);

public:
	EccentricityDirectionPropertyWidget(QWidget *parent = NULL);

	enum EccentricityDirectionEnum
	{
		ECCENTRICITY_DIR_DECREASE		= -1,
		ECCENTRICITY_DIR_UNDEFINED		=  0,
		ECCENTRICITY_DIR_INCREASE		=  1
	};

	static QString eccentricityDirectionString(enum EccentricityDirectionEnum eValue);
	static EccentricityDirectionEnum eccentricityDirectionEnum(const QString &sName);
	void setValue(const QString &sText);

private:
	static QMap<int, EccentricityDirectionEnum> indexToEnumHash;
	static QMap<int, QString> mEccentricityDirection;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class VariantExtensionPropertyManager :	public QtVariantPropertyManager
{
	Q_OBJECT

signals:
	void filterChanged(QtProperty *, const QString &);

public:

	VariantExtensionPropertyManager(QObject *parent = NULL);
	~VariantExtensionPropertyManager();

	static int rotationDirectionTypeId()
	{
		return qMetaTypeId<RotationDirectionPropertyType>();
	}
	static int movementDirectionTypeId()
	{
		return qMetaTypeId<MovementDirectionPropertyType>();
	}
	static int eccentricityDirectionTypeId()
	{
		return qMetaTypeId<EccentricityDirectionPropertyType>();
	}
	static int stringArrayTypeId()
	{
		return qMetaTypeId<StringArrayPropertyType>();
	}

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

