#pragma once

#include "ManagedAudioManager.h"
#include "DirectXBase.h"

ref class WaveRenderer sealed : public DirectXBase
{
public:
	WaveRenderer(ManagedAudioManager^ audioManager);

	// DirectXBase methods.
	virtual void CreateDeviceIndependentResources() override;
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;

	// Method for updating time-dependent objects.
	void Update(float timeTotal, float timeDelta);

	// Method to change the text position based on input events.
	void UpdateTextPosition(Windows::Foundation::Point deltaTextPosition);

	// Methods to adjust the window background color.
	void BackgroundColorNext();
	void BackgroundColorPrevious();

	// Methods to save and load state in response to suspend.
	void SaveInternalState(Windows::Foundation::Collections::IPropertySet^ state);
	void LoadInternalState(Windows::Foundation::Collections::IPropertySet^ state);

private:
	ManagedAudioManager^ audioManager_;

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_blackBrush;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;
	Microsoft::WRL::ComPtr<IDWriteTextLayout> m_textLayout;
	DWRITE_TEXT_METRICS m_textMetrics;
	Windows::Foundation::Point m_textPosition;
	bool m_renderNeeded;
	int m_backgroundColorIndex;
};
