#pragma once

#include "particle.h"
#include "utils/track.h"
#include "resources/resource.h"

namespace Particles {

	struct TCoreSystem : public IResource {

		struct TLife {
			float duration = 1.f;            // expected particle life time
			float durationVariation = 0.f;   // lifetime variation
			int maxParticles = 1;            // maximum number of alive particles
			float timeFactor = 1.f;
		};

		struct TEmission {
			enum EType { Point, Line, Square, Box, Sphere, Circle, Cylinder };

			EType type = Point;				// type of emissor
			float interval = 0.f;			// generation interval
			int count = 1;					// number of particles each generation
			bool cyclic = false;			// system re-emits after the interval time
			float size = 0.f;				// emissor size
			float halfLength = 0.f;			// emissor half length
			float angle = 0.f;				// emission angle
			bool radial = false;			// ignores angle and emits from the center
			bool randomDirection = false;   // direction generated randomly
			float duration = 0.f;			// system emitting time
		};

		struct TMovement {
			float velocity = 0.f;				// initial speed
			float velocityVariation = 0.f;		// variation of the initial speed generation
			float acceleration = 0.f;			// acceleration
			float initialRotation = 0;			// initial rotation
			bool initialRandomRotation = false;	// sets random rotation when emitted
			bool initialTargetRotation = false;	// sets target rotation when emitted (only with meshes)
			float spin = 0.f;					// rotation speed (radians)
			VEC3 spin_axis = { 0.f, 1.f, 0.f };	// spin axis
			float gravity = 0.f;				// gravity factor
			float wind = 0.f;					// wind factor
			bool ground = false;				// limit by ground
			bool following = false;				// follows its emissor target
			std::string followTarget;			// target to follow each particle
			VEC3 followTargetOffset;			// target to follow offset
		};

		struct TRender {
			enum EType { Billboard, HorizontalBillboard, StretchedBillboard, Mesh };

			EType type = Billboard;							// particle type
			const CTexture* texture = nullptr;				// particle texture
			const CRenderMesh* mesh = nullptr;				// mesh for mesh particles
			const CRenderTechnique* technique = nullptr;	// technique for particle rendering
			VEC2 frameSize = VEC2(1, 1);					// size of frame in the texture (in UV coords)
			int numFrames = 1;								// number of animation frames
			int initialFrame = 0;							// initial frame
			bool initialRandomFrame = false;				// ignores initialFrame
			float frameSpeed = 0.f;							// frame change speed
			float motionBlurAmount = 0.f;					// amount of blur for stretched particles
			bool glow = false;
		};

		struct TSize {
			TTrack<float> sizes;			// track of sizes along the particle lifetime
			float scale = 1.f;				// scale factor
			float scale_variation = 0.f;	// variation of scale at generation
		};

		struct TColor {
			TTrack<VEC4> colors;	// track of colors along the particle lifetime
			float opacity = 1.f;	// opacity factor
		};

		TLife         life;
		TEmission     emission;
		TMovement     movement;
		TRender       render;
		TSize         size;
		TColor        color;

		void debugInMenu() override;
		void onFileChanged(const std::string& filename) override;
	};

	using VParticles = std::vector<TParticle>;

	class CSystem {
	private:
		void emit();
		VEC3 generatePosition() const;
		VEC3 generateVelocity(const VEC3& position) const;

		MAT44 world;
		VEC3 targetPosition;
		QUAT targetRotation;

		LaunchConfig config;
		int boneId = -1;
		CHandle followTargetTransform;

		TParticleHandle		_handle;
		const TCoreSystem*  _core = nullptr;
		VParticles          _particles;
		float               _time = 0.f;
		float               _globalTime = 0.f;
		float               _fadeDuration = 0.f;
		float               _fadeTime = 0.f;
		CHandle             particleEntityHandle; //Handle of the entity

		void updateWorld();

	public:
		CSystem(TParticleHandle handle, const TCoreSystem* core, CHandle particleHandle, LaunchConfig config);
		bool update(float delta);
		void render();
		void launch();
		TParticleHandle getHandle() const;
		void fadeOut(float duration);
		void setOffset(VEC3 offset);
		void setRotationOffset(QUAT rotationOffset);
		CHandle getParticleEntityHandle();
		void debugInMenu();
		int getNbParticles();
		const TCoreSystem* getCore();
		void renderDebug();
	};
}
