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

WaveRenderer::WaveRenderer(SoundPlayer^ soundPlayer) :
	renderNeeded_(true),
	position_(0.0f, 0.0f),
	soundPlayer_(soundPlayer),
	current_(nullptr)

{}

void WaveRenderer::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();
}

void WaveRenderer::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();

	DX::ThrowIfFailed(m_d2dContext->CreateSolidColorBrush(ColorF(ColorF::Black), &blackBrush_));
	DX::ThrowIfFailed(m_d2dContext->CreateSolidColorBrush(ColorF(ColorF::Yellow), &yellowBrush_));
}

void WaveRenderer::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();
}

bool WaveRenderer::HitTest(Windows::Foundation::Point point)
{
	auto line = GetLineAt(point);
	return line != nullptr;
}

ManagedAudioLine^ WaveRenderer::GetLineAt(Windows::Foundation::Point point)
{
	auto epsilon = 10;
	for (auto& line : lines_)
	{
		AudioLine tmp(point);
		tmp.end = line->start;

		auto lineSlope = line->ComputeSlope();
		auto tmpSlope = tmp.ComputeSlope();
		auto result = abs(lineSlope - tmpSlope) <= epsilon;
		if (result)
		{
			auto managedLine = ref new ManagedAudioLine();
			managedLine->Length = line->Length();
			return managedLine;
		}
	}

	return nullptr;
}

void WaveRenderer::StartLine(Windows::Foundation::Point position)
{
	current_ = new AudioLine(position);
}

void WaveRenderer::UpdateLinePosition(Windows::Foundation::Point position)
{
	if (current_ == nullptr) return;

	current_->end.x  = position.X;
	current_->end.y = position.Y;

}

void WaveRenderer::EndLine(Windows::Foundation::Point position)
{
	if (current_ == nullptr) return;

	//Short lines should be cancelled.
	if (current_->Length() > 30)
	{
		current_->end.x  = position.X;
		current_->end.y = position.Y;
		lines_.push_back(unique_ptr<AudioLine>(current_));
	}
	else
	{
		delete current_;
	}

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
 
	auto graph = soundPlayer_->GetAudioGraph();

	pt1.x = 0;
	pt1.y = 0;
	int x = 0;
	for(auto i : graph)
	{
		pt2.x = x++;
		pt2.y = (i/10 + 250);
 
		this->m_d2dContext->DrawLine(pt1, pt2, this->blackBrush_.Get(), 2);
 
		pt1 = pt2;
	}

	for (auto& line : lines_)
	{
		this->m_d2dContext->DrawLine(line->start, line->end, this->blackBrush_.Get(), 8);

		auto slope = (line->end.y - line->start.y)/(line->end.x - line->start.x);
		auto b = line->end.y - slope*line->end.x;
		auto y3 = slope * line->end.y + b;
		auto x3 = y3/slope - b;
		D2D1_POINT_2F p3;
		p3.x = x3;
		p3.y = y3;
	}

	if (current_ != nullptr)
	{
		this->m_d2dContext->DrawLine(current_->start, current_->end, this->yellowBrush_.Get(), 8);
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
