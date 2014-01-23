//ExperimentManagerplugin
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

#include "ParameterPropertyExtensions.h"

QMap<int, QString> RotationDirectionPropertyWidget::mRotationDirection;
QMap<int, RotationDirectionPropertyWidget::RotationDirectionEnum> RotationDirectionPropertyWidget::indexToEnumHash;
QMap<int, QString> MovementDirectionPropertyWidget::mMovementDirection;
QMap<int, MovementDirectionPropertyWidget::MovementDirectionEnum> MovementDirectionPropertyWidget::indexToEnumHash;
QMap<int, QString> EccentricityDirectionPropertyWidget::mEccentricityDirection;
QMap<int, EccentricityDirectionPropertyWidget::EccentricityDirectionEnum> EccentricityDirectionPropertyWidget::indexToEnumHash;

///////////////////////////////////////////////////////////////////////////////

StringArrayPropertyWidget::StringArrayPropertyWidget(QWidget *parent) : QComboBox(parent)
{
	tmpDialog = NULL;
	tmpLayout = NULL;
	tmpTextEdit = NULL;
	buttonBox = NULL;
	sCurrentList = "";
	sCurrentSeperator = EXPERIMENT_PARAMETER_ARRAYSEP_CHAR;//Default value...
	//this->setEditable(true);
	this->setFrame(false);
	this->addItem(sCurrentList);
	this->addItem("<Edit...>");
	this->setCurrentText("");
	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChangedSlot(int)));
};

StringArrayPropertyWidget::~StringArrayPropertyWidget()
{
	if(tmpTextEdit)
	{
		delete tmpTextEdit;
		tmpTextEdit = NULL;
	}
	if(buttonBox)
	{
		delete buttonBox;
		buttonBox = NULL;
	}
	if(tmpLayout)
	{
		delete tmpLayout;
		tmpLayout = NULL;
	}
	if(tmpDialog)
	{
		delete tmpDialog;
		tmpDialog = NULL;
	}
}

void StringArrayPropertyWidget::setText(const QString &sText)
{
	sCurrentList = sText;
	if(this->count() > 1)
	{
		this->setItemText(0,sCurrentList);
		emit PropertyWidgetChanged(sCurrentList);
	}
}

void StringArrayPropertyWidget::setSeperator(const QString &sSeperator)
{
	sCurrentSeperator = sSeperator;
}

void StringArrayPropertyWidget::currentIndexChangedSlot(int nIndex)
{
	if(nIndex==1)//Edit
	{
		if(tmpDialog == NULL)
		{
			tmpDialog = new QDialog(this);
			tmpLayout = new QVBoxLayout(tmpDialog);
			tmpTextEdit = new QPlainTextEdit(this);
			tmpLayout->addWidget(tmpTextEdit);
			buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
			tmpLayout->addWidget(buttonBox);
			connect(buttonBox, SIGNAL(accepted()), this, SLOT(checkAcception()));
			connect(buttonBox, SIGNAL(rejected()), tmpDialog, SLOT(reject()));
		}
		tmpTextEdit->setPlainText(sCurrentList.replace(sCurrentSeperator,"\n"));
		if(tmpDialog->exec() == 1)
		{
			QStringList tmpStrings = tmpTextEdit->toPlainText().split("\n",QString::SkipEmptyParts);
			setText(tmpStrings.join(sCurrentSeperator));
		}			
		setCurrentIndex(0);
	}
}

void StringArrayPropertyWidget::checkAcception()
{	
	if(tmpDialog)
	{

		tmpDialog->accept();
	}	
}

/////////////////////////////////////////////////////////////////////////////////

RotationDirectionPropertyWidget::RotationDirectionPropertyWidget(QWidget *parent) : QComboBox(parent) 
{
	int nIndex = 0;
	this->addItem(QIcon(":/resources/Clockwise.png"),rotationDirectionString(ROTATION_DIR_CLOCKWISE), ROTATION_DIR_CLOCKWISE);
	indexToEnumHash[nIndex] = ROTATION_DIR_CLOCKWISE;
	nIndex++;
	this->addItem(QIcon(":/resources/Counterclockwise.png"),rotationDirectionString(ROTATION_DIR_COUNTERCLOCKWISE), ROTATION_DIR_COUNTERCLOCKWISE);
	indexToEnumHash[nIndex] = ROTATION_DIR_COUNTERCLOCKWISE;
	nIndex++;
	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChangedSlot(int)));
}

QString RotationDirectionPropertyWidget::rotationDirectionString(enum RotationDirectionEnum eValue)
{
	if(mRotationDirection.isEmpty())
	{
		mRotationDirection[RotationDirectionPropertyWidget::ROTATION_DIR_UNDEFINED] = "Undefined";
		mRotationDirection[RotationDirectionPropertyWidget::ROTATION_DIR_CLOCKWISE] = "Clockwise";	
		mRotationDirection[RotationDirectionPropertyWidget::ROTATION_DIR_COUNTERCLOCKWISE] = "CounterClockwise";
	}
	return mRotationDirection[eValue];
}

RotationDirectionPropertyWidget::RotationDirectionEnum RotationDirectionPropertyWidget::rotationDirectionEnum(const QString &sName)
{
	if(QString::compare(mRotationDirection[RotationDirectionPropertyWidget::ROTATION_DIR_CLOCKWISE], sName, Qt::CaseInsensitive) == 0)
		return ROTATION_DIR_CLOCKWISE;
	else if(QString::compare(mRotationDirection[RotationDirectionPropertyWidget::ROTATION_DIR_COUNTERCLOCKWISE], sName, Qt::CaseInsensitive) == 0)
		return ROTATION_DIR_COUNTERCLOCKWISE;
	//else if(QString::compare(mRotationDirection[RotationDirectionPropertyWidget::ROTATION_DIR_UNDEFINED], sName, Qt::CaseInsensitive) == 0)
	//	return ROTATION_DIR_UNDEFINED;
	return ROTATION_DIR_UNDEFINED; 
};

void RotationDirectionPropertyWidget::currentIndexChangedSlot(int nIndex)
{
	emit PropertyWidgetChanged(QString::number((int)indexToEnumHash[nIndex]));
}

void RotationDirectionPropertyWidget::setValue(const QString &sText)
{
	this->setCurrentText(sText);
}

////////////////////////////////////////////////////////////////////////////////////////

MovementDirectionPropertyWidget::MovementDirectionPropertyWidget(QWidget *parent) : QComboBox(parent) 
{
	int nIndex = 0;
	this->addItem(QIcon(":/resources/Upwards.png"),movementDirectionString(MOVEMENT_DIR_DOWNUP), MOVEMENT_DIR_DOWNUP);
	indexToEnumHash[nIndex] = MOVEMENT_DIR_DOWNUP;
	nIndex++;
	this->addItem(QIcon(":/resources/Downwards.png"),movementDirectionString(MOVEMENT_DIR_UPDOWN), MOVEMENT_DIR_UPDOWN);
	indexToEnumHash[nIndex] = MOVEMENT_DIR_UPDOWN;
	nIndex++;
	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChangedSlot(int)));
}

QString MovementDirectionPropertyWidget::movementDirectionString(enum MovementDirectionEnum eValue)
{
	if(mMovementDirection.isEmpty())
	{
		mMovementDirection[MovementDirectionPropertyWidget::MOVEMENT_DIR_UNDEFINED] = "Undefined";
		mMovementDirection[MovementDirectionPropertyWidget::MOVEMENT_DIR_DOWNUP] = "Upwards";	
		mMovementDirection[MovementDirectionPropertyWidget::MOVEMENT_DIR_UPDOWN] = "Downwards";
	}
	return mMovementDirection[eValue];
}

MovementDirectionPropertyWidget::MovementDirectionEnum MovementDirectionPropertyWidget::movementDirectionEnum(const QString &sName)
{
	if(QString::compare(mMovementDirection[MovementDirectionPropertyWidget::MOVEMENT_DIR_DOWNUP], sName, Qt::CaseInsensitive) == 0)
		return MOVEMENT_DIR_DOWNUP;
	else if(QString::compare(mMovementDirection[MovementDirectionPropertyWidget::MOVEMENT_DIR_UPDOWN], sName, Qt::CaseInsensitive) == 0)
		return MOVEMENT_DIR_UPDOWN;
	//else if(QString::compare(mMovementDirection[MovementDirectionPropertyWidget::MOVEMENT_DIR_UNDEFINED], sName, Qt::CaseInsensitive) == 0)
	//	return MOVEMENT_DIR_UNDEFINED;
	return MOVEMENT_DIR_UNDEFINED; 
}

void MovementDirectionPropertyWidget::currentIndexChangedSlot(int nIndex)
{
	emit PropertyWidgetChanged(QString::number((int)indexToEnumHash[nIndex]));
}

void MovementDirectionPropertyWidget::setValue(const QString &sText)
{
	this->setCurrentText(sText);
}

////////////////////////////////////////////////////////////////////////////////////////

EccentricityDirectionPropertyWidget::EccentricityDirectionPropertyWidget(QWidget *parent) : QComboBox(parent) 
{
	int nIndex = 0;
	this->addItem(QIcon(":/resources/Decrease.png"),eccentricityDirectionString(ECCENTRICITY_DIR_DECREASE), ECCENTRICITY_DIR_DECREASE);
	indexToEnumHash[nIndex] = ECCENTRICITY_DIR_DECREASE;
	nIndex++;
	this->addItem(QIcon(":/resources/Increase.png"),eccentricityDirectionString(ECCENTRICITY_DIR_INCREASE), ECCENTRICITY_DIR_INCREASE);
	indexToEnumHash[nIndex] = ECCENTRICITY_DIR_INCREASE;
	nIndex++;
	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChangedSlot(int)));	
}

QString EccentricityDirectionPropertyWidget::eccentricityDirectionString(enum EccentricityDirectionEnum eValue)
{
	if(mEccentricityDirection.isEmpty())
	{
		mEccentricityDirection[EccentricityDirectionPropertyWidget::ECCENTRICITY_DIR_UNDEFINED] = "Undefined";
		mEccentricityDirection[EccentricityDirectionPropertyWidget::ECCENTRICITY_DIR_DECREASE] = "Decrease";	
		mEccentricityDirection[EccentricityDirectionPropertyWidget::ECCENTRICITY_DIR_INCREASE] = "Increase";
	}
	return mEccentricityDirection[eValue];
}

EccentricityDirectionPropertyWidget::EccentricityDirectionEnum EccentricityDirectionPropertyWidget::eccentricityDirectionEnum(const QString &sName)
{
	if(QString::compare(mEccentricityDirection[EccentricityDirectionPropertyWidget::ECCENTRICITY_DIR_INCREASE], sName, Qt::CaseInsensitive) == 0)
		return ECCENTRICITY_DIR_INCREASE;
	else if(QString::compare(mEccentricityDirection[EccentricityDirectionPropertyWidget::ECCENTRICITY_DIR_DECREASE], sName, Qt::CaseInsensitive) == 0)
		return ECCENTRICITY_DIR_DECREASE;
	//else if(QString::compare(mEccentricityDirection[EccentricityDirectionPropertyWidget::ECCENTRICITY_DIR_UNDEFINED], sName, Qt::CaseInsensitive) == 0)
	//	return ECCENTRICITY_DIR_UNDEFINED;
	return ECCENTRICITY_DIR_UNDEFINED; 
}

void EccentricityDirectionPropertyWidget::currentIndexChangedSlot(int nIndex)
{
	emit PropertyWidgetChanged(QString::number((int)indexToEnumHash[nIndex]));
}

void EccentricityDirectionPropertyWidget::setValue(const QString &sText)
{
	this->setCurrentText(sText);
}

////////////////////////////////////////////////////////////////////////////////////////

VariantExtensionPropertyManager::VariantExtensionPropertyManager(QObject *parent) : QtVariantPropertyManager(parent)
{
}

VariantExtensionPropertyManager::~VariantExtensionPropertyManager(void)
{
}

bool VariantExtensionPropertyManager::isPropertyTypeSupported(int propertyType) const
{
	if ((propertyType == movementDirectionTypeId()) || (propertyType == eccentricityDirectionTypeId()) || (propertyType == rotationDirectionTypeId()) || (propertyType == stringArrayTypeId()))
		return true;
	return QtVariantPropertyManager::isPropertyTypeSupported(propertyType);
}

bool VariantExtensionPropertyManager::hasCustomPropertyType(const QtProperty *property) const
{
	if( (propertyType(property) == (QVariant::Type) VariantExtensionPropertyManager::movementDirectionTypeId()) ||
		(propertyType(property) == (QVariant::Type) VariantExtensionPropertyManager::eccentricityDirectionTypeId()) ||
		(propertyType(property) == (QVariant::Type) VariantExtensionPropertyManager::rotationDirectionTypeId()) ||
		(propertyType(property) == (QVariant::Type) VariantExtensionPropertyManager::stringArrayTypeId()) )
		return true;
	return false;
};

bool VariantExtensionPropertyManager::isManagedCustomPropertyType(const QtProperty *property) const
{
	if(hasCustomPropertyType(property) == true)
	{
		if(theValues.contains(property))
			return true;
		return false;
	}
	return false;
};

QVariant VariantExtensionPropertyManager::value(const QtProperty *property) const
{
	if (isManagedCustomPropertyType(property))
		return theValues[property].value;
	return QtVariantPropertyManager::value(property);
};

void VariantExtensionPropertyManager::setValue(QtProperty *property, const QVariant &val)
{
	if(isManagedCustomPropertyType(property)) 
	{
		Data data = theValues[property];
		if (data.value == val) 
			return;		
		if(propertyType(property) == (QVariant::Type) VariantExtensionPropertyManager::rotationDirectionTypeId())
		{
			RotationDirectionPropertyWidget::RotationDirectionEnum tmpEnum = (RotationDirectionPropertyWidget::RotationDirectionEnum)val.toInt();
			data.value = RotationDirectionPropertyWidget::rotationDirectionString(tmpEnum);
		}
		else if(propertyType(property) == (QVariant::Type) VariantExtensionPropertyManager::movementDirectionTypeId())
		{
			MovementDirectionPropertyWidget::MovementDirectionEnum tmpEnum = (MovementDirectionPropertyWidget::MovementDirectionEnum)val.toInt();
			data.value = MovementDirectionPropertyWidget::movementDirectionString(tmpEnum);
		}
		else if(propertyType(property) == (QVariant::Type) VariantExtensionPropertyManager::eccentricityDirectionTypeId())
		{
			EccentricityDirectionPropertyWidget::EccentricityDirectionEnum tmpEnum = (EccentricityDirectionPropertyWidget::EccentricityDirectionEnum)val.toInt();
			data.value = EccentricityDirectionPropertyWidget::eccentricityDirectionString(tmpEnum);
		}
		else if(propertyType(property) == (QVariant::Type) VariantExtensionPropertyManager::stringArrayTypeId())
		{
			data.value = val.toString();
		}
		else
		{
			data.value = val.toString();
		}		
		theValues[property] = data;
		emit propertyChanged(property);
		emit valueChanged(property, data.value);
		return;
	}
	return QtVariantPropertyManager::setValue(property, val);
};

QString VariantExtensionPropertyManager::valueText(const QtProperty *property) const
{
	if(isManagedCustomPropertyType(property))
		return value(property).toString(); 
	return QtVariantPropertyManager::valueText(property);		
}

void VariantExtensionPropertyManager::initializeProperty(QtProperty *property)
{ 
	if(hasCustomPropertyType(property))
	{
		theValues[property] = Data();
		return;
	}
	return QtVariantPropertyManager::initializeProperty(property);
}

void VariantExtensionPropertyManager::uninitializeProperty(QtProperty *property)
{
	if(isManagedCustomPropertyType(property))
		theValues.remove(property); 
}

///////////////////////////////////////////////////////////////////////////

void VariantExtensionPropertyFactory::slotSetValue(const QString &val)
{
	QObject *sendObj = sender();
	if(sendObj->isWidgetType())
	{
		QWidget *sendWidget = qobject_cast<QWidget *>(sendObj);
		QtProperty *property = editorToProperty[sendWidget];
		if(property)
		{
			this->propertyManager(property)->setValue(property,val);
			return;
		}
	}
	return;
}

void VariantExtensionPropertyFactory::slotEditorDestroyed(QObject *obj)
{
	Q_UNUSED(obj);
}

QWidget *VariantExtensionPropertyFactory::createEditor(QtVariantPropertyManager *manager, QtProperty *property, QWidget *parent)
{
	if (manager->propertyType(property) == VariantExtensionPropertyManager::rotationDirectionTypeId()) 
	{
		RotationDirectionPropertyWidget *editor = new RotationDirectionPropertyWidget(parent);
		editor->setValue(manager->value(property).toString());
		createdEditors[property].append(editor);
		editorToProperty[editor] = property;
		//bResult = connect(editor, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(slotSetValue(const QString &)));
		connect(editor, SIGNAL(PropertyWidgetChanged(const QString&)), this, SLOT(slotSetValue(const QString &)));
		connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
		return editor;
	}
	else if (manager->propertyType(property) == VariantExtensionPropertyManager::movementDirectionTypeId()) 
	{
		MovementDirectionPropertyWidget *editor = new MovementDirectionPropertyWidget(parent);
		editor->setValue(manager->value(property).toString());
		createdEditors[property].append(editor);
		editorToProperty[editor] = property;
		//bResult = connect(editor, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(slotSetValue(const QString &)));
		connect(editor, SIGNAL(PropertyWidgetChanged(const QString&)), this, SLOT(slotSetValue(const QString &)));
		connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
		return editor;
	}
	else if (manager->propertyType(property) == VariantExtensionPropertyManager::eccentricityDirectionTypeId()) 
	{
		EccentricityDirectionPropertyWidget *editor = new EccentricityDirectionPropertyWidget(parent);
		editor->setValue(manager->value(property).toString());
		createdEditors[property].append(editor);
		editorToProperty[editor] = property;
		//bResult = connect(editor, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(slotSetValue(const QString &)));
		connect(editor, SIGNAL(PropertyWidgetChanged(const QString&)), this, SLOT(slotSetValue(const QString &)));
		connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
		return editor;
	}
	else if (manager->propertyType(property) == VariantExtensionPropertyManager::stringArrayTypeId()) 
	{
		StringArrayPropertyWidget *editor = new StringArrayPropertyWidget(parent);
		createdEditors[property].append(editor);
		editorToProperty[editor] = property;
		connect(editor, SIGNAL(PropertyWidgetChanged(const QString&)), this, SLOT(slotSetValue(const QString &)));
		editor->setSeperator(EXPERIMENT_PARAMETER_ARRAYSEP_CHAR);
		editor->setText(manager->value(property).toString());
		connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
		return editor;
	}
	return QtVariantEditorFactory::createEditor(manager,property,parent);
}