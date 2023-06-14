
#include "BuildConfig.h"
#ifdef MULTITHREAD_RENDER

#include "Rendering.h"





void RenderBranch::GradientCircle(float x, float y, float radius, Col inner, Col outer, bool antialiased) {
	RenderBranchGradientCircleInternal* Object = new RenderBranchGradientCircleInternal();
	Object->x = x;
	Object->y = y;
	Object->radius = radius;
	Object->inner = inner;
	Object->outer = outer;
	Object->antialiased = antialiased;
	Object->FC = FrameCount;
	RenderQueue.emplace_front(reinterpret_cast<RenderObject*>(Object));
}
void RenderBranch::NewFrame() {
	FrameCount++;
	

}
void RenderBranch::RenderFrame() {

	if (RenderQueue.empty())
		return;

	for (auto& Object : RenderQueue) {

		if (Object->FrameCount() != FrameCount)
			continue;


		auto Key = Object->GetKey();
		if (Key == RenderObjectKey::ROKEY_GRADIENT_CIRCLE) {
			RenderBranchGradientCircleInternal* Circle = reinterpret_cast<RenderBranchGradientCircleInternal*>(Object);
			Render::GradientCircle(Circle->x, Circle->y, Circle->radius, Circle->inner, Circle->outer, Circle->antialiased);
			Client->Log("Rendering Circle");
		}
	}
	
	while (RenderQueue.back()->FrameCount() != FrameCount)
		RenderQueue.pop_back();
}
#endif