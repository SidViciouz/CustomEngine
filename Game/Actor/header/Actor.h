/*
* Actor which is  spawned at game.
* actor has mesh and transform information.
*/

#pragma once

#include "../Mesh/header/Mesh.h"
#include "../Mesh/header/Object.h"
#include <memory>


namespace Game
{
	class CActor
	{
	public:
		static shared_ptr<CActor>				Create(const char* pPath);
		static shared_ptr<CActor>				Create(shared_ptr<Renderer::CMesh> pMesh);
		//when ~Actor is protected, shared_ptr<CActor>(new CActor(pPath)) doesn't work.
												~CActor();

		void									Reset();

		virtual void							Update(double pDeltaTime);

		shared_ptr<Renderer::CMesh>				GetMesh() const;
		shared_ptr<Renderer::CObject>			GetObject() const;

	protected:
												CActor() = delete;
												CActor(const char* pPath);
												CActor(shared_ptr<Renderer::CMesh> pMesh);
		
		shared_ptr<Renderer::CMesh>				mMesh;
		shared_ptr<Renderer::CObject>			mObject;
	};
}