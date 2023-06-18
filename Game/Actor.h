#include "../Mesh/header/Mesh.h"
#include "../Mesh/header/Object.h"
#include <memory>

namespace Game
{
	class CActor
	{
	public:
		static shared_ptr<CActor>				Create(const char* pPath);
												~CActor();

	protected:
												CActor() = delete;
												CActor(const char* pPath);
		//when ~Actor is protected, shared_ptr<CActor>(new CActor(pPath)) doesn't work.
		
		shared_ptr<Renderer::CMesh>				mMesh;
		shared_ptr<Renderer::CObject>			mObject;


		friend shared_ptr<CActor>				Create(const char* pPath);
	};
}