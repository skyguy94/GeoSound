#pragma once

#include "AudioManager.h"
#include "DirectXBase.h"

public ref class WaveRenderer sealed : public DirectXBase
{
public:
	WaveRenderer(AudioManager *audioManager);

	virtual void CreateDeviceIndependentResources() override;
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;

	void StartLine(Windows::Foundation::Point position);
	void UpdateLinePosition(Windows::Foundation::Point position);
	void EndLine(Windows::Foundation::Point position);

	bool HitTest(Windows::Foundation::Point position);

	void Update(float timeTotal, float timeDelta);

	void SaveInternalState(Windows::Foundation::Collections::IPropertySet^ state);
	void LoadInternalState(Windows::Foundation::Collections::IPropertySet^ state);

private:
	AudioManager *audioManager_;

	std::list<Windows::UI::Xaml::Shapes::Line^> lines_;
	Windows::UI::Xaml::Shapes::Line ^current_;

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> blackBrush_;
	Windows::Foundation::Point position_;
	bool renderNeeded_;
};
