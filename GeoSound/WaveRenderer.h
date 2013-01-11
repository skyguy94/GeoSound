#pragma once

#include "AudioLine.h"
#include "SoundPlayer.h"
#include "DirectXBase.h"

ref class ManagedAudioLine
{
private:
	double length_;

public:
	property double Length
	{
		virtual double get() { return length_; };
		virtual void set(double value){ length_ = value; };
	}
};

private ref class WaveRenderer sealed : public DirectXBase
{
public:
	WaveRenderer(SoundPlayer^ soundPlayer);

	virtual void CreateDeviceIndependentResources() override;
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;

	void StartLine(Windows::Foundation::Point position);
	void UpdateLinePosition(Windows::Foundation::Point position);
	void EndLine(Windows::Foundation::Point position);

	bool HitTest(Windows::Foundation::Point position);
	ManagedAudioLine^ GetLineAt(Windows::Foundation::Point position);

	void Update(float timeTotal, float timeDelta);

	void SaveInternalState(Windows::Foundation::Collections::IPropertySet^ state);
	void LoadInternalState(Windows::Foundation::Collections::IPropertySet^ state);

private:
	SoundPlayer^ soundPlayer_;

	std::list<std::unique_ptr<AudioLine>> lines_;
	AudioLine* current_;

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> blackBrush_;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> yellowBrush_;
	Windows::Foundation::Point position_;
	bool renderNeeded_;
};
