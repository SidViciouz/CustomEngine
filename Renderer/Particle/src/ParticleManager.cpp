#include "../Particle/header/ParticleManager.h"
#include "../Particle/header/ParticleUpdateTask.h"
#include "../Multithreading/header/TaskBatch.h"
#include "../Multithreading/header/ThreadPool.h"

namespace Renderer
{
	CParticleManager::CParticleManager() :
		mMaxParticleNum{ MAX_PARTICLE_NUM }, mMaxPoolNum{MAX_PARTICLE_MEMORY_POOL}
	{
		//create memory manager
		mParticleMemoryManager = CParticleMemoryManager::Get();

		//create vertex buffer as many as memory pool counts
		int lParticlesPerPool = mMaxParticleNum / mMaxPoolNum;
		
		mVertexBuffer = make_shared<CParticleVertexBuffer>(mMaxPoolNum, lParticlesPerPool);
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
	


	void CParticleManager::Update(float pDeltaTime, Math::SVector3 pCameraPosition)
	{
		//update all particle system by multithread
		shared_ptr<CParticleUpdateTask> lParticleUpdateTasks[MAX_PARTICLE_MEMORY_POOL];
		shared_ptr<CParticleUpdateTask> lFirstTask;

		shared_ptr<Multithreading::CTaskBatch> lTaskBatch = make_shared<Multithreading::CTaskBatch>();
		for (int lPoolIndex = 0; lPoolIndex < MAX_PARTICLE_MEMORY_POOL; ++lPoolIndex)
		{
			mVertexBuffer->ClearBuffer(lPoolIndex);

			lParticleUpdateTasks[lPoolIndex] = make_shared<CParticleUpdateTask>(mParticleSystems[lPoolIndex], pDeltaTime, pCameraPosition, mVertexBuffer);

			
			if (lFirstTask == nullptr)
				lFirstTask = lParticleUpdateTasks[lPoolIndex];
			
			else
				lTaskBatch->AddTask(lParticleUpdateTasks[lPoolIndex]);
				
		}

		Multithreading::CThreadPool::Singleton()->AddTaskBatch(lTaskBatch);

		if(lFirstTask != nullptr)
			lFirstTask->Execute();
		
		lTaskBatch->Wait();	
		
	}



	int CParticleManager::GetMaxParticleNum() const
	{
		return mMaxParticleNum;
	}



	int CParticleManager::GetMaxPoolNum() const
	{
		return mMaxPoolNum;
	}




	void CParticleManager::SetVertexBufferHandle(int pHandle)
	{
		mVertexBufferHandle = pHandle;
	}



	void CParticleManager::SetIndexBufferHandle(int pHandle)
	{
		mIndexBufferHandle = pHandle;
	}



	int CParticleManager::GetVertexBufferHandle() const
	{
		return mVertexBufferHandle;
	}



	int	CParticleManager::GetIndexBufferHandle() const
	{
		return mIndexBufferHandle;
	}



	int CParticleManager::GetVertexCount(int pPoolIndex)
	{
		return mVertexBuffer->GetVertexCount(pPoolIndex);
	}



	int	CParticleManager::GetIndexCount(int pPoolIndex)
	{
		return mVertexBuffer->GetIndexCount(pPoolIndex);
	}



	const vector<SParticleVertex>& CParticleManager::GetVertexBuffer(int pPoolIndex)
	{
		return mVertexBuffer->GetVertexBuffer(pPoolIndex);
	}



	const vector<uint16_t>& CParticleManager::GetIndexBuffer(int pPoolIndex)
	{
		return mVertexBuffer->GetIndexBuffer(pPoolIndex);
	}
}