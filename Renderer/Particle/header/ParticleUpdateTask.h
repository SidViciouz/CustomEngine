#pragma once

#include "../Common/header/Common.h"
#include "../Particle/header/ParticleSystem.h"
#include "../Multithreading/header/Task.h"

namespace Renderer
{
	class CParticleUpdateTask : public Multithreading::CTask
	{
	public:
												CParticleUpdateTask(vector<shared_ptr<CParticleSystem>>& pParticleSystems,float pDeltaTime, Math::SVector3 pCameraPosition, shared_ptr<CParticleVertexBuffer> pVertexBuffer);

		virtual void							Execute() override;

	private:
		vector<shared_ptr<CParticleSystem>>&	mParticleSystems;
		float									mDeltaTime;
		const Math::SVector3&					mCameraPosition;
		shared_ptr<CParticleVertexBuffer>		mVertexBuffer;
	};
}
