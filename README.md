# CustomEngine

![particle and input](https://github.com/SidViciouz/Untitled/assets/70373435/41e8ba92-059d-44ac-a696-7a94418b235f)

<br>
<br>
<br>
<br>
<br>

## 진입점

Game/main.cpp의 wWinMain이 진입점입니다.


## PBR

* DrawMeshPBR()로 물리 기반 렌더링 가능


```cpp
 Renderer.DrawMeshPBR(MeshHandle, ObjectHandle, AlbedoTextureHandle, MetallicTextureHandle, NormalTexture3Handle, RoughnessTexture4Handle, AOTextureHandle);
```
<br>
<br>

![PBR예시](https://github.com/SidViciouz/Untitled/assets/70373435/9063d86d-7ae0-4583-afe3-f35e49b07f00)

<br>
<br>
<br>

## Animation

* Animation Graph를 구성하고 Transition을 추가하면 조건에 따라서 Animation을 변경 가능


```cpp
CAnimationGraph AnimGraph(Mesh->GetSkeleton());
AnimGraph.LoadAnimation("n_walk_f", "../Model/ALS_N_Walk_F.FBX");
AnimGraph.LoadAnimation("n_run_f", "../Model/ALS_N_Run_F.FBX");
AnimGraph.LoadAnimation("cls_walk_f", "../Model/ALS_CLF_Walk_F.FBX");
AnimGraph.AddTransition("cls_walk_f", "n_run_f", []()->bool {return true; }, 1.0f);
AnimGraph.AddTransition("n_run_f", "n_walk_f", []()->bool {return true; }, 1.0f);
AnimGraph.AddTransition("n_walk_f", "cls_walk_f", []()->bool {return true; }, 1.0f);
AnimGraph.Reset("n_walk_f");
```
<br>
<br>

![애니메이션 트랜지션](https://github.com/SidViciouz/Untitled/assets/70373435/40177f62-b100-497e-8df8-3b82d8756deb)

<br>
<br>
<br>


## Particle System

* Particle Emitter를 등록하고 attribute들을 설정해서 particle을 생성


```cpp
CParticleSystem ParticleSystem = ParticleManager->AddParticleSystem();

CParticleEmitter ParticleEmitter = ParticleManager->AddParticleEmitter(ParticleSystem);

lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eEmissionRate, 10.0f);
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::ePosition, Math::SVector3(1, 0, 3));
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::ePositionVariance, Math::SVector3(1.0, 0.2, 1.0));
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eVelocity, Math::SVector3(0.2, 0.2, 0.2));
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eVelocityVariance, Math::SVector3(0.1, 0.1, 0.1));
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eAcceleration, Math::SVector3(0.2, 0.2, 0.2));
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eAccelerationVariance, Math::SVector3(0.1, 0.1, 0.1));
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eScale, Math::SVector2(0.5, 0.5));
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eCurrentFrame, 0.0f);
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eDuration, 0.0f);
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eNumTextureRow, 6);
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eNumTextureColumn, 6);
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eFrameRate, 30.0f);
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eParticleDuration, 3.0f);
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eParticleDurationVariance, 1.0f);
```
<br>
<br>

![파티클 예시](https://github.com/SidViciouz/Untitled/assets/70373435/cc32d3e7-900d-4ab1-8203-b51f9f7f6820)

<br>
<br>
<br>


## 사용법

```cpp
// input manager 생성
shared_ptr<CInputManager> lInputManager = CInputManager::Create();

// particle manager, emitter 생성
shared_ptr<CParticleManager> lParticleManager = make_shared<CParticleManager>();
shared_ptr<CParticleSystem> lParticleSystem = lParticleManager->AddParticleSystem();
shared_ptr<CParticleEmitter> lParticleEmitter = lParticleManager->AddParticleEmitter(lParticleSystem);

// particle emitter property 설정
lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eEmissionRate, 1.5f);
lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::ePosition, Math::SVector3(1, 0, 3));
lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::ePositionVariance, Math::SVector3(1.0, 0.2, 1.0));
lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eVelocity, Math::SVector3(0.2, 0.2, 0.2));
lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eVelocityVariance, Math::SVector3(0.1, 0.1, 0.1));
lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eAcceleration, Math::SVector3(0.2, 0.2, 0.2));
lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eAccelerationVariance, Math::SVector3(0.1, 0.1, 0.1));
lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eScale, Math::SVector2(0.5, 0.5));
lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eCurrentFrame, 0.0f);
lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eDuration, 0.0f);
lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eNumTextureRow, 8);
lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eNumTextureColumn, 8);
lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eFrameRate, 30.0f);
lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eParticleDuration, 3.0f);
lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eParticleDurationVariance, 1.0f);

// 카메라 및 오브젝트 생성
shared_ptr<CCamera> lCamera = make_shared<CCamera>(800, 600);
shared_ptr<CMesh>	lMesh1 = make_shared<CMesh>("../Model/Complex_Arch_01.fbx");
shared_ptr<CObject>	lObject1 = make_shared<CObject>();
shared_ptr<CMesh>	lMesh2 = make_shared<CMesh>("../Model/AnimMan2.FBX");
shared_ptr<CObject>	lObject2 = make_shared<CObject>();
shared_ptr<CMesh>	lMesh3 = make_shared<CMesh>("../Model/Sphere.fbx");
shared_ptr<CObject>	lObject3 = make_shared<CObject>();
shared_ptr<CMesh>	lMesh4 = make_shared<CMesh>("../Model/Simple_Floor_20x20.FBX");
shared_ptr<CObject>	lObject4 = make_shared<CObject>();

// 오브젝트 설정
lObject1->SetTranslation(Math::SVector3(0, -3, 3));
lObject1->SetOrientation(Math::SQuaternion(1 * cosf(DirectX::XMConvertToRadians(-45)), 0, 0, sinf(DirectX::XMConvertToRadians(-45))));
lObject1->SetScale(Math::SVector3(0.01f, 0.01f, 0.01f));
lObject2->SetTranslation(Math::SVector3(0, -1.5, 2));
lObject2->SetOrientation(Math::SQuaternion(1 * cosf(DirectX::XMConvertToRadians(-45)), 0, 0, sinf(DirectX::XMConvertToRadians(-45))));
lObject2->SetScale(Math::SVector3(0.01f, 0.01f, 0.01f));
lObject3->SetTranslation(Math::SVector3(0, 1.5, 5));
lObject3->SetScale(Math::SVector3(2, 2, 2));
lObject4->SetTranslation(Math::SVector3(0, -3.0f, 0.0f));
lObject4->SetOrientation(Math::SQuaternion(1 * cosf(DirectX::XMConvertToRadians(-45)), 0, 0, sinf(DirectX::XMConvertToRadians(-45))));
lObject4->SetScale(Math::SVector3(0.03f, 0.03f, 0.03f));

// 애니메이션 그래프 생성
CAnimationGraph lAnimGraph(lMesh2->GetSkeleton());
lAnimGraph.LoadAnimation("n_walk_f", "../Model/ALS_N_Walk_F.FBX");
lAnimGraph.LoadAnimation("n_run_f", "../Model/ALS_N_Run_F.FBX");
lAnimGraph.LoadAnimation("cls_walk_f", "../Model/ALS_CLF_Walk_F.FBX");
lAnimGraph.LoadAnimation("tpose", "../Model/ALS_Mannequin_T_Pose.FBX");
lAnimGraph.LoadAnimation("clf_getup_back", "../Model/ALS_CLF_GetUp_Back.FBX");
lAnimGraph.LoadAnimation("n_pose", "../Model/ALS_N_Pose.FBX");
lAnimGraph.LoadAnimation("n_jumpwalk_lf", "../Model/ALS_N_JumpWalk_LF.FBX");
lAnimGraph.LoadAnimation("kicking", "../Model/Kicking.fbx");
lAnimGraph.Reset("n_run_f");
lAnimGraph.AddTransition("cls_walk_f", "n_run_f", []()->bool {return true; }, 1.0f);
lAnimGraph.AddTransition("n_run_f", "cls_walk_f", []()->bool {return true; }, 1.0f);

// 렌더러 생성
CRenderer r(hInstance);
r.Initialize();

/ 렌더러에 텍스처 로드
r.LoadBegin();
int lTexture1Handle = r.LoadTexture(L"../Material/rustediron2_basecolor.dds");
int lTexture2Handle = r.LoadTexture(L"../Material/rustediron2_metallic.dds");
int lTexture3Handle = r.LoadTexture(L"../Material/rustediron2_normal.dds");
int lTexture4Handle = r.LoadTexture(L"../Material/rustediron2_roughness.dds");
int lTileAOHandle = r.LoadTexture(L"../Material/T_Tiles_M.dds");
int lTileNormalHandle = r.LoadTexture(L"../Material/T_Tiles_N.dds");
int lParticleSpriteHandle = r.LoadTexture(L"../Material/T_Smoke_A.dds");
r.LoadEnd();

float lData1[9] = { 1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,1.0f };
float lData2[9] = { 0.0f,1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,1.0f,1.0f };
float lData3[9] = { 0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,2.0f };

// 렌더러에 메시, 오브젝트, 카메라 설정
r.SetCamera(lCamera);

int lMesh1Handle = r.SetMesh(lMesh1);
int lObject1Handle = r.SetObject(lObject1);

int lMesh2Handle = r.SetMesh(lMesh2);
int lObject2Handle = r.SetObject(lObject2);

int lMesh3Handle = r.SetMesh(lMesh3);
int lObject3Handle = r.SetObject(lObject3);

int lMesh4Handle = r.SetMesh(lMesh4);
int lObject4Handle = r.SetObject(lObject4);

r.SetParticleManager(lParticleManager);

r.Resize(1800, 1000);

// 렌더링 루프
while (r.Loop())
{
	// input manager에서 간단하게 캐릭터 이동
	if (lInputManager->GetKeyPressed(0x41))
	{
	lObject2->AddTranslation(Math::SVector3(-0.01f, 0, 0));
	}
	if (lInputManager->GetKeyPressed(0x44))
	{
	lObject2->AddTranslation(Math::SVector3(0.01f, 0, 0));
	}
	if (lInputManager->GetKeyPressed(0x57))
	{
	lObject2->AddTranslation(Math::SVector3(0, 0, 0.01f));
	}
	if (lInputManager->GetKeyPressed(0x53))
	{
	lObject2->AddTranslation(Math::SVector3(0, 0, -0.01f));
	}

	// 파티클과 애니메이션 업데이트
	lParticleManager->Update(1 / 60.0f, lCamera->GetPosition());
	lAnimGraph.Update(1 / 60.0f);

	// 렌더링
	r.DrawBegin();

	r.DrawLine(lData1);
	r.DrawLine(lData2);
	r.DrawLine(lData3);

	r.DrawMesh(lMesh1Handle, lObject1Handle);
	r.DrawMeshPBR(lMesh4Handle, lObject4Handle, -1, -1, lTileNormalHandle, -1, lTileAOHandle);
	r.DrawMeshPBR(lMesh3Handle, lObject3Handle, lTexture1Handle, lTexture2Handle, lTexture3Handle, lTexture4Handle, -1);
	r.DrawParticles(lParticleSpriteHandle);
	r.DrawMeshPBR(lMesh2Handle, lObject2Handle, -1, -1, -1, -1, -1);

	r.DrawEnd();
}

```

