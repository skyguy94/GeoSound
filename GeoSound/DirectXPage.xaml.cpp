#include "pch.h"
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
	soundPlayer_(ref new SoundPlayer())
{
	InitializeComponent();

	m_renderer = ref new WaveRenderer(soundPlayer_);

	m_renderer->Initialize(Window::Current->CoreWindow, SwapChainPanel, DisplayProperties::LogicalDpi);

	Window::Current->CoreWindow->SizeChanged += ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &DirectXPage::OnWindowSizeChanged);
	DisplayProperties::LogicalDpiChanged += ref new DisplayPropertiesEventHandler(this, &DirectXPage::OnLogicalDpiChanged);
	m_eventToken = CompositionTarget::Rendering::add(ref new EventHandler<Object^>(this, &DirectXPage::OnRendering));

	DataContext = this;
	
	soundPlayer_->AddNamedSound("G3", 392.00);
	soundPlayer_->AddNamedSound("A4", 440.00);

	soundPlayer_->AddNamedSound("B4", 493.88);
	soundPlayer_->AddNamedSound("C4", 523.25);
	soundPlayer_->AddNamedSound("D4", 587.33);

	soundPlayer_->AddNamedSound("E4", 659.26);
	soundPlayer_->AddNamedSound("F4", 701.00);
	soundPlayer_->AddNamedSound("G4", 783.99);
	soundPlayer_->AddNamedSound("A5", 880.00);

	m_timer = ref new BasicTimer();
}

DirectXPage::~DirectXPage()
{ }

void DirectXPage::OnPointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args)
{
	m_lastPointValid = true;
	auto currentPoint = args->GetCurrentPoint(this);
	auto isOver = m_renderer->HitTest(currentPoint->Position);
	if (isOver)
	{
		auto line = m_renderer->GetLineAt(currentPoint->Position);
		if (line == nullptr) return;

		auto length = line->Length;
		auto frequency = 1 / (2 * length); 
		soundPlayer_->PlaySineWave(frequency * 111111 * 9);
	}
	else
	{
		m_renderer->StartLine(currentPoint->Position);
	}

	m_renderNeeded = true;
	args->Handled = true;
}

void DirectXPage::OnPointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args)
{
	auto currentPoint = args->GetCurrentPoint(this);
	if (currentPoint->IsInContact)
	{
		if (m_lastPointValid)
		{
			m_renderer->UpdateLinePosition(currentPoint->RawPosition);
			m_renderNeeded = true;
		}
		m_lastPoint = currentPoint->Position;
		m_lastPointValid = true;
	}
	else
	{
		m_lastPointValid = false;
	}
	args->Handled = true;
}

void DirectXPage::OnPointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args)
{
	m_lastPointValid = true;
	auto currentPoint = args->GetCurrentPoint(this);

	m_renderer->EndLine(currentPoint->Position);

	soundPlayer_->StopPlayingAllSounds();

	m_lastPointValid = false;
	m_renderNeeded = true;
	args->Handled = true;
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
	soundPlayer_->StopPlayingAllSounds();
	m_renderNeeded = true;
	e->Handled = true;
}

void GeoSound::DirectXPage::Button_PointerPressed_1(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	Grid^grid = dynamic_cast<Grid^>(sender);
	soundPlayer_->PlayNamedSound(grid->Name);
	m_renderNeeded = true;
	e->Handled = true;
}
