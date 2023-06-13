#include "../Particle/header/ParticleMemoryManager.h"

namespace Renderer
{
	shared_ptr<CParticleMemoryManager>	CParticleMemoryManager::mThis = nullptr;

	CParticleMemoryManager::CParticleMemoryManager(int pNumPools, int pNumParticles, int pNumEmitters, int pNumParticleSystems)
		: mNumPools{pNumPools}, mMaxParticleNum{ pNumParticles }, mMaxEmitterNum{ pNumEmitters }, mMaxParticleSystemNum{pNumParticleSystems}
	{
		int lParticlesPerPool = mMaxParticleNum / mNumPools;
		int lEmittersPerPool = mMaxEmitterNum / mNumPools;
		int lParticleSystemsPerPool = mMaxParticleSystemNum / mNumPools;

		for (int lPoolIndex = 0; lPoolIndex < pNumPools; ++lPoolIndex)
		{
			mParticleMemoryPools[lPoolIndex] = make_shared<CParticleMemoryPool>(lPoolIndex, lParticlesPerPool, lEmittersPerPool, lParticleSystemsPerPool);
			mRandomGenerator[lPoolIndex] = make_shared<mt19937>(time(nullptr));
		}
	}



	shared_ptr<CParticleMemoryManager> CParticleMemoryManager::Get()
	{
		if (mThis == nullptr)
		{
			mThis = make_shared<CParticleMemoryManager>(MAX_PARTICLE_MEMORY_POOL, MAX_PARTICLE_NUM, MAX_PARTICLE_EMITTER_NUM, MAX_PARTICLE_SYSTEM_NUM);
		}

		return mThis;
	}



	shared_ptr<CParticleSystem> CParticleMemoryManager::GetParticleSystem()
	{
		int lMemoryPoolIndex = GetNextMemoryPoolIndex();

		return mParticleMemoryPools[lMemoryPoolIndex]->GetParticleSystem();
	}



	void CParticleMemoryManager::ReleaseParticleSystem(shared_ptr<CParticleSystem> pParticleSystem)
	{
		int lMemoryPoolIndex = pParticleSystem->GetMemoryPoolIndex();

		mParticleMemoryPools[lMemoryPoolIndex]->ReleaseParticleSystem(pParticleSystem);
	}



	shared_ptr<CParticleEmitter> CParticleMemoryManager::GetParticleEmitter(int pMemoryPoolIndex)
	{
		return mParticleMemoryPools[pMemoryPoolIndex]->GetParticleEmitter();
	}



	void CParticleMemoryManager::ReleaseParticleEmitter(shared_ptr<CParticleEmitter> pParticleEmitter)
	{
		int lMemoryPoolIndex = pParticleEmitter->GetMemoryPoolIndex();

		mParticleMemoryPools[lMemoryPoolIndex]->ReleaseParticleEmitter(pParticleEmitter);
	}



	shared_ptr<CParticle> CParticleMemoryManager::GetParticle(int pMemoryPoolIndex)
	{
		return mParticleMemoryPools[pMemoryPoolIndex]->GetParticle();
	}



	void CParticleMemoryManager::ReleaseParticle(shared_ptr<CParticle> pParticle)
	{
		int lMemoryPoolIndex = pParticle->GetMemoryPoolIndex();

		mParticleMemoryPools[lMemoryPoolIndex]->ReleaseParticle(pParticle);
	}



	template<typename T>
	T CParticleMemoryManager::GetRandomValue(int pMemoryPoolIndex, const T& pMin, const T& pMax)
	{
		printf("can not generate randome value of [%s] type.\n", typeid(T).name());

		return T{};
	}



	template<>
	int CParticleMemoryManager::GetRandomValue<int>(int pMemoryPoolIndex, const int& pMin, const int& pMax)
	{
		uniform_int_distribution<int> lDistribution(pMin, pMax);

		return lDistribution(*mRandomGenerator[pMemoryPoolIndex]);
	}



	template<>
	float CParticleMemoryManager::GetRandomValue<float>(int pMemoryPoolIndex, const float& pMin, const float& pMax)
	{
		uniform_real_distribution<float> lDistribution(pMin, pMax);

		return lDistribution(*mRandomGenerator[pMemoryPoolIndex]);
	}



	template<>
	Math::SVector2 CParticleMemoryManager::GetRandomValue<Math::SVector2>(int pMemoryPoolIndex, const Math::SVector2& pMin, const Math::SVector2& pMax)
	{
		Math::SVector2 lVector;

		for (int i = 0; i < 2; ++i)
		{
			uniform_real_distribution<float> lDistribution(pMin.mElement[i], pMax.mElement[i]);
			lVector.mElement[i] = lDistribution(*mRandomGenerator[pMemoryPoolIndex]);
		}

		return lVector;
	}



	template<>
	Math::SVector3 CParticleMemoryManager::GetRandomValue<Math::SVector3>(int pMemoryPoolIndex, const Math::SVector3& pMin, const Math::SVector3& pMax)
	{
		Math::SVector3 lVector;

		for (int i = 0; i < 3; ++i)
		{
			uniform_real_distribution<float> lDistribution(pMin.mElement[i], pMax.mElement[i]);
			lVector.mElement[i] = lDistribution(*mRandomGenerator[pMemoryPoolIndex]);
		}

		return lVector;
	}



	int CParticleMemoryManager::GetNextMemoryPoolIndex()
	{
		int lMinIndex = -1;
		int lMinParticleNum = INT_MAX;

		for (int lPoolIndex = 0; lPoolIndex < mNumPools; ++lPoolIndex)
		{
			if (!mParticleMemoryPools[lPoolIndex]->IsAvailable())
				continue;

			int lParticleNum = mParticleMemoryPools[lPoolIndex]->GetParticleNum();
			if (lParticleNum < lMinParticleNum)
			{
				lMinIndex = lPoolIndex;
				lMinParticleNum = lParticleNum;
			}
		}

		return lMinIndex;
	}
}