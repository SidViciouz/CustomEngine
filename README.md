# Untitled

![particle and input](https://github.com/SidViciouz/Untitled/assets/70373435/41e8ba92-059d-44ac-a696-7a94418b235f)

## PBR

* DrawMeshPBR()로 물리 기반 렌더링 가능

```C
 r.DrawMeshPBR(MeshHandle, ObjectHandle, AlbedoTextureHandle, MetallicTextureHandle, NormalTexture3Handle, RoughnessTexture4Handle, AOTextureHandle);
```

## Animation

* Animation Graph를 구성하고 Transition을 추가하면 조건에 따라서 Animation을 변경 가능

```
CAnimationGraph AnimGraph(lMesh2->GetSkeleton());
AnimGraph.LoadAnimation("n_walk_f", "../Model/ALS_N_Walk_F.FBX");
AnimGraph.LoadAnimation("n_run_f", "../Model/ALS_N_Run_F.FBX");
AnimGraph.LoadAnimation("cls_walk_f", "../Model/ALS_CLF_Walk_F.FBX");
AnimGraph.AddTransition("cls_walk_f", "n_run_f", []()->bool {return true; }, 1.0f);
AnimGraph.AddTransition("n_run_f", "n_walk_f", []()->bool {return true; }, 1.0f);
AnimGraph.AddTransition("n_walk_f", "cls_walk_f", []()->bool {return true; }, 1.0f);
AnimGraph.Reset("n_walk_f");
```

## Particle System

* Particle Emitter를 등록하고 attribute들을 설정해서 particle을 생성

```
CParticleSystem ParticleSystem = ParticleManager->AddParticleSystem();

CParticleEmitter ParticleEmitter = ParticleManager->AddParticleEmitter(ParticleSystem);

lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eEmissionRate, 1.5f);
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::ePosition, Math::SVector3(1, 0, 3));
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::ePositionVariance, Math::SVector3(1.0, 0.2, 1.0));
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eVelocity, Math::SVector3(0.2, 0.2, 0.2));
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eVelocityVariance, Math::SVector3(0.1, 0.1, 0.1));
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eAcceleration, Math::SVector3(0.2, 0.2, 0.2));
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eAccelerationVariance, Math::SVector3(0.1, 0.1, 0.1));
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eScale, Math::SVector2(0.5, 0.5));
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eCurrentFrame, 0.0f);
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eDuration, 0.0f);
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eNumTextureRow, 8);
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eNumTextureColumn, 8);
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eFrameRate, 30.0f);
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eParticleDuration, 3.0f);
lParticleManager->SetParticleEmitterValue(ParticleEmitter, EParticleEmitterProperty::eParticleDurationVariance, 1.0f);
```
