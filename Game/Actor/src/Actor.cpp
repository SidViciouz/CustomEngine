#include "Actor/header/Actor.h"

namespace Game
{
	shared_ptr<CActor> CActor::Create(const char* pPath)
	{
		return shared_ptr<CActor>(new CActor(pPath));
	}
	
	shared_ptr<CActor> CActor::Create(shared_ptr<Renderer::CMesh> pMesh)
	{
		return shared_ptr<CActor>(new CActor(pMesh));
	}

	CActor::CActor(const char* pPath)
		: mMesh{ make_shared<Renderer::CMesh>(pPath) }, mObject{ make_shared<Renderer::CObject>() }
	{

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
		mObject.reset();
		mMesh.reset();
	}

	void CActor::Update(double pDeltaTime)
	{

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
		mMeshHandle = pMeshHandle;
	}

	int CActor::GetMeshHandle() const
	{
		return mMeshHandle;
	}

	void CActor::SetObjectHandle(int pObjectHandle)
	{
		mObjectHandle = pObjectHandle;
	}

	int CActor::GetObjectHandle() const
	{
		return mObjectHandle;
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

	const Math::SVector3 CActor::GetTranslation() const
	{
		return mObject->GetTranslation();
	}

	const Math::SQuaternion CActor::GetOrientation() const
	{
		return mObject->GetOrientation();
	}

	const Math::SVector3 CActor::GetScale() const
	{
		return mObject->GetScale();
	}
}