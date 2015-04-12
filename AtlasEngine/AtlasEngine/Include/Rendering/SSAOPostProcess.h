#pragma once
#include "IRenderPostProcess.h"
#include "Rendering\ShaderProgram.h"
#include "Rendering\Uniform.h"
#include "Math\Vector2.h"

class FSSAOPostProcess : public IRenderPostProcess
{
public:
	FSSAOPostProcess();
	~FSSAOPostProcess();

	void OnPostLightingPass() override;

	void SetSampleRadius(const Vector2f& Radius);
	void SetMaxDistance(const float Distance);

private:
	FShaderProgram mSSAOShader;
	EZGL::FUniform mSampleRadius;
	EZGL::FUniform mMaxDistance;
};

