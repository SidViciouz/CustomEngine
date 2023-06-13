#include "../Particle/header/ParticleUpdateTask.h"

namespace Renderer
{
	CParticleUpdateTask::CParticleUpdateTask(vector<shared_ptr<CParticleSystem>>& pParticleSystems, float pDeltaTime, Math::SVector3 pCameraPosition, shared_ptr<CParticleVertexBuffer> pVertexBuffer) :
		mParticleSystems{pParticleSystems}, mDeltaTime{pDeltaTime}, mCameraPosition{ pCameraPosition }, mVertexBuffer{pVertexBuffer}
	{

	}



	void CParticleUpdateTask::Execute()
	{
		for (auto& lParticleSystem : mParticleSystems)
		{
			lParticleSystem->Update(mVertexBuffer, mDeltaTime, mCameraPosition);
		}
	}

}