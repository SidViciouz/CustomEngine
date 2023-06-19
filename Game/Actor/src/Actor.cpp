#include "Actor/header/Actor.h"

namespace Game
{
	shared_ptr<CActor> CActor::Create(const char* pPath)
	{
		return shared_ptr<CActor>(new CActor(pPath));
	}
	

	CActor::CActor(const char* pPath)
		: mMesh{ make_shared<Renderer::CMesh>(pPath) }, mObject{ make_shared<Renderer::CObject>() }
	{

	}

	CActor::~CActor()
	{

	}

	void CActor::Update()
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