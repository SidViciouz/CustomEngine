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
}