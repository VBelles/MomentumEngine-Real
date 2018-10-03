#include "mcv_platform.h"
#include "particle_parser.h"
#include "modules/system_modules/particles/particle_system.h"

namespace Particles {
	void CParser::parseFile(const std::string& filename) {
		std::ifstream file_json(filename);
		json json_data;
		file_json >> json_data;

		for (auto& pFile : json_data) {
			const TCoreSystem* cps = Resources.get(pFile)->as<TCoreSystem>();
			assert(cps);
		}
	}

	void CParser::parseParticlesFile(const std::string& filename, TCoreSystem* cps) {
		std::ifstream file_json(filename);
		json json_data;
		file_json >> json_data;

		parseParticleSystem(json_data, cps);
	}

	void CParser::parseParticleSystem(const json& data, TCoreSystem* cps) {
		// life
		const json& life = data["life"];
		cps->life.duration = life.value("duration", cps->life.duration);
		cps->life.durationVariation = life.value("duration_variation", cps->life.durationVariation);
		cps->life.maxParticles = life.value("max_particles", cps->life.maxParticles);
		cps->life.timeFactor = life.value("time_factor", cps->life.timeFactor);

		// emission
		const json& emission = data["emission"];
		cps->emission.cyclic = emission.value("cyclic", cps->emission.cyclic);
		cps->emission.interval = emission.value("interval", cps->emission.interval);
		cps->emission.count = emission.value("count", cps->emission.count);
		const std::string emitterType = emission.value("type", "point");
		if (emitterType == "line")			cps->emission.type = TCoreSystem::TEmission::Line;
		else if (emitterType == "square")	cps->emission.type = TCoreSystem::TEmission::Square;
		else if (emitterType == "box")		cps->emission.type = TCoreSystem::TEmission::Box;
		else if (emitterType == "sphere")	cps->emission.type = TCoreSystem::TEmission::Sphere;
		else if (emitterType == "circle")	cps->emission.type = TCoreSystem::TEmission::Circle;
		else if (emitterType == "cylinder")	cps->emission.type = TCoreSystem::TEmission::Cylinder;
		else								cps->emission.type = TCoreSystem::TEmission::Point;
		cps->emission.size = emission.value("size", cps->emission.size);
		cps->emission.halfLength = emission.value("half_length", cps->emission.size);
		cps->emission.angle = deg2rad(emission.value("angle", rad2deg(cps->emission.angle)));
		cps->emission.radial = emission.value("radial", cps->emission.radial);
		cps->emission.randomDirection = emission.value("random_direction", cps->emission.randomDirection);
		cps->emission.duration = emission.value("duration", cps->emission.duration);

		// movement
		const json& movement = data["movement"];
		cps->movement.velocity = movement.value("velocity", cps->movement.velocity);
		cps->movement.velocityVariation = movement.value("velocity_variation", cps->movement.velocityVariation);
		cps->movement.acceleration = movement.value("acceleration", cps->movement.acceleration);
		cps->movement.initialRotation = deg2rad(movement.value("initial_rotation", rad2deg(cps->movement.initialRotation)));
		cps->movement.initialRandomRotation = movement.value("initial_random_rotation", cps->movement.initialRandomRotation);
		cps->movement.initialTargetRotation = movement.value("initial_target_rotation", cps->movement.initialTargetRotation);
		cps->movement.spin = deg2rad(movement.value("spin", rad2deg(cps->movement.spin)));
		cps->movement.spin_axis = movement.count("spin_axis") ? loadVEC3(movement["spin_axis"]) : cps->movement.spin_axis;
		cps->movement.wind = movement.value("wind", cps->movement.wind);
		cps->movement.gravity = movement.value("gravity", cps->movement.gravity);
		cps->movement.ground = movement.value("ground", cps->movement.ground);
		cps->movement.following = movement.value("following", cps->movement.following);
		cps->movement.followTarget = movement.value("follow_target", cps->movement.followTarget);
		cps->movement.followTargetOffset = movement.count("follow_target_offset") ? loadVEC3(movement["follow_target_offset"]) : cps->movement.followTargetOffset;


		// render
		const json& render = data["render"];
		cps->render.initialFrame = render.value("initial_frame", cps->render.initialFrame);
		cps->render.frameSize = loadVEC2(render.value("frame_size", "1 1"));
		cps->render.numFrames = render.value("num_frames", cps->render.numFrames);
		cps->render.initialRandomFrame = movement.value("initial_random_frame", cps->render.initialRandomFrame);
		cps->render.frameSpeed = render.value("frame_speed", cps->render.frameSpeed);
		cps->render.texture = Resources.get(render.value("texture", ""))->as<CTexture>();
		cps->render.mesh = Resources.get(render.value("mesh", "unit_quad_xy.mesh"))->as<CRenderMesh>();
		const std::string renderType = render.value("type", "billboard");

		bool additive = render.value("additive", false);
		std::string defaulRenderTechnique;
		if (renderType == "mesh") {
			cps->render.type = TCoreSystem::TRender::Mesh;
			defaulRenderTechnique = additive ? "particles_mesh_additive.tech" : "particles_mesh.tech";
		}
		else if (renderType == "horizontal_billboard") {
			cps->render.type = TCoreSystem::TRender::HorizontalBillboard;
			defaulRenderTechnique = additive ? "particles_additive.tech" : "particles.tech";
		}
		else if (renderType == "stretched_billboard") {
			cps->render.type = TCoreSystem::TRender::StretchedBillboard;
			cps->render.motionBlurAmount = render.value("motion_blur_amount", 0.1f);
			defaulRenderTechnique = additive ? "particles_stretched_additive.tech" : "particles_stretched.tech";
		}
		else {
			cps->render.type = TCoreSystem::TRender::Billboard;
			defaulRenderTechnique = additive ? "particles_additive.tech" : "particles.tech";
		}
		cps->render.technique = Resources.get(render.value("technique", defaulRenderTechnique))->as<CRenderTechnique>();

		// color
		const json& color = data["color"];
		cps->color.opacity = color.value("opacity", cps->color.opacity);
		for (auto& clr : color["colors"]) {
			float time = clr[0];
			VEC4 value = loadVEC4(clr[1]);
			cps->color.colors.set(time, value);
		}
		cps->color.colors.sort();

		// size
		const json& size = data["size"];
		cps->size.scale = size.value("scale", cps->size.scale);
		cps->size.scale_variation = size.value("scale_variation", cps->size.scale_variation);
		for (auto& sz : size["sizes"]) {
			float time = sz[0];
			float value = sz[1];
			cps->size.sizes.set(time, value);
		}
		cps->size.sizes.sort();

	}
}
