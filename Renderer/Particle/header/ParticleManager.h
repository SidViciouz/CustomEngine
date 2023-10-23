#pragma once

#include "../Common/header/Common.h"
#include "../Particle/header/ParticleMemoryManager.h"
#include "../Particle/header/ParticleSystem.h"
#include "../Particle/header/ParticleVertexBuffer.h"

namespace Renderer
{
	//manage resource about particles and render it
	class CParticleManager
	{
	public:
		static shared_ptr<CParticleManager>		Singleton();
		
		shared_ptr<CParticleSystem>				AddParticleSystem();

		shared_ptr<CParticleEmitter>			AddParticleEmitter(shared_ptr<CParticleSystem> pParticleSystem);
		
		template<typename T>
		void									SetParticleEmitterValue(shared_ptr<CParticleEmitter> pParticleEmitter,
													EParticleEmitterProperty pProperty, const T& pValue);
		
		void									Update(float pDeltaTime, Math::SVector3 pCameraPosition);

		int										GetMaxParticleNum() const;
		int										GetMaxPoolNum() const;

		void									SetVertexBufferHandle(int pHandle);
		void									SetIndexBufferHandle(int pHandle);

		int										GetVertexBufferHandle() const;
		int										GetIndexBufferHandle() const;

		int										GetVertexCount(int pPoolIndex);
		int										GetIndexCount(int pPoolIndex);

		const vector<SParticleVertex>&			GetVertexBuffer(int pPoolIndex);
		const vector<uint16_t>&					GetIndexBuffer(int pPoolIndex);

												CParticleManager();
	private:
	
		shared_ptr<CParticleMemoryManager>		mParticleMemoryManager;
		
		vector<shared_ptr<CParticleSystem>>		mParticleSystems[MAX_PARTICLE_MEMORY_POOL];

		shared_ptr<CParticleVertexBuffer>		mVertexBuffer;

		int										mMaxParticleNum;
		int										mMaxPoolNum;
		/*
		* gpu vertex buffer handle
		*/
		int										mVertexBufferHandle;
		/*
		* gpu index buffer handle
		*/
		int										mIndexBufferHandle;
	};

}
