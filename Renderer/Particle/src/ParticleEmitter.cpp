#include "../Particle/header/ParticleEmitter.h"
#include "../Particle/header/ParticleVertexBuffer.h"
#include "../Particle/header/ParticleMemoryManager.h"
#include "../../Maths/header/Matrix3.h"

namespace Renderer
{
	CParticleEmitter::CParticleEmitter(int pMemoryPoolIndex) :
		mMemoryPoolIndex{pMemoryPoolIndex}
	{

	}



	void CParticleEmitter::Reset()
	{
		for (int i = 0; i < 3; ++i)
		{
			mPosition.mElement[i] = 0;
			mVelocity.mElement[i] = 0;
			mAcceleration.mElement[i] = 0;

			mPositionVariance.mElement[i] = 0;
			mVelocityVariance.mElement[i] = 0;
			mAccelerationVariance.mElement[i] = 0;
		}

		for (int i = 0; i < 2; ++i)
		{
			mScale.mElement[i] = 0;
			mScaleVariance.mElement[i] = 0;
		}

		mAngle = 0;
		mAngularVelocity = 0;
		mAngularAcceleration = 0;
		mDuration = 0;
		mParticleDuration = 0;
		mCurrentTime = 0;
		mCurrentFrame = 0;
		mFrameRate = 0;
		mNumTextureRow = 1;
		mNumTextureColumn = 1;
		mEmissionRate = 0;

		mAngleVariance = 0;
		mAngularVelocityVariance = 0;
		mAngularAccelerationVariance = 0;
		mParticleDurationVariance = 0;
		mFrameVariance = 0;

		mEmissionUnderZero = 0;
		mActivated = true;
	}



	int	CParticleEmitter::GetMemoryPoolIndex() const
	{
		return mMemoryPoolIndex;
	}

	
	
	void CParticleEmitter::Update(shared_ptr<CParticleVertexBuffer> pVertexBuffer, const float& pDeltaTime, const Math::SVector3& pCameraPosition)
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
			//add new particles to emit
			float lEmissionFloat = mEmissionRate* pDeltaTime;

			int lEmissionNum = (int)lEmissionFloat;

			mEmissionUnderZero += lEmissionFloat - (float)lEmissionNum;

			if (mEmissionUnderZero >= 1.0f)
			{
				mEmissionUnderZero -= 1.0f;
				++lEmissionNum;
			}

			shared_ptr<CParticleMemoryManager> lMemoryManager = CParticleMemoryManager::Get();

			for (int lNewParticleIndex = 0; lNewParticleIndex < lEmissionNum; ++lNewParticleIndex)
			{
				shared_ptr<CParticle> lParticle = lMemoryManager->GetParticle(mMemoryPoolIndex);

				if (lParticle == nullptr)
					break;

				lParticle->mPosition = lMemoryManager->GetRandomValue(mMemoryPoolIndex,mPositionVariance*-1.0f, mPositionVariance) + mPosition;
				lParticle->mScale = lMemoryManager->GetRandomValue(mMemoryPoolIndex, mScaleVariance * -1.0f, mScaleVariance) + mScale;
				lParticle->mAngle = lMemoryManager->GetRandomValue(mMemoryPoolIndex, mAngleVariance * -1.0f, mAngleVariance) + mAngle;
				lParticle->mVelocity = lMemoryManager->GetRandomValue(mMemoryPoolIndex, mVelocityVariance * -1.0f, mVelocityVariance) + mVelocity;
				lParticle->mAcceleration = lMemoryManager->GetRandomValue(mMemoryPoolIndex, mAccelerationVariance * -1.0f, mAccelerationVariance) + mAcceleration;
				lParticle->mAngularVelocity = lMemoryManager->GetRandomValue(mMemoryPoolIndex, mAngularVelocityVariance * -1.0f, mAngularVelocityVariance) + mAngularVelocity;
				lParticle->mAngularAcceleration = lMemoryManager->GetRandomValue(mMemoryPoolIndex, mAngularAccelerationVariance * -1.0f, mAngularAccelerationVariance) + mAngularAcceleration;
				lParticle->mDuration = lMemoryManager->GetRandomValue(mMemoryPoolIndex, mParticleDurationVariance * -1.0f, mParticleDurationVariance) + mParticleDuration;
				lParticle->mCurrentFrame = lMemoryManager->GetRandomValue(mMemoryPoolIndex, 0.0f, mFrameVariance) + mCurrentFrame;
				lParticle->mNumTextureRow = mNumTextureRow;
				lParticle->mNumTextureColumn = mNumTextureColumn;
				lParticle->mFrameRate = mFrameRate;

				mParticles.push_back(lParticle);
			}

			//remove particle that exceed its duration 
			//else update and add it to vertex buffer

			Math::SVector3 lFront = (mPosition - pCameraPosition).Normalize();
			Math::SVector3 lUp(0, 1, 0);
			Math::SVector3 lRight = Math::SVector3::Cross(lUp, lFront).Normalize();
			lUp = Math::SVector3::Cross(lFront, lRight).Normalize();

			Math::SMatrix3 lRotationMatrix;
			lRotationMatrix.SetColumn(0, lRight);
			lRotationMatrix.SetColumn(1, lUp);
			lRotationMatrix.SetColumn(2, lFront);

			for (int lParticleIndex = 0; lParticleIndex < mParticles.size(); ++lParticleIndex)
			{
				if (mParticles[lParticleIndex]->mCurrentTime > mParticles[lParticleIndex]->mDuration)
				{
					lMemoryManager->ReleaseParticle(mParticles[lParticleIndex]);
					mParticles.erase(mParticles.begin() + lParticleIndex);
					--lParticleIndex;
				}
				else
				{
					mParticles[lParticleIndex]->Update(pDeltaTime);
					//mParticles[lParticleIndex]->mCameraDistance = Math::SVector3::Dot(mParticles[lParticleIndex]->mPosition - pCameraPosition,lFront);
				}
			}
			/*
			sort(mParticles.begin(), mParticles.end(), [](shared_ptr<CParticle> pA, shared_ptr<CParticle> pB) {
				return pA->mCameraDistance > pB->mCameraDistance;
			});
			*/
			for (int lParticleIndex = 0; lParticleIndex < mParticles.size(); ++lParticleIndex)
			{
				array<SParticleVertex,4> lParticleVertices;

				SParticleVertex& lTopRight = lParticleVertices[0];
				SParticleVertex& lTopLeft = lParticleVertices[1];
				SParticleVertex& lBottomLeft = lParticleVertices[2];
				SParticleVertex& lBottomRight = lParticleVertices[3];

				float lWidth = 1.0f / mParticles[lParticleIndex]->mNumTextureColumn;
				float lHeight = 1.0f / mParticles[lParticleIndex]->mNumTextureRow;

				//bottom-right
				lBottomRight.mPosition.mX += mParticles[lParticleIndex]->mScale.mX;
				lBottomRight.mPosition.mY -= mParticles[lParticleIndex]->mScale.mY;
				lBottomRight.mTextureCoord.mX = lWidth * (mParticles[lParticleIndex]->mCurrentColumn+1);
				lBottomRight.mTextureCoord.mY = lHeight * (mParticles[lParticleIndex]->mCurrentRow+1);

				//top-right
				lTopRight.mPosition.mX += mParticles[lParticleIndex]->mScale.mX;
				lTopRight.mPosition.mY += mParticles[lParticleIndex]->mScale.mY;
				lTopRight.mTextureCoord.mX = lBottomRight.mTextureCoord.mX;
				lTopRight.mTextureCoord.mY = lBottomRight.mTextureCoord.mY - lHeight;

				//top-left
				lTopLeft.mPosition.mX -= mParticles[lParticleIndex]->mScale.mX;
				lTopLeft.mPosition.mY += mParticles[lParticleIndex]->mScale.mY;
				lTopLeft.mTextureCoord.mX = lBottomRight.mTextureCoord.mX - lWidth;
				lTopLeft.mTextureCoord.mY = lBottomRight.mTextureCoord.mY - lHeight;

				//bottom-left
				lBottomLeft.mPosition.mX -= mParticles[lParticleIndex]->mScale.mX;
				lBottomLeft.mPosition.mY -= mParticles[lParticleIndex]->mScale.mY;
				lBottomLeft.mTextureCoord.mX = lBottomRight.mTextureCoord.mX - lWidth;
				lBottomLeft.mTextureCoord.mY = lBottomRight.mTextureCoord.mY;


				//facing camera and translation
				for (int i = 0; i < 4; ++i)
				{
					lParticleVertices[i].mPosition = lParticleVertices[i].mPosition.Transform(lRotationMatrix) +mParticles[lParticleIndex]->mPosition;
				}
				pVertexBuffer->AddParticleVertex(mMemoryPoolIndex, lParticleVertices);
			}
		}
	}



	template<typename T>
	void CParticleEmitter::SetProperty(EParticleEmitterProperty pProperty, const T& pValue)
	{
		printf("you can not set the property with [%s] type.\n", typeid(T).name());
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

		case EParticleEmitterProperty::eAccelerationVariance:
			mAccelerationVariance = pValue;
			break;

		case EParticleEmitterProperty::ePositionVariance:
			mPositionVariance = pValue;
			break;

		case EParticleEmitterProperty::eVelocityVariance:
			mVelocityVariance = pValue;
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

		case EParticleEmitterProperty::eScale:
			mScale = pValue;
			break;

		case EParticleEmitterProperty::eScaleVariance:
			mScaleVariance = pValue;
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

		case EParticleEmitterProperty::eAngularAcceleration:
			mAngularAcceleration = pValue;
			break;

		case EParticleEmitterProperty::eAngularVelocity:
			mAngularVelocity = pValue;
			break;

		case EParticleEmitterProperty::eCurrentFrame:
			mCurrentFrame = pValue;
			break;

		case EParticleEmitterProperty::eFrameRate:
			mFrameRate = pValue;
			break;

		case EParticleEmitterProperty::eCurrentTime:
			mCurrentTime = pValue;
			break;

		case EParticleEmitterProperty::eDuration:
			mDuration = pValue;
			break;

		case EParticleEmitterProperty::eParticleDuration:
			mParticleDuration = pValue;
			break;

		case EParticleEmitterProperty::eEmissionRate:
			mEmissionRate = pValue;
			break;

		case EParticleEmitterProperty::eAngleVariance:
			mAngleVariance = pValue;
			break;

		case EParticleEmitterProperty::eAngularAccelerationVariance:
			mAngularAccelerationVariance = pValue;
			break;

		case EParticleEmitterProperty::eAngularVelocityVariance:
			mAngularVelocityVariance = pValue;
			break;

		case EParticleEmitterProperty::eParticleDurationVariance:
			mParticleDurationVariance = pValue;
			break;

		case EParticleEmitterProperty::eFrameVariance:
			mFrameVariance = pValue;
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