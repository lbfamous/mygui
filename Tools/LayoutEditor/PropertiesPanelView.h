/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/
#ifndef __PROPERTIES_PANEL_VIEW_H__
#define __PROPERTIES_PANEL_VIEW_H__

#include "BaseLayout/BaseLayout.h"
#include "PropertyField.h"
#include "PanelView.h"
#include "PanelMainProperties.h"
#include "PanelProperties.h"
#include "PanelItems.h"
#include "PanelUserData.h"
#include "PanelControllers.h"
#include "EditorToolTip.h"

namespace tools
{
	class PropertiesPanelView :
		public wraps::BaseLayout
	{
	public:
		PropertiesPanelView(MyGUI::Widget* _parent = nullptr);
		virtual ~PropertiesPanelView();

	private:
		void notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget);
		void notifyWindowChangeCoord(MyGUI::Window* _sender);

		void hideWidgetsPairs(MyGUI::Widget* _window);
		void createPropertiesWidgetsPair(MyGUI::Widget* _window, const std::string& _property, const std::string& _value, const std::string& _type, int y, PropertyField& _field);

		PanelProperties* getPropertyWindow(WidgetStyle* _style);

	private:
		MyGUI::IntSize mOldSize;
		PanelView* mPanelView;

		typedef std::vector<PropertyField> VectorPropertyField;
		typedef std::map<MyGUI::Widget*, VectorPropertyField> MapInfo;
		MapInfo mPropertyInfo;

		PanelMainProperties* mPanelMainProperties;

		typedef std::map<WidgetStyle*, PanelProperties*> MapPropertyWindow;
		MapPropertyWindow mMapPropertyWindow;

		PanelItems* mPanelItems;
		PanelUserData* mPanelUserData;
		PanelControllers* mPanelControllers;

		typedef std::vector<wraps::BasePanelViewItem*> VectorPanel;
		VectorPanel mPanels;

		MyGUI::Widget* mCurrentWidget;
		int mPropertyItemHeight;

		EditorToolTip* mToolTip;
	};

} // namespace tools

#endif // __PROPERTIES_PANEL_VIEW_H__
