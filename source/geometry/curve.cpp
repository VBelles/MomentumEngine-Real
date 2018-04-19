#include "mcv_platform.h"
#include "curve.h"
#include "resources/resource.h"

// ----------------------------------------------
class CCurveResourceClass : public CResourceClass {
public:
	CCurveResourceClass() {
		class_name = "Curves";
		extensions = { ".curve" };
	}
	IResource* create(const std::string& name) const override {
		dbg("Creating curve %s\n", name.c_str());
		CCurve* res = new CCurve();
		bool is_ok = res->load(name);
		assert(is_ok);
		return res;
	}
};

// A specialization of the template defined at the top of this file
// If someone class getResourceClassOf<CTexture>, use this function:
template<>
const CResourceClass* getResourceClassOf<CCurve>() {
	static CCurveResourceClass the_resource_class;
	return &the_resource_class;
}
// ----------------------------------------------

bool CCurve::load(const std::string& fileName) {
    auto jData = loadJson(fileName);

    return load(jData);
}

bool CCurve::load(const json& jData) {
    std::string typeName = jData["type"];

    setType(typeName);

    auto& jKnots = jData["knots"];
    for (auto& jKnot : jKnots) {
        VEC3 knot = loadVEC3(jKnot);
        addKnot(knot);
    }

	calculateRadius();

	setLoop(jData.value("loop", false));

    return true;
}

void CCurve::clear() {
	_knots.clear();
}

void CCurve::addKnot(const VEC3& point) {
	_knots.push_back(point);
}

void CCurve::setType(const std::string & typeName) {
    if (typeName == "catmull-rom") {
        _type = EType::CATMULL_ROM;
    }
	else if (typeName == "circular") {
		_type = EType::CIRCULAR;
	}
    else {
        _type = EType::UNKNOWN;
    }
}

void CCurve::setLoop(const bool value) {
    _loop = value;
}

void CCurve::calculateRadius() {
	// _knots[0] tiene que ser el centro del movimiento circular,
	// _knots[1] un punto cualquiera del c�rculo.
	if (_type == EType::CIRCULAR && _knots.size() >= 2) {
		_center = _knots[0];
		_radius = VEC3::Distance(_center, _knots[1]);
	}
}

VEC3 CCurve::evaluate(float ratio) const {
	if (_type == EType::CATMULL_ROM) {
		return evaluateAsCatmull(ratio);
	}
	else if (_type == EType::CIRCULAR) {
		return evaluateAsCircle(ratio);
	}
	return VEC3::Zero;
}

VEC3 CCurve::evaluateAsCatmull(float ratio) const {
    ratio = clamp(ratio, 0.f, 0.99999f);
    int nsegments = static_cast<int>(_knots.size()) - 3;
    float ratioPerSegment = 1.f / static_cast<float>(nsegments);
    int currentSegment = static_cast<int>(ratio / ratioPerSegment);
    float segmentRatio = fmodf(ratio, ratioPerSegment) / ratioPerSegment;

	int idx = currentSegment + 1;

	VEC3 p1 = _knots[idx - 1];
	VEC3 p2 = _knots[idx];
	VEC3 p3 = _knots[idx + 1];
	VEC3 p4 = _knots[idx + 2];

	return VEC3::CatmullRom(p1, p2, p3, p4, segmentRatio);
}

VEC3 CCurve::evaluateAsCircle(float ratio) const {
	//// rotation
	//float yaw, pitch;
	//VEC3 front = 


	//VEC3 direction = waypoints[currentWaypoint] - myPosition;
	//direction.Normalize();

	//getYawPitchFromVector(front, &yaw, &pitch);

	//// final values
	//VEC3 newFront = getVectorFromYawPitch(yaw, pitch);
	//VEC3 newPos = _center - newFront * _distance;


	return VEC3();
}
