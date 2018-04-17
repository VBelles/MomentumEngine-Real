#pragma once

class CCurve : public IResource {
public:
	enum class EType { UNKNOWN = 0, CATMULL_ROM };

    CCurve() = default;
    bool load(const std::string& fileName);
    bool load(const json& j);
    void clear();
    void addKnot(const VEC3& point);
    void setType(const std::string& typeName);
    void setLoop(const bool value);

    VEC3 evaluate(float ratio) const;
    VEC3 evaluateAsCatmull(float ratio) const;

private:
    std::vector<VEC3> _knots;
    EType _type = EType::UNKNOWN;
	bool _loop = false;
};
