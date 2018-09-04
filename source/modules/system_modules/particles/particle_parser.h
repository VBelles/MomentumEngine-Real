#pragma once

namespace Particles {
    struct TCoreSystem;

    class CParser {
    public:
        void parseFile(const std::string& file);
		void parseParticlesFile(const std::string& file, TCoreSystem* cps);
		void parseParticleSystem(const json& data, TCoreSystem* cps);
    };
}
