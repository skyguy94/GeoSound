#include "pch.h"
#include "SimpleTextRenderer.h"
#include <DirectXMath.h>

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace DirectX;

static const D2D1::ColorF sc_bgColors[] = 
{
	D2D1::ColorF::CornflowerBlue,
	D2D1::ColorF::Orchid,
	D2D1::ColorF::DeepPink,
	D2D1::ColorF::Red,
	D2D1::ColorF::DarkOrange,
	D2D1::ColorF::Gold,
	D2D1::ColorF::GreenYellow,
	D2D1::ColorF::Aqua
};

SimpleTextRenderer::SimpleTextRenderer() :
	m_animating(false),
	m_renderNeeded(true),
	m_bgColorIndex(0),
	m_viewPosition(0.0f, 0.0f)
{
}

SimpleTextRenderer::~SimpleTextRenderer()
{
}

void SimpleTextRenderer::CreateDeviceIndependentResources()
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

void SimpleTextRenderer::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black),
			&m_blackBrush
			)
		);

	Platform::String^ text = "Hello, DirectX!";

	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextLayout(
			text->Data(),
			text->Length(),
			m_textFormat.Get(),
			700, // maxWidth
			1000, // maxHeight
			&m_textLayout
			)
		);

	m_textLayout->GetMetrics(&m_textMetrics);

	D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET,
		D2D1::PixelFormat(DXGI_FORMAT_A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);

	D2D1_SIZE_U opacityBitmapSize = D2D1::SizeU(
		static_cast<UINT32>(m_textMetrics.widthIncludingTrailingWhitespace),
		static_cast<UINT32>(m_textMetrics.height)
		);

	DX::ThrowIfFailed(
		m_d2dContext->CreateBitmap(
			opacityBitmapSize,
			nullptr,
			0,
			&properties,
			&m_opacityBitmap
			)
		);

	m_d2dContext->SetTarget(m_opacityBitmap.Get());
	m_d2dContext->BeginDraw();

	m_d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::Black, 0.0f));

	m_d2dContext->DrawTextLayout(
		D2D1::Point2F(0.0f, 0.0f),
		m_textLayout.Get(),
		m_blackBrush.Get(),
		D2D1_DRAW_TEXT_OPTIONS_NO_SNAP
		);

	DX::ThrowIfFailed(
		m_d2dContext->EndDraw()
		);

	m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());
}

void SimpleTextRenderer::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();
}

void SimpleTextRenderer::Update(float timeTotal, float timeDelta)
{
	(void) timeTotal;	// Unused parameter
	(void) timeDelta;	// Unused parameter
}

void SimpleTextRenderer::Render()
{
	D2D1_SIZE_F size = m_d2dContext->GetSize();

	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(
		size.width/2 - m_textMetrics.widthIncludingTrailingWhitespace/2 + m_viewPosition.X,
		size.height/2 - m_textMetrics.height/2 + m_viewPosition.Y
		);

	m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	m_d2dContext->BeginDraw();

	m_d2dContext->Clear(D2D1::ColorF(sc_bgColors[m_bgColorIndex]));

	m_d2dContext->SetTransform(translation);

	if (!m_animating)
	{
		// Use default (high quality) text rendering when the text is not being animated.
		m_d2dContext->DrawTextLayout(
			D2D1::Point2F(0.0f, 0.0f),
			m_textLayout.Get(),
			m_blackBrush.Get(),
			D2D1_DRAW_TEXT_OPTIONS_NO_SNAP
			);
	}
	else
	{
		// Use fast opacity mask text rendering when the text is being animated.
		m_d2dContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		m_d2dContext->FillOpacityMask(m_opacityBitmap.Get(), m_blackBrush.Get(), D2D1_OPACITY_MASK_CONTENT_GRAPHICS);
		m_d2dContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	}

	HRESULT hr = m_d2dContext->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		m_d2dContext->SetTarget(nullptr);
		m_d2dTargetBitmap = nullptr;
		CreateWindowSizeDependentResources();
	}
	else
	{
		DX::ThrowIfFailed(hr);
	}

	m_renderNeeded = false;
}

void SimpleTextRenderer::UpdateView(Point deltaViewPosition)
{
	m_viewPosition.X += deltaViewPosition.X;
	m_viewPosition.Y += deltaViewPosition.Y;
}

void SimpleTextRenderer::SetRenderFast(bool value)
{
	m_animating = value;
}

void SimpleTextRenderer::BackgroundColorNext()
{
	m_bgColorIndex++;
	if (m_bgColorIndex >= ARRAYSIZE(sc_bgColors))
		m_bgColorIndex = 0;
}

void SimpleTextRenderer::BackgroundColorPrevious()
{
	m_bgColorIndex--;
	if (m_bgColorIndex < 0)
		m_bgColorIndex = ARRAYSIZE(sc_bgColors) - 1;
}