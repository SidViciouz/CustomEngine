#include "../Particle/header/ParticleManager.h"

namespace Renderer
{
	CParticleManager::CParticleManager()
	{
		//create memory manager
		mParticleMemoryManager = CParticleMemoryManager::Get();

		//create vertex buffer as many as memory pool counts
		int lParticlesPerPool = MAX_PARTICLE_NUM / MAX_PARTICLE_MEMORY_POOL;
		int lVertexNumPerPool = lParticlesPerPool * 6;
		for (int lPoolIndex = 0; lPoolIndex < MAX_PARTICLE_MEMORY_POOL; ++lPoolIndex)
		{
			mVertexBuffer[lPoolIndex] = make_shared<CParticleVertexBuffer>(lVertexNumPerPool);
		}
	}


	
	
	shared_ptr<CParticleSystem> CParticleManager::AddParticleSystem()
	{
		shared_ptr<CParticleSystem> lParticleSystem = mParticleMemoryManager->GetParticleSystem();

		mParticleSystems[lParticleSystem->GetMemoryPoolIndex()].push_back(lParticleSystem);

		return lParticleSystem;
	}



	shared_ptr<CParticleEmitter> CParticleManager::AddParticleEmitter(shared_ptr<CParticleSystem> pParticleSystem)
	{
		int lMemoryPoolIndex = pParticleSystem->GetMemoryPoolIndex();
		shared_ptr<CParticleEmitter> lParticleEmitter = mParticleMemoryManager->GetParticleEmitter(lMemoryPoolIndex);
		
		pParticleSystem->AddParticleEmitter(lParticleEmitter);

		return lParticleEmitter;
	}



	template<typename T>
	void CParticleManager::SetParticleEmitterValue(shared_ptr<CParticleEmitter> pParticleEmitter, EParticleEmitterProperty pProperty, const T& pValue)
	{
		printf("you can not set the property with [%s] type.\n",typeid(T).name());
		return;
	}
	

	template<>
	void CParticleManager::SetParticleEmitterValue<Math::SVector3>(shared_ptr<CParticleEmitter> pParticleEmitter, EParticleEmitterProperty pProperty, const Math::SVector3& pValue)
	{
		pParticleEmitter->SetProperty(pProperty, pValue);
	}



	template<>
	void CParticleManager::SetParticleEmitterValue<Math::SVector2>(shared_ptr<CParticleEmitter> pParticleEmitter, EParticleEmitterProperty pProperty, const Math::SVector2& pValue)
	{
		pParticleEmitter->SetProperty(pProperty, pValue);
	}



	template<>
	void CParticleManager::SetParticleEmitterValue<float>(shared_ptr<CParticleEmitter> pParticleEmitter, EParticleEmitterProperty pProperty, const float& pValue)
	{
		pParticleEmitter->SetProperty(pProperty, pValue);
	}


	template<>
	void CParticleManager::SetParticleEmitterValue<int>(shared_ptr<CParticleEmitter> pParticleEmitter, EParticleEmitterProperty pProperty, const int& pValue)
	{
		pParticleEmitter->SetProperty(pProperty, pValue);
	}
	
}