#include "pch.h"
#include "WaveRenderer.h"
#include "ManagedAudioManager.h"

using std::unique_ptr;

using namespace D2D1;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;

static const ColorF BackgroundColors[] = 
{
	ColorF::CornflowerBlue,
	ColorF::Orchid,
	ColorF::DeepPink,	ColorF::Red,
	ColorF::DarkOrange,
	ColorF::Gold,
	ColorF::GreenYellow,
	ColorF::Aqua
};

WaveRenderer::WaveRenderer(ManagedAudioManager^ audioManager) :
	m_renderNeeded(true),
	m_backgroundColorIndex(0),
	m_textPosition(0.0f, 0.0f),
	audioManager_(audioManager)

{
	audioManager_->Initialize();
}

void WaveRenderer::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();

	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextFormat(
			L"Segoe UI",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			42.0f,
			L"en-US",
			&m_textFormat
			)
		);

	DX::ThrowIfFailed(
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING)
		);
}

void WaveRenderer::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
			ColorF(ColorF::Black),
			&m_blackBrush
			)
		);

	Platform::String^ text = "Hello, DirectX!";

	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextLayout(
			text->Data(),
			text->Length(),
			m_textFormat.Get(),
			700, // maxWidth.
			1000, // maxHeight.
			&m_textLayout
			)
		);

	DX::ThrowIfFailed(
		m_textLayout->GetMetrics(&m_textMetrics)
		);
}

void WaveRenderer::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();

	// Add code to create window size dependent objects here.
}

void WaveRenderer::Update(float timeTotal, float timeDelta)
{
	(void) timeTotal; // Unused parameter.
	(void) timeDelta; // Unused parameter.

	// Add code to update time dependent objects here.
}

void WaveRenderer::Render()
{
	m_d2dContext->BeginDraw();

	m_d2dContext->Clear(ColorF(BackgroundColors[m_backgroundColorIndex]));

	D2D_POINT_2F pt1, pt2;
 
	auto graph = audioManager_->GetAudioGraph();

	pt1.x = 0;
	pt1.y = 0;
	int x = 0;
	for(auto i : graph)
	{
		pt2.x = x++;
		pt2.y = i + 250;
 
		this->m_d2dContext->DrawLine(pt1, pt2, this->m_blackBrush.Get(), 2);
 
		pt1 = pt2;
	}

	pt1.x = 100;
	pt1.y = 0;
	pt2.x = m_textPosition.X;
	pt2.y = m_textPosition.Y;
	this->m_d2dContext->DrawLine(pt1, pt2, this->m_blackBrush.Get(), 8);
	HRESULT hr = m_d2dContext->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}

	m_renderNeeded = false;
}

void WaveRenderer::UpdateTextPosition(Point deltaTextPosition)
{
	m_textPosition.X += deltaTextPosition.X;
	m_textPosition.Y += deltaTextPosition.Y;
}

void WaveRenderer::BackgroundColorNext()
{
	m_backgroundColorIndex++;
	if (m_backgroundColorIndex >= ARRAYSIZE(BackgroundColors))
	{
		m_backgroundColorIndex = 0;
	}
}

void WaveRenderer::BackgroundColorPrevious()
{
	m_backgroundColorIndex--;
	if (m_backgroundColorIndex < 0)
	{
		m_backgroundColorIndex = ARRAYSIZE(BackgroundColors) - 1;
	}
}

void WaveRenderer::SaveInternalState(IPropertySet^ state)
{
	if (state->HasKey("m_backgroundColorIndex"))
	{
		state->Remove("m_backgroundColorIndex");
	}
	if (state->HasKey("m_textPosition"))
	{
		state->Remove("m_textPosition");
	}
	state->Insert("m_backgroundColorIndex", PropertyValue::CreateInt32(m_backgroundColorIndex));
	state->Insert("m_textPosition", PropertyValue::CreatePoint(m_textPosition));
}

void WaveRenderer::LoadInternalState(IPropertySet^ state)
{
	if (state->HasKey("m_backgroundColorIndex") && state->HasKey("m_textPosition"))
	{
		m_backgroundColorIndex = safe_cast<IPropertyValue^>(state->Lookup("m_backgroundColorIndex"))->GetInt32();
		m_textPosition = safe_cast<IPropertyValue^>(state->Lookup("m_textPosition"))->GetPoint();
	}
}
