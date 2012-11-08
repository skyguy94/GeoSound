#include "pch.h"
#include "ManagedSineWaveOscillator.h"
#include "ManagedAudioManager.h"
#include "DirectXPage.xaml.h"

using std::unique_ptr;
using namespace GeoSound;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Input;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Shapes;
using namespace Windows::Graphics::Display;

DirectXPage::DirectXPage() :
	m_renderNeeded(true),
	m_lastPointValid(false),
	audioManager_(ref new ManagedAudioManager())
{
	InitializeComponent();

	m_renderer = ref new WaveRenderer(audioManager_);

	m_renderer->Initialize(
Window::Current->CoreWindow, SwapChainPanel, DisplayProperties::LogicalDpi);

	Window::Current->CoreWindow->SizeChanged += ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &DirectXPage::OnWindowSizeChanged);
	DisplayProperties::LogicalDpiChanged += ref new DisplayPropertiesEventHandler(this, &DirectXPage::OnLogicalDpiChanged);
	m_eventToken = CompositionTarget::Rendering::add(ref new EventHandler<Object^>(this, &DirectXPage::OnRendering));

	DataContext = this;

	m_timer = ref new BasicTimer();
}

DirectXPage::~DirectXPage()
{ }

void DirectXPage::OnPointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args)
{
	auto currentPoint = args->GetCurrentPoint(nullptr);
	if (currentPoint->IsInContact)
	{
		if (m_lastPointValid)
		{
			Windows::Foundation::Point delta(currentPoint->Position.X - m_lastPoint.X,currentPoint->Position.Y - m_lastPoint.Y);
			m_renderer->UpdateTextPosition(delta);
			m_renderNeeded = true;
		}
		m_lastPoint = currentPoint->Position;
		m_lastPointValid = true;
	}
	else
	{
		m_lastPointValid = false;
	}
}

void DirectXPage::OnPointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args)
{
	m_lastPointValid = false;
}

void DirectXPage::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	m_renderNeeded = true;
}

void DirectXPage::OnLogicalDpiChanged(Platform::Object^ sender)
{
	m_renderNeeded = true;
}

void DirectXPage::OnRendering(Platform::Object^ sender, Platform::Object^ args)
{
	if (m_renderNeeded)
	{
		m_timer->Update();
		m_renderer->Update(m_timer->Total, m_timer->Delta);
		m_renderer->Render();
		m_renderer->Present();
		m_renderNeeded = false;
		m_renderNeeded = false;
	}
}

void GeoSound::DirectXPage::Button_PointerReleased_1(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	audioManager_->ClearSounds();
	m_renderNeeded = true;
}


void GeoSound::DirectXPage::Button_PointerPressed_1(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	Rectangle^rect = dynamic_cast<Rectangle^>(sender); 
	if (rect->Name == "A4")
	{
		audioManager_->AddSound(ref new ManagedSineWaveOscillator(440, 100));
	}
	else
	if (rect->Name == "D4")
	{
		audioManager_->AddSound(ref new ManagedSineWaveOscillator(440, 100));
		audioManager_->AddSound(ref new ManagedSineWaveOscillator(600, 100));
	}
	else
	if (rect->Name == "F4")
	{
		audioManager_->AddSound(ref new ManagedSineWaveOscillator(440, 100));
		audioManager_->AddSound(ref new ManagedSineWaveOscillator(600, 100));
		audioManager_->AddSound(ref new ManagedSineWaveOscillator(780, 100));
	}
	m_renderNeeded = true;
}
