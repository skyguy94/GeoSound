#pragma once

#include <wrl.h>
#include "DirectXBase.h"

ref class SimpleTextRenderer sealed : public DirectXBase
{
public:
	SimpleTextRenderer();
	~SimpleTextRenderer();
	virtual void CreateDeviceIndependentResources() override;
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;
	void Update(float timeTotal, float timeDelta);

	void OnManipulationUpdated(Windows::UI::Input::GestureRecognizer^ sender, Windows::UI::Input::ManipulationUpdatedEventArgs^ args);
	void OnManipulationCompleted(Windows::UI::Input::GestureRecognizer^ sender, Windows::UI::Input::ManipulationCompletedEventArgs^ args);
	void OnTapped(Windows::UI::Input::GestureRecognizer^ sender, Windows::UI::Input::TappedEventArgs^ args);

	void UpdateView(Windows::Foundation::Point deltaViewPosition);
	void SetRenderFast(bool value);
	void BackgroundColorNext();
	void BackgroundColorPrevious();

private:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_blackBrush;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_opacityBitmap;
	Microsoft::WRL::ComPtr<IDWriteTextLayout> m_textLayout;
	DWRITE_TEXT_METRICS m_textMetrics;
	Windows::Foundation::Point m_viewPosition;
	bool m_animating;
	bool m_renderNeeded;
	int m_bgColorIndex;
};
