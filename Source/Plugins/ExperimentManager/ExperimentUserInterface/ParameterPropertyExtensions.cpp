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

#include "ParameterPropertyExtensions.h"

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
	this->addItem(QIcon(":/resources/Clockwise.png"),VariantExtensionPropertyManager::rotationDirectionString(VariantExtensionPropertyManager::ROTATION_DIR_CLOCKWISE), VariantExtensionPropertyManager::ROTATION_DIR_CLOCKWISE);
	this->addItem(QIcon(":/resources/Counterclockwise.png"),VariantExtensionPropertyManager::rotationDirectionString(VariantExtensionPropertyManager::ROTATION_DIR_COUNTERCLOCKWISE), VariantExtensionPropertyManager::ROTATION_DIR_COUNTERCLOCKWISE);
};

MovementDirectionPropertyWidget::MovementDirectionPropertyWidget(QWidget *parent) : QComboBox(parent) 
{
	this->addItem(QIcon(":/resources/Upwards.png"),VariantExtensionPropertyManager::movementDirectionString(VariantExtensionPropertyManager::MOVEMENT_DIR_DOWNUP), VariantExtensionPropertyManager::MOVEMENT_DIR_DOWNUP);
	this->addItem(QIcon(":/resources/Downwards.png"),VariantExtensionPropertyManager::movementDirectionString(VariantExtensionPropertyManager::MOVEMENT_DIR_UPDOWN), VariantExtensionPropertyManager::MOVEMENT_DIR_UPDOWN);
};

EccentricityDirectionPropertyWidget::EccentricityDirectionPropertyWidget(QWidget *parent) : QComboBox(parent) 
{
	this->addItem(QIcon(":/resources/Decrease.png"),VariantExtensionPropertyManager::eccentricityDirectionString(VariantExtensionPropertyManager::ECCENTRICITY_DIR_DECREASE), VariantExtensionPropertyManager::ECCENTRICITY_DIR_DECREASE);
	this->addItem(QIcon(":/resources/Increase.png"),VariantExtensionPropertyManager::eccentricityDirectionString(VariantExtensionPropertyManager::ECCENTRICITY_DIR_INCREASE), VariantExtensionPropertyManager::ECCENTRICITY_DIR_INCREASE);
};

/////////////////////////////////////////////////////////////////////////////////

QMap<int, QString> VariantExtensionPropertyManager::mRotationDirection;
QMap<int, QString> VariantExtensionPropertyManager::mMovementDirection;
QMap<int, QString> VariantExtensionPropertyManager::mEccentricityDirection;

VariantExtensionPropertyManager::VariantExtensionPropertyManager(QObject *parent) : QtVariantPropertyManager(parent)
{
	mRotationDirection[ROTATION_DIR_CLOCKWISE] = "Clockwise";
	mRotationDirection[ROTATION_DIR_COUNTERCLOCKWISE] = "CounterClockwise";
	mMovementDirection[MOVEMENT_DIR_DOWNUP] = "Upwards";
	mMovementDirection[MOVEMENT_DIR_UPDOWN] = "Downwards";
	mEccentricityDirection[ECCENTRICITY_DIR_DECREASE] = "Decrease";
	mEccentricityDirection[ECCENTRICITY_DIR_INCREASE] = "Increase";
}

VariantExtensionPropertyManager::~VariantExtensionPropertyManager(void)
{
}

QString VariantExtensionPropertyManager::rotationDirectionString(enum RotationDirectionEnum eValue)
{
	return mRotationDirection[eValue];
};

QString VariantExtensionPropertyManager::movementDirectionString(enum MovementDirectionEnum eValue)
{
	return mMovementDirection[eValue];
};

QString VariantExtensionPropertyManager::eccentricityDirectionString(enum EccentricityDirectionEnum eValue)
{
	return mEccentricityDirection[eValue];
};

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
			RotationDirectionEnum tmpEnum = (RotationDirectionEnum)val.toInt();
			data.value = VariantExtensionPropertyManager::rotationDirectionString(tmpEnum);
		}
		else if(propertyType(property) == (QVariant::Type) VariantExtensionPropertyManager::movementDirectionTypeId())
		{
			MovementDirectionEnum tmpEnum = (MovementDirectionEnum)val.toInt();
			data.value = VariantExtensionPropertyManager::movementDirectionString(tmpEnum);
		}
		else if(propertyType(property) == (QVariant::Type) VariantExtensionPropertyManager::eccentricityDirectionTypeId())
		{
			EccentricityDirectionEnum tmpEnum = (EccentricityDirectionEnum)val.toInt();
			data.value = VariantExtensionPropertyManager::eccentricityDirectionString(tmpEnum);
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
};

void VariantExtensionPropertyFactory::slotEditorDestroyed(QObject *obj)
{
	Q_UNUSED(obj);
};

QWidget *VariantExtensionPropertyFactory::createEditor(QtVariantPropertyManager *manager, QtProperty *property, QWidget *parent)
{
	if (manager->propertyType(property) == VariantExtensionPropertyManager::rotationDirectionTypeId()) 
	{
		RotationDirectionPropertyWidget *editor = new RotationDirectionPropertyWidget(parent);
		bool bResult;
		//editor->setText(manager->value(property).toString());
		//editor->setFilter(manager->filter(property));
		createdEditors[property].append(editor);
		editorToProperty[editor] = property;
		bResult = connect(editor, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(slotSetValue(const QString &)));
		bResult = connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
		return editor;
	}
	else if (manager->propertyType(property) == VariantExtensionPropertyManager::movementDirectionTypeId()) 
	{
		MovementDirectionPropertyWidget *editor = new MovementDirectionPropertyWidget(parent);
		bool bResult;
		//editor->setText(manager->value(property).toString());
		//editor->setFilter(manager->filter(property));
		createdEditors[property].append(editor);
		editorToProperty[editor] = property;
		bResult = connect(editor, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(slotSetValue(const QString &)));
		bResult = connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
		return editor;
	}
	else if (manager->propertyType(property) == VariantExtensionPropertyManager::eccentricityDirectionTypeId()) 
	{
		EccentricityDirectionPropertyWidget *editor = new EccentricityDirectionPropertyWidget(parent);
		bool bResult;
		//editor->setText(manager->value(property).toString());
		//editor->setFilter(manager->filter(property));
		createdEditors[property].append(editor);
		editorToProperty[editor] = property;
		bResult = connect(editor, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(slotSetValue(const QString &)));
		bResult = connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
		return editor;
	}
	else if (manager->propertyType(property) == VariantExtensionPropertyManager::stringArrayTypeId()) 
	{
		StringArrayPropertyWidget *editor = new StringArrayPropertyWidget(parent);
		bool bResult;
		//QStringList tmpStringList = manager->value(property).toString().split(EXPERIMENT_PARAMETER_ARRAYSEP_CHAR,QString::SkipEmptyParts);
		//editor->setText(manager->value(property).toString().replace(EXPERIMENT_PARAMETER_ARRAYSEP_CHAR,"\n"));//   .split(EXPERIMENT_PARAMETER_ARRAYSEP_CHAR,QString::SkipEmptyParts))
		//editor->setText(manager->value(property).toString());
		//editor->setFilter(manager->filter(property));
		createdEditors[property].append(editor);
		editorToProperty[editor] = property;
		bResult = connect(editor, SIGNAL(PropertyWidgetChanged(const QString&)), this, SLOT(slotSetValue(const QString &)));
		editor->setSeperator(EXPERIMENT_PARAMETER_ARRAYSEP_CHAR);
		editor->setText(manager->value(property).toString());
		bResult = connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
		return editor;
	}
	return QtVariantEditorFactory::createEditor(manager,property,parent);
}