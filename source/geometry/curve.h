#pragma once

class CCurve : public IResource {
public:
	enum class EType { UNKNOWN = 0, CATMULL_ROM, CIRCULAR };

    CCurve() = default;
    bool load(const std::string& fileName);
    bool load(const json& j);
    void clear();
    void addKnot(const VEC3& point);
    void setType(const std::string& typeName);
    void setLoop(const bool value);
    bool isLooping() { return _loop; }
	void calculateRadius();
    float getRadius() { return _radius; }
    VEC3 getCenter() { return _center; }
    EType getType() { return _type; }

    VEC3 evaluate(float ratio, VEC3 pos = VEC3::Zero) const;
	VEC3 evaluateAsCatmull(float ratio) const;
	VEC3 evaluateAsCircle(float ratio, VEC3 pos) const;

private:
    std::vector<VEC3> _knots;
	VEC3 _center = VEC3::Zero;
	float _radius = 1.0f;
    EType _type = EType::UNKNOWN;
	bool _loop = false;
};
