#include "Actor/header/Bullet.h"
#include "../Mesh/header/Mesh.h"
#include "Actor/header/ActorPool.h"
#include "Game/header/Game.h"
#include "Component/header/PhysicsComponent.h"

namespace Game
{
	shared_ptr<CBullet> CBullet::Create(shared_ptr<Renderer::CMesh> pMesh)
	{
		shared_ptr<CBullet> lNew = shared_ptr<CBullet>(new CBullet(pMesh));

		lNew->mThis = lNew;

		return lNew;
	}

	CBullet::CBullet(shared_ptr<Renderer::CMesh> pMesh)
		: CActor{pMesh}
	{
		mTimer.Reset();

		mPhysicsComponent = make_shared<CPhysicsComponent>(shared_ptr<CActor>(this));
		mPhysicsComponent->SetVelocity(Math::SVector3(0, 0, 100));
		mPhysicsComponent->SetAcceleration(Math::SVector3(0, 0, 20));
		AddComponent(mPhysicsComponent);
	}

	CBullet::~CBullet()
	{

	}

	void CBullet::Reset(shared_ptr<Renderer::CMesh> pMesh)
	{
		CActor::Reset(pMesh);

		mTimer.Reset();
		
		mPhysicsComponent->Reset();
		mPhysicsComponent->SetVelocity(Math::SVector3(0,0,100));
		mPhysicsComponent->SetAcceleration(Math::SVector3(0,0,20));
	}


	void CBullet::Update(double pDeltaTime)
	{
		mTimer.Tick();

		if (mTimer.GetTotalTime() > 0.5f)
		{
			Destroy();
			return;
		}

		CActor::Update(pDeltaTime);
	}

	void CBullet::Listen(const SEvent pEvent)
	{
		printf("this is bullet!\n");
	}

	void CBullet::Destroy()
	{
		CWorld::Singleton()->AddDestroyQueue(mThis);
	}
}