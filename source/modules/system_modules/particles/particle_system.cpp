#include "mcv_platform.h"
#include "particle_system.h"
#include "particle_parser.h"
#include "render/render_objects.h"
#include "components/comp_camera.h"
#include "components/comp_transform.h"
#include <random>
#include "skeleton/comp_skeleton.h"
#include "skeleton/cal3d2engine.h"

class CParticleResourceClass : public CResourceClass {
public:
	CParticleResourceClass() {
		class_name = "Particles";
		extensions = { ".particles" };
	}
	IResource* create(const std::string& name) const override {
		dbg("Creating particles %s\n", name.c_str());
		Particles::CParser parser;
		Particles::TCoreSystem* res = new Particles::TCoreSystem;
		parser.parseParticlesFile(name, res);
		assert(res);
		return res;
	}

	
};

void Particles::TCoreSystem::debugInMenu() {
	{ // Life
		ImGui::Text("Life");
		ImGui::DragFloat("Duration", &life.duration, 0.01f);
		ImGui::DragFloat("Duration variation", &life.duration, 0.01f);
		ImGui::DragInt("Max particles", &life.maxParticles);
		ImGui::DragFloat("Time factor", &life.timeFactor, 0.01f);
	}

	{ // Emission
		ImGui::Text("Emission");
		std::string types[] = { "Point", "Line", "Square", "Box", "Sphere", "Circle", "Cylinder" };
		if (ImGui::BeginCombo("Type", types[emission.type].c_str())) {
			for (int i = 0; i < TEmission::EType::End; i++) {
				if (ImGui::Selectable(types[i].c_str(), emission.type == i)) {
					emission.type = TEmission::EType(i);
				}
			}
			ImGui::EndCombo();
		}
		ImGui::DragFloat("Interval", &emission.interval, 0.0001f);
		ImGui::DragInt("Count", &emission.count);
		ImGui::Checkbox("Cyclic", &emission.cyclic);
		ImGui::DragFloat("Size", &emission.size, 0.01f);
		float deg = rad2deg(emission.angle);
		if (ImGui::DragFloat("Angle", &deg)) {
			emission.interval = deg2rad(deg);
		}
		ImGui::Checkbox("Radial", &emission.radial);
		ImGui::Checkbox("Random direction", &emission.randomDirection);
		ImGui::DragFloat("Duration", &emission.duration, 0.0001f);
	}

	{ // Movement
		ImGui::Text("Movement");
		ImGui::DragFloat("Velocity", &movement.velocity, 0.01f);
		ImGui::DragFloat("Velocity variation", &movement.velocityVariation, 0.01f);
		ImGui::DragFloat("Acceleration", &movement.acceleration, 0.01f);
		float deg = rad2deg(movement.initialRotation);
		if (ImGui::DragFloat("Initial rotation", &deg)) {
			movement.initialRotation = deg2rad(deg);
		}
		deg = rad2deg(movement.spin);
		if (ImGui::DragFloat("Spin", &deg)) {
			movement.spin = deg2rad(deg);
		}
		if (render.mesh) ImGui::DragFloat3("Spin axis", &movement.spin_axis.x, 1.f);
		ImGui::DragFloat("Gravity", &movement.gravity, 0.01f);
		ImGui::DragFloat("Wind", &movement.wind, 0.01f);
		ImGui::Checkbox("Ground", &movement.ground);
	}

	{ // Render
		ImGui::Text("Render");
		std::string types[] = { "Billboard", "HorizontalBillboard", "StretchedBillboard", "Mesh" };
		if (ImGui::BeginCombo("Type", types[render.type].c_str())) {
			for (int i = 0; i < TRender::EType::End; i++) {
				if (ImGui::Selectable(types[i].c_str(), render.type == i)) {
					render.type = TRender::EType(i);
				}
			}
			ImGui::EndCombo();
		}
		ImGui::Text("Sprite");
		const_cast<CTexture*>(render.texture)->debugInMenu();
		ImGui::Text("Mesh");
		const_cast<CRenderMesh*>(render.mesh)->debugInMenu();
		ImGui::DragFloat2("Frame size (UV)", &render.frameSize.x, 0.01f, -1.f, 1.f);
		ImGui::DragInt("Num frames", &render.numFrames);
		ImGui::DragInt("Initial frame", &render.initialFrame);
		ImGui::DragFloat("Frame speed", &render.frameSpeed, 0.01f);
		if (render.type == TRender::StretchedBillboard) ImGui::DragFloat("Motion blur amount", &render.motionBlurAmount, 0.001f);

	}
}

void Particles::TCoreSystem::onFileChanged(const std::string& filename) {
	if (filename != getName()) {
		return;
	}
	dbg("Hot reloading %s\n", filename.c_str());
	destroy();
	Particles::CParser().parseParticlesFile(filename, this);
}


// A specialization of the template defined at the top of this file
// If someone class getResourceClassOf<Particles::TCoreParticleSystem>, use this function:
template<>
const CResourceClass* getResourceClassOf<Particles::TCoreSystem>() {
	static CParticleResourceClass the_resource_class;
	return &the_resource_class;
}

namespace {
	float random(float start, float end) {
		static std::default_random_engine e;
		static std::uniform_real<float> d{ 0, 1 };
		return lerp(start, end, d(e));
	}
	const VEC3 kGravity(0.f, -9.8f, 0.f);
}

namespace Particles {

	CSystem::CSystem(TParticleHandle handle, const TCoreSystem* core, CHandle entityHandle, LaunchConfig config)
		: _core(core)
		, _handle(handle)
		, particleEntityHandle(entityHandle)
		, config(config) {
		assert(_core);
	}

	void CSystem::launch() {
		_time = 0.f;
		_globalTime = 0.f;
		updateWorld();
		emit();
	}


	bool CSystem::update(float delta) {
		updateWorld();
		const VEC3& kWindVelocity = EngineParticles.getWindVelocity();

		float fadeRatio = 1.f;
		if (_fadeDuration != 0.f) {
			_fadeTime += delta;
			fadeRatio = 1.f - (_fadeTime / _fadeDuration);
		}

		delta *= _core->life.timeFactor;

		auto it = _particles.begin();
		while (it != _particles.end()) {
			TParticle& p = *it;

			p.lifetime += delta;

			if (p.max_lifetime > 0.f && p.lifetime >= p.max_lifetime) {
				it = _particles.erase(it);
			}
			else {
				VEC3 dir = p.velocity;
				dir.Normalize();
				p.velocity += dir * _core->movement.acceleration * delta;
				p.velocity += kGravity * _core->movement.gravity * delta;
				
				if (_core->movement.following) {
					p.localPosition += p.velocity * delta;
					p.localPosition += kWindVelocity * _core->movement.wind * delta;
					p.position = VEC3::Transform(p.localPosition, world);
				}
				else {
					p.position += p.velocity * delta;
					p.position += kWindVelocity * _core->movement.wind * delta;
				}
				if (!_core->render.mesh) { //Billboard particle
					p.rotation += _core->movement.spin * delta;
				}
				else { //Mesh particle
					QUAT quat = QUAT::CreateFromAxisAngle(_core->movement.spin_axis, _core->movement.spin * delta);
					p.rotationQuat = p.rotationQuat * quat;
				}
				if (_core->movement.ground) {
					p.position.y = std::max(0.f, p.position.y);
				}

				float life_ratio = p.max_lifetime > 0.f ? clamp(p.lifetime / p.max_lifetime, 0.f, 1.f) : 1.f;
				p.color = _core->color.colors.get(life_ratio) * fadeRatio;
				p.color.z *= _core->color.opacity;
				p.size = _core->size.sizes.get(life_ratio);

				int frame_idx = (int)(p.lifetime * _core->render.frameSpeed);
				p.frame = _core->render.initialFrame + (frame_idx % _core->render.numFrames);

				++it;
			}
		}

		bool canEmmit = _core->emission.cyclic || _globalTime <= _core->emission.duration;
		// check if a new batch of particles needs to be generated
		if (canEmmit) {
			_time += delta;
			if (_time > _core->emission.interval) {
				emit();
				_time -= _core->emission.interval;
			}
		}

		_globalTime += delta;

		return fadeRatio > 0.f && (!_particles.empty() || canEmmit);
	}

	void CSystem::render() {
		const CRenderTechnique* technique = _core->render.technique;
		const CRenderMesh* particleMesh = _core->render.mesh;
		CEntity* eCurrentCamera = EngineCameras.getCurrentBlendedCamera();
		assert(technique && particleMesh && eCurrentCamera);
		TCompCamera* camera = eCurrentCamera->get< TCompCamera >();
		assert(camera);
		const VEC3 cameraPos = camera->getCamera()->getPosition();
		const VEC3 cameraUp = camera->getCamera()->getUp();
		const VEC3 cameraForward = camera->getCamera()->getFront();

		const int frameCols = static_cast<int>(1.f / _core->render.frameSize.x);

		technique->activate();
		_core->render.texture->activate(TS_ALBEDO);

		for (auto& p : _particles) {
			VEC3 particlePosition = p.position;
			if (_core->render.type == TCoreSystem::TRender::Billboard) {
				MAT44 bb = MAT44::CreateBillboard(particlePosition, cameraPos, VEC3(0, 1, 0));
				MAT44 sc = MAT44::CreateScale(p.size * p.scale);
				MAT44 rt = MAT44::CreateFromYawPitchRoll(0.f, 0.f, p.rotation);
				cb_object.obj_world = rt * sc * bb;
			}
			else if (_core->render.type == TCoreSystem::TRender::HorizontalBillboard) {
				MAT44 bb = MAT44::CreateConstrainedBillboard(particlePosition, cameraPos, VEC3(0, 1, 0), &cameraForward);
				MAT44 sc = MAT44::CreateScale(p.size * p.scale);
				MAT44 rt = MAT44::CreateFromYawPitchRoll(0.f, 0.f, p.rotation);
				cb_object.obj_world = rt * sc * bb;
			}
			else if (_core->render.type == TCoreSystem::TRender::Mesh) {
				cb_object.obj_world = MAT44::CreateScale(p.size * p.scale)
					* MAT44::CreateFromQuaternion(p.rotationQuat)
					* MAT44::CreateFromQuaternion(config.rotationOffset)
					* MAT44::CreateTranslation(particlePosition);
			}

			int row = p.frame / frameCols;
			int col = p.frame % frameCols;
			VEC2 minUV = VEC2(col * _core->render.frameSize.x, row * _core->render.frameSize.y);
			VEC2 maxUV = minUV + _core->render.frameSize;

			cb_object.obj_color = VEC4(1, 1, 1, 1);
			cb_object.updateGPU();

			cb_particles.particle_minUV = minUV;
			cb_particles.particle_maxUV = maxUV;
			cb_particles.particle_color = p.color;
			cb_particles.particle_velocity = p.velocity;
			cb_particles.particle_scale = p.scale * p.size;
			cb_particles.particle_position = particlePosition;
			cb_particles.particle_rotation = p.rotation;
			cb_particles.particle_motion_blur_amount = _core->render.motionBlurAmount;

			cb_particles.updateGPU();

			particleMesh->activateAndRender();
		}
	}


	void CSystem::emit() {
		for (int i = 0; i < _core->emission.count && _particles.size() < _core->life.maxParticles; ++i) {
			TParticle particle;
			particle.localPosition = generatePosition();
			particle.position = VEC3::Transform(particle.localPosition, world);
			particle.velocity = generateVelocity(particle.position);
			particle.color = _core->color.colors.get(0.f);
			particle.size = _core->size.sizes.get(0.f);
			particle.scale = _core->size.scale + random(-_core->size.scale_variation, _core->size.scale_variation);
			particle.frame = _core->render.initialFrame;
			particle.rotation = _core->movement.initialRotation;
			particle.lifetime = 0.f;
			particle.max_lifetime = _core->life.duration + random(-_core->life.durationVariation, _core->life.durationVariation);
			_particles.push_back(particle);
		}
	}

	void CSystem::forceEmission(int quantity) {
		updateWorld();
		for (int i = 0; i < quantity; ++i) {
			TParticle particle;
			particle.localPosition = generatePosition();
			particle.position = VEC3::Transform(particle.localPosition, world);
			particle.velocity = generateVelocity(particle.position);
			particle.color = _core->color.colors.get(0.f);
			particle.size = _core->size.sizes.get(0.f);
			particle.scale = _core->size.scale + random(-_core->size.scale_variation, _core->size.scale_variation);
			particle.frame = _core->render.initialFrame;
			particle.rotation = 0.f;
			particle.lifetime = 0.f;
			particle.max_lifetime = _core->life.duration + random(-_core->life.durationVariation, _core->life.durationVariation);

			_particles.push_back(particle);
		}
	}

	CHandle CSystem::getParticleEntityHandle() {
		return particleEntityHandle;
	}

	VEC3 CSystem::generatePosition() const {
		const float& size = _core->emission.size;

		switch (_core->emission.type) {
		case TCoreSystem::TEmission::Point:
			return VEC3::Zero;

		case TCoreSystem::TEmission::Line:
			return VEC3(random(-size, size), 0.f, 0.f);

		case TCoreSystem::TEmission::Square:
			return VEC3(random(-size, size), 0.f, random(-size, size));

		case TCoreSystem::TEmission::Box:
			return VEC3(random(-size, size), random(-size, size), random(-size, size));

		case TCoreSystem::TEmission::Sphere:
		{
			VEC3 dir(random(-1, 1), random(-1, 1), random(-1, 1));
			dir.Normalize();
			return dir * random(0, size);
		}
		case TCoreSystem::TEmission::Circle:
		{
			VEC3 dir(random(-1, 1), 0.f, random(-1, 1));
			dir.Normalize();
			return dir * random(0, size);
		}
		case TCoreSystem::TEmission::Cylinder:
		{
			VEC3 dir(random(-1, 1), 0.f, random(-1, 1));
			dir.Normalize();
			VEC3 pos = dir * random(0, size);
			pos.y = random(-size, size);
			return pos;
		}
		}
		return VEC3::Zero;
	}

	VEC3 CSystem::generateVelocity(const VEC3& particlePosition) const {
		const float velocity = _core->movement.velocity + random(-_core->movement.velocityVariation, _core->movement.velocityVariation);

		if (_core->emission.radial) {
			VEC3 dir = particlePosition - position;
			dir.Normalize();
			return dir * velocity;
		}

		if (_core->emission.randomDirection) {
			VEC3 dir = VEC3(random(-1, 1), random(-1, 1), random(-1, 1));
			dir.Normalize();
			return dir * velocity;
		}

		const float& angle = _core->emission.angle;
		if (angle != 0.f) {
			float radius = tan(angle);
			float x = sqrtf(radius) * cosf(angle) * random(-1, 1);
			float z = sqrtf(radius) * sinf(angle) * random(-1, 1);
			VEC3 pos(x, 1.f, z);
			pos.Normalize();
			return pos * velocity;
		}
		return VEC3::Up * velocity;
	}

	TParticleHandle CSystem::getHandle() const {
		return _handle;
	}

	void CSystem::fadeOut(float duration) {
		_fadeDuration = duration;
		_fadeTime = 0.f;
	}

	void CSystem::setOffset(VEC3 offset) {
		this->config.offset = offset;
	}

	void CSystem::setRotationOffset(QUAT rotationOffset) {
		this->config.rotationOffset = rotationOffset;
	}

	void CSystem::updateWorld() {
		world = MAT44::Identity;
		if (config.targetEntity.isValid()) {
			CEntity* e = config.targetEntity;

			if (config.bone != "" && boneId == -1) {
				boneId = ((TCompSkeleton*)e->get<TCompSkeleton>())->getBoneId(config.bone);
			}

			QUAT rotation;
			VEC3 translation;
			if (boneId != -1) { // Follow bone
				CalBone* bone = ((TCompSkeleton*)e->get<TCompSkeleton>())->getBone(boneId);
				rotation = Cal2DX(bone->getRotationAbsolute());
				translation = Cal2DX(bone->getTranslationAbsolute());
			}
			else { // Follow transform
				TCompTransform* e_transform = e->get<TCompTransform>();
				rotation = e_transform->getRotation();
				translation = e_transform->getPosition();
			}
			position = translation + config.offset;
			world = MAT44::CreateTranslation(config.offset)
				* MAT44::CreateFromQuaternion(rotation)
				* MAT44::CreateTranslation(translation);
		}
	}


}
