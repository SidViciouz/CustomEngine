#include "Actor/header/Actor.h"
#include "Component/header/BaseComponent.h"

namespace Game
{
	shared_ptr<CActor> CActor::Create(shared_ptr<Renderer::CMesh> pMesh)
	{
		return shared_ptr<CActor>(new CActor(pMesh));
	}

	CActor::CActor(shared_ptr<Renderer::CMesh> pMesh)
		: mMesh{ pMesh }, mObject{ make_shared<Renderer::CObject>() }
	{

	}

	CActor::~CActor()
	{

	}

	void CActor::Reset()
	{
		mObject->SetTranslation(Math::SVector3(0, 0, 0));
		mObject->SetOrientation(Math::SQuaternion(0, 0, 0, 1));
		mObject->SetScale(Math::SVector3(1, 1, 1));

		mMesh.reset();
	}

	void CActor::Reset(shared_ptr<Renderer::CMesh> pMesh)
	{
		mMesh = pMesh;
	}


	void CActor::Update(double pDeltaTime)
	{
		for (auto& lComponent : mComponents)
		{
			lComponent->Update(pDeltaTime);
		}
	}

	shared_ptr<Renderer::CMesh> CActor::GetMesh() const
	{
		return mMesh;
	}

	shared_ptr<Renderer::CObject> CActor::GetObject() const
	{
		return mObject;
	}

	void CActor::SetMeshHandle(int pMeshHandle)
	{
		mMesh->SetMeshHandle(pMeshHandle);
	}

	int CActor::GetMeshHandle() const
	{
		return mMesh->GetMeshHandle();
	}

	void CActor::SetObjectHandle(int pObjectHandle)
	{
		mObject->SetObjectHandle(pObjectHandle);
	}

	int CActor::GetObjectHandle() const
	{
		return mObject->GetObjectHandle();
	}

	void CActor::SetTranslation(const Math::SVector3& pTranslation)
	{
		mObject->SetTranslation(pTranslation);
	}

	void CActor::SetOrientation(const Math::SQuaternion& pOrientation)
	{
		mObject->SetOrientation(pOrientation);
	}

	void CActor::SetScale(const Math::SVector3& pScale)
	{
		mObject->SetScale(pScale);
	}

	const Math::SVector3& CActor::GetTranslation() const
	{
		return mObject->GetTranslation();
	}

	const Math::SQuaternion& CActor::GetOrientation() const
	{
		return mObject->GetOrientation();
	}

	const Math::SVector3& CActor::GetScale() const
	{
		return mObject->GetScale();
	}

	Math::SVector3& CActor::GetTranslation()
	{
		return mObject->GetTranslation();
	}

	Math::SQuaternion& CActor::GetOrientation()
	{
		return mObject->GetOrientation();
	}

	Math::SVector3& CActor::GetScale()
	{
		return mObject->GetScale();
	}

	void CActor::AddComponent(shared_ptr<IBaseComponent> pComponent)
	{
		mComponents.push_back(pComponent);
	}

	void CActor::Listen(SEvent pEvent)
	{

	}
}