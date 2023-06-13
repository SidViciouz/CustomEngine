#include "../Particle/header/ParticleEmitter.h"
#include "../Particle/header/ParticleVertexBuffer.h"
#include "../Particle/header/ParticleMemoryManager.h"

namespace Renderer
{
	CParticleEmitter::CParticleEmitter(int pMemoryPoolIndex) :
		mMemoryPoolIndex{pMemoryPoolIndex}
	{

	}



	void CParticleEmitter::Reset()
	{

	}



	int	CParticleEmitter::GetMemoryPoolIndex() const
	{
		return mMemoryPoolIndex;
	}

	
	
	template<typename T>
	void CParticleEmitter::SetProperty(EParticleEmitterProperty pProperty, const T& pValue)
	{
		printf("you can not set the property with [%s] type.\n", typeid(T).name());
	}
	


	void CParticleEmitter::Update(shared_ptr<CParticleVertexBuffer> pVertexBuffer, const float& pDeltaTime, const Math::SVector3& pCameraDirection, const Math::SVector3& pCameraPosition)
	{
		mCurrentTime += pDeltaTime;

		if (mDuration == 0)
		{
			mCurrentTime = 0;
		}
		else if (mCurrentTime > mDuration)
		{
			mActivated = false;
		}


		if (mActivated)
		{
			float lEmissionFloat = mEmissionRate* pDeltaTime;

			int lEmissionNum = (int)lEmissionFloat;

			mEmissionUnderZero += lEmissionFloat - (float)lEmissionNum;

			if (mEmissionUnderZero >= 1.0f)
			{
				mEmissionUnderZero -= 1.0f;
				++lEmissionNum;
			}


			for (int lNewParticleIndex = 0; lNewParticleIndex < lEmissionNum; ++lNewParticleIndex)
			{
				shared_ptr<CParticle> lParticle = CParticleMemoryManager::Get()->GetParticle(mMemoryPoolIndex);

				if (lParticle == nullptr)
					break;

				CParticleMemoryManager::Get()->GetRandomValue(mMemoryPoolIndex, -3.0f, 10.0f);

				mParticles.push_back(lParticle);

			}

		}



		//for(int i=0; i<6; ++i)
		//pVertexBuffer->AddParticleVertex();
	}



	template<>
	void CParticleEmitter::SetProperty<Math::SVector3>(EParticleEmitterProperty pProperty, const Math::SVector3& pValue)
	{
		switch (pProperty)
		{
		case EParticleEmitterProperty::eAcceleration:
			mAcceleration = pValue;
			break;

		case EParticleEmitterProperty::ePosition:
			mPosition = pValue;
			break;

		case EParticleEmitterProperty::eVelocity:
			mVelocity = pValue;
			break;

		default:
			printf("you can not set the property with [%s] type", typeid(Math::SVector3).name());
			break;
		}
	}



	template<>
	void CParticleEmitter::SetProperty<Math::SVector2>(EParticleEmitterProperty pProperty, const Math::SVector2& pValue)
	{
		switch (pProperty)
		{
		case EParticleEmitterProperty::eBottomLeft:
			mBottomLeft = pValue;
			break;

		case EParticleEmitterProperty::eTopRight:
			mTopRight = pValue;
			break;

		case EParticleEmitterProperty::eScale:
			mScale = pValue;
			break;

		default:
			printf("you can not set the property with [%s] type", typeid(Math::SVector2).name());
			break;
		}
	}



	template<>
	void CParticleEmitter::SetProperty<float>(EParticleEmitterProperty pProperty, const float& pValue)
	{
		switch (pProperty)
		{
		case EParticleEmitterProperty::eAngle:
			mAngle = pValue;
			break;

		case EParticleEmitterProperty::eAngularAccelration:
			mAngularAcceleration = pValue;
			break;

		case EParticleEmitterProperty::eAngularVelocity:
			mAngularVelocity = pValue;
			break;

		case EParticleEmitterProperty::eCurrentFrame:
			mCurrentFrame = pValue;
			break;

		case EParticleEmitterProperty::eCurrentTime:
			mCurrentTime = pValue;
			break;

		case EParticleEmitterProperty::eDuration:
			mDuration = pValue;
			break;

		case EParticleEmitterProperty::eEmissionRate:
			mEmissionRate = pValue;
			break;

		default:
			printf("you can not set the property with [%s] type", typeid(float).name());
			break;
		}
	}



	template<>
	void CParticleEmitter::SetProperty<int>(EParticleEmitterProperty pProperty, const int& pValue)
	{
		switch (pProperty)
		{
		case EParticleEmitterProperty::eNumTextureColumn:
			mNumTextureColumn = pValue;
			break;

		case EParticleEmitterProperty::eNumTextureRow:
			mNumTextureRow = pValue;
			break;

		default:
			printf("you can not set the property with [%s] type", typeid(int).name());
			break;
		}
	}
	
}