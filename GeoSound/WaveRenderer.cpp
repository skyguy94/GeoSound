#include "pch.h"
#include "WaveRenderer.h"
#include "AudioManager.h"

using std::unique_ptr;
using std::abs;

using namespace D2D1;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Shapes;

WaveRenderer::WaveRenderer(AudioManager* audioManager) :
	renderNeeded_(true),
	position_(0.0f, 0.0f),
	audioManager_(audioManager),
	current_(nullptr)

{
	audioManager_->Initialize();
}

void WaveRenderer::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();
}

void WaveRenderer::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();

	DX::ThrowIfFailed(m_d2dContext->CreateSolidColorBrush(ColorF(ColorF::Black), &blackBrush_));}

void WaveRenderer::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();
}

bool WaveRenderer::HitTest(Windows::Foundation::Point hitTest)
{
	auto epsilon = 10;
	for (auto line : lines_)
	{
		auto slope = (line->Y2 - line->Y1)/(line->X2 - line->X1);
		auto b = line->Y1 - slope*line->X1;
		auto y3 = slope * hitTest.X + b;
		return abs(hitTest.Y - y3) <= epsilon;
	}

	return false;
}

void WaveRenderer::StartLine(Windows::Foundation::Point position)
{
	current_ = ref new Line();
	current_->X1 = position.X;
	current_->Y1 = position.Y;
	current_->X2 = position.X;
	current_->X2 = position.Y;
	lines_.push_back(current_);
}

void WaveRenderer::UpdateLinePosition(Windows::Foundation::Point position)
{
	if (current_ == nullptr) return

	current_->X2  = position.X;
	current_->Y2 = position.Y;
}

void WaveRenderer::EndLine(Windows::Foundation::Point position)
{
	current_->X2  = position.X;
	current_->Y2 = position.Y;
	current_ = nullptr;
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

	m_d2dContext->Clear(ColorF(ColorF::CornflowerBlue));

	D2D_POINT_2F pt1, pt2;
 
	auto graph = audioManager_->GetAudioGraph();

	pt1.x = 0;
	pt1.y = 0;
	int x = 0;
	for(auto i : graph)
	{
		pt2.x = x++;
		pt2.y = i + 250;
 
		this->m_d2dContext->DrawLine(pt1, pt2, this->blackBrush_.Get(), 2);
 
		pt1 = pt2;
	}

	for (auto line : lines_)
	{
		pt1.x = line->X1;
		pt1.y = line->Y1;
		pt2.x = line->X2;
		pt2.y = line->Y2;
		this->m_d2dContext->DrawLine(pt1, pt2, this->blackBrush_.Get(), 8);
	}

	HRESULT hr = m_d2dContext->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}

	renderNeeded_ = false;
}

void WaveRenderer::SaveInternalState(IPropertySet^ state)
{
	//if (state->HasKey("m_backgroundColorIndex"))
	//{
	//	state->Remove("m_backgroundColorIndex");
	//}
	//if (state->HasKey("m_textPosition"))
	//{
	//	state->Remove("m_textPosition");
	//}
	//state->Insert("m_backgroundColorIndex", PropertyValue::CreateInt32(m_backgroundColorIndex));
	//state->Insert("m_textPosition", PropertyValue::CreatePoint(m_textPosition));
}

void WaveRenderer::LoadInternalState(IPropertySet^ state)
{
	//if (state->HasKey("m_backgroundColorIndex") && state->HasKey("m_textPosition"))
	//{
	//	m_backgroundColorIndex = safe_cast<IPropertyValue^>(state->Lookup("m_backgroundColorIndex"))->GetInt32();
	//	m_textPosition = safe_cast<IPropertyValue^>(state->Lookup("m_textPosition"))->GetPoint();
	//}
}
