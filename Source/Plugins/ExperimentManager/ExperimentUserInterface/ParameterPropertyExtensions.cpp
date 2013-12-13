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

StringArrayPropertyWidget::StringArrayPropertyWidget(QWidget *parent) : QFrame(parent), textEdit(NULL), button(NULL), layout(NULL)
{
	textEdit = new QTextEdit();
	button = new QPushButton();
	layout = new QHBoxLayout();



	textEdit->setText("AA BB");
	button->setText("Butt");

	layout->addWidget(textEdit);
	layout->addWidget(button);

	this->setLayout(layout);
	this->show();

	//this->addItem(QIcon(":/resources/Clockwise.png"),VariantExtensionPropertyManager::rotationDirectionString(VariantExtensionPropertyManager::ROTATION_DIR_CLOCKWISE), VariantExtensionPropertyManager::ROTATION_DIR_CLOCKWISE);
	//this->addItem(QIcon(":/resources/Counterclockwise.png"),VariantExtensionPropertyManager::rotationDirectionString(VariantExtensionPropertyManager::ROTATION_DIR_COUNTERCLOCKWISE), VariantExtensionPropertyManager::ROTATION_DIR_COUNTERCLOCKWISE);
	//this->setText("Pietje");
};

StringArrayPropertyWidget::~StringArrayPropertyWidget()
{
	if (textEdit)
		delete textEdit;
	if (button)
		delete button;
	if (layout)
		delete layout;
}

void StringArrayPropertyWidget::setText(const QString &sText)
{
	if(textEdit)
		textEdit->setText(sText);
}

/////////////////////////////////////////////////////////////////////////////////

RotationDirectionPropertyWidget::RotationDirectionPropertyWidget(QWidget *parent) : QComboBox(parent) 
{
	this->addItem(QIcon(":/resources/Clockwise.png"),VariantExtensionPropertyManager::rotationDirectionString(VariantExtensionPropertyManager::ROTATION_DIR_CLOCKWISE), VariantExtensionPropertyManager::ROTATION_DIR_CLOCKWISE);
	this->addItem(QIcon(":/resources/Counterclockwise.png"),VariantExtensionPropertyManager::rotationDirectionString(VariantExtensionPropertyManager::ROTATION_DIR_COUNTERCLOCKWISE), VariantExtensionPropertyManager::ROTATION_DIR_COUNTERCLOCKWISE);
};

/////////////////////////////////////////////////////////////////////////////////

QMap<int, QString> VariantExtensionPropertyManager::mRotationDirection;

VariantExtensionPropertyManager::VariantExtensionPropertyManager(QObject *parent) : QtVariantPropertyManager(parent)
{
	mRotationDirection[ROTATION_DIR_CLOCKWISE] = "Clockwise";
	mRotationDirection[ROTATION_DIR_COUNTERCLOCKWISE] = "CounterClockwise";
}

VariantExtensionPropertyManager::~VariantExtensionPropertyManager(void)
{
}

QString VariantExtensionPropertyManager::rotationDirectionString(enum RotationDirectionEnum eValue)
{
	return mRotationDirection[eValue];
};

bool VariantExtensionPropertyManager::isPropertyTypeSupported(int propertyType) const
{
	if ((propertyType == rotationDirectionTypeId()) || (propertyType == stringArrayTypeId()))
		return true;
	return QtVariantPropertyManager::isPropertyTypeSupported(propertyType);
}

bool VariantExtensionPropertyManager::hasCustomPropertyType(const QtProperty *property) const
{
	if((propertyType(property) == (QVariant::Type) VariantExtensionPropertyManager::rotationDirectionTypeId()) ||
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
	else if (manager->propertyType(property) == VariantExtensionPropertyManager::stringArrayTypeId()) 
	{
		StringArrayPropertyWidget *editor = new StringArrayPropertyWidget(parent);
		bool bResult;
		QStringList tmpStringList = manager->value(property).toString().split(EXPERIMENT_PARAMETER_ARRAYSEP_CHAR,QString::SkipEmptyParts);
		//editor->setText(manager->value(property).toString().replace(EXPERIMENT_PARAMETER_ARRAYSEP_CHAR,"\n"));//   .split(EXPERIMENT_PARAMETER_ARRAYSEP_CHAR,QString::SkipEmptyParts))
		//editor->setText(manager->value(property).toString());
		//editor->setFilter(manager->filter(property));
		createdEditors[property].append(editor);
		editorToProperty[editor] = property;
		//bResult = connect(editor, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(slotSetValue(const QString &)));
		bResult = connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
		return editor;
	}
	return QtVariantEditorFactory::createEditor(manager,property,parent);
}