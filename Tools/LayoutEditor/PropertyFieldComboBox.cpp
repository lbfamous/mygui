/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldComboBox.h"
#include "Localise.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "UndoManager.h"
#include "SettingsManager.h"

namespace tools
{
	const std::string DEFAULT_STRING = "[DEFAULT]";

	PropertyFieldComboBox::PropertyFieldComboBox(MyGUI::Widget* _parent) :
		BaseLayout("PropertyFieldComboBox.layout", _parent),
		mText(nullptr),
		mField(nullptr)
	{
		assignWidget(mText, "Text");
		assignWidget(mField, "Field");

		mField->eventComboAccept += newDelegate (this, &PropertyFieldComboBox::notifyForceApplyProperties2);
		mField->eventToolTip += newDelegate (this, &PropertyFieldComboBox::notifyToolTip);
	}

	PropertyFieldComboBox::~PropertyFieldComboBox()
	{
	}

	void PropertyFieldComboBox::initialise(const std::string& _type, MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;
		mType = _type;

		onFillValues();
	}

	void PropertyFieldComboBox::onFillValues()
	{
		WidgetStyle::VectorString values = WidgetTypes::getInstance().findPossibleValues(mType);

		for (WidgetStyle::VectorString::iterator iter = values.begin(); iter != values.end(); ++iter)
			mField->addItem(*iter);
		mField->beginToItemFirst();
	}

	void PropertyFieldComboBox::notifyApplyProperties(MyGUI::Widget* _sender)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;
		std::string value = mField->getOnlyText();

		if (value == DEFAULT_STRING && mField->getCaption() == DEFAULT_VALUE)
			value = "";

		onAction(value);
	}

	void PropertyFieldComboBox::onAction(const std::string& _value)
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

		bool success = EditorWidgets::getInstance().tryToApplyProperty(widgetContainer->widget, mName, _value);

		bool found = false;
		// ���� ����� ��-�� ����, �� ������� (��� ������ ���� ������) ��������
		for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
		{
			if (iterProperty->first == mName)
			{
				found = true;
				if (_value.empty())
					widgetContainer->mProperty.erase(iterProperty);
				else
					iterProperty->second = _value;
				break;
			}
		}

		// ���� ������ �������� �� ���� ������, �� ���������
		if (!_value.empty() && !found)
			widgetContainer->mProperty.push_back(MyGUI::PairString(mName, _value));

		UndoManager::getInstance().addValue(PR_PROPERTIES);
	}

	void PropertyFieldComboBox::notifyForceApplyProperties2(MyGUI::ComboBox* _sender, size_t _index)
	{
		notifyApplyProperties(_sender);
	}

	void PropertyFieldComboBox::onToolTip(const MyGUI::ToolTipInfo& _info)
	{
	}

	void PropertyFieldComboBox::notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info)
	{
		onToolTip(_info);
	}

	MyGUI::IntSize PropertyFieldComboBox::getContentSize()
	{
		return MyGUI::IntSize(0, SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("PropertyItemHeight"));
	}

	void PropertyFieldComboBox::setCoord(const MyGUI::IntCoord& _coord)
	{
		mMainWidget->setCoord(_coord);
	}

	void PropertyFieldComboBox::setValue(const std::string& _value)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		if (_value.empty())
		{
			mField->setCaption(DEFAULT_VALUE);
		}
		else
		{
			mField->setOnlyText(_value);
		}
	}

	void PropertyFieldComboBox::setName(const std::string& _value)
	{
		mName = _value;
		mText->setCaption(_value);
	}

} // namespace tools
