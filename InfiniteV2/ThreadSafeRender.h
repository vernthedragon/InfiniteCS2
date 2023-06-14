
#pragma once
#include "BuildConfig.h"
#ifdef MULTITHREAD_RENDER


#include "Rendering.h"
#include <deque>
enum RenderObjectKey {
	ROKEY_GRADIENT_CIRCLE = 0
};

class RenderObject {
public:
	virtual RenderObjectKey GetKey() = 0;
	virtual int FrameCount() = 0;
};

class RenderBranchGradientCircleInternal : public RenderObject {
public:
	RenderObjectKey GetKey() {
		return RenderObjectKey::ROKEY_GRADIENT_CIRCLE;
	}
	int FrameCount() {
		return FC;
	}
	float x; float y; float radius; Col inner; Col outer; bool antialiased;
	int FC;
};

class RenderBranch {
public:
	void GradientCircle(float x, float y, float radius, Col inner, Col outer, bool antialiased = false);
	void NewFrame();
	void RenderFrame();
private:
	int FrameCount = 0;
	std::deque< RenderObject* > RenderQueue;
};
#endif