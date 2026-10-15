#include "intersectionshader.h"
#include "../core/utils.h"

IntersectionShader::IntersectionShader() :
    hitColor(Vector3D(1, 0, 0))
{ }

IntersectionShader::IntersectionShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{ }

Vector3D IntersectionShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{
    // TASK 2: Implement the Intersection Integrator

    //if (Utils::hasIntersection(r, objList))
    //    return Vector3D(0.0, 1.0, 0.0);


    // TASK 4: Implement the Normal Integrator
    
    Intersection its;
    if (Utils::getClosestIntersection(r, objList, its)) {
        return Vector3D(its.normal + Vector3D(1.0, 1.0, 1.0)) / 2.0;
    }
    return bgColor;
};
