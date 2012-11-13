#pragma once

#include "DirectXPage.g.h"
#include "WaveRenderer.h"
#include "BasicTimer.h"

namespace GeoSound
{
    [Windows::Foundation::Metadata::WebHostHidden]
	private ref class DirectXPage sealed
	{
	public:
		DirectXPage();
		virtual ~DirectXPage();

	private:
		void OnPointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args);
		void OnPointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args);
		void OnPointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args);
		
		void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
		void OnLogicalDpiChanged(Platform::Object^ sender);
		void OnRendering(Object^ sender, Object^ args);

		void Button_PointerReleased_1(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void Button_PointerPressed_1(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);

		Windows::Foundation::EventRegistrationToken m_eventToken;
		WaveRenderer^ m_renderer;
		std::unique_ptr<AudioManager> audioManager_;

		bool m_renderNeeded;

		Windows::Foundation::Point m_lastPoint;
		bool m_lastPointValid;
		BasicTimer ^m_timer;
	};
}
