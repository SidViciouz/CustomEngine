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
		void									Reset(shared_ptr<Renderer::CMesh> pMesh);

		virtual void							Update(double pDeltaTime);

		shared_ptr<Renderer::CMesh>				GetMesh() const;
		shared_ptr<Renderer::CObject>			GetObject() const;

		//set mesh handle that represent this mesh in renderer
		void									SetMeshHandle(int pMeshHandle);
		int										GetMeshHandle() const;

		//set object handle that represent this mesh in rendere
		void									SetObjectHandle(int pObjectHandle);
		int										GetObjectHandle() const;

		//set actor's transforom
		void									SetTranslation(const Math::SVector3& pTranslation);
		void									SetOrientation(const Math::SQuaternion& pOrientation);
		void									SetScale(const Math::SVector3& pScale);

		const Math::SVector3					GetTranslation() const;
		const Math::SQuaternion					GetOrientation() const;
		const Math::SVector3					GetScale() const;

	protected:
												CActor() = delete;
												CActor(const char* pPath);
												CActor(shared_ptr<Renderer::CMesh> pMesh);
		
		shared_ptr<Renderer::CMesh>				mMesh;

		shared_ptr<Renderer::CObject>			mObject;
	};
}