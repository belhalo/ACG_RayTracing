#include "depthshader.h"

#include "../core/utils.h"

DepthShader::DepthShader() :
    color(Vector3D(1, 0, 0))
{ }

DepthShader::DepthShader(Vector3D hitColor_, double maxDist_, Vector3D bgColor_) :
    Shader(bgColor_), maxDist(maxDist_), color(hitColor_)
{ }

Vector3D DepthShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{
    // TASK 3: Implement the Depth Integrator
    Intersection its;
    if (Utils::getClosestIntersection(r, objList, its)) {
        Vector3D intersection_pos = its.itsPoint;
        Vector3D camera_origin = r.o;

        float distance = (r.o - intersection_pos).length();

        return Vector3D(1.0, 1.0 - distance / 1, 3.0);
    }

    return color;
}
