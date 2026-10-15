#include "whittedintegrator.h"


#include "../core/utils.h"

Whittedintegrator::Whittedintegrator() {};
Vector3D Whittedintegrator::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{
    // search for the intersection
    Intersection its;
    if (!Utils::getClosestIntersection(r, objList, its)) return Vector3D(0.0, 0.0, 0.0); 

    const Material& objMaterial = its.shape->getMaterial();

    Vector3D outgoingLight(0.0, 0.0, 0.0);

    Vector3D x = its.itsPoint;
    Vector3D n = its.normal.normalized();
    Vector3D wo = (r.d - x).normalized();

    // (1) hasSpecular() case
    const int maxDepth = 10; // defines the max value to avoid infinit loops
    if (objMaterial.hasSpecular() && r.depth < maxDepth) {
        Vector3D wr = Utils::computeReflectionDirection(r.d, its.normal);

        Ray reflectionRay(its.itsPoint, wr, r.depth + 1);
        reflectionRay.minT = 1e-3;
        outgoingLight = computeColor(reflectionRay, objList, lsList);
    }

    // (2) hasTransmission() case
    if (objMaterial.hasTransmission()) {
        if (r.depth >= maxDepth)
            return Vector3D(0.0, 0.0, 0.0);

        Vector3D nGeom = its.normal.normalized();
        bool entering = dot(r.d, nGeom) < 0.0;
        Vector3D n = entering ? nGeom : -nGeom;

        // mu_t = eta_1 / eta_2
        double mu = objMaterial.getIndexOfRefraction();

        if (!entering) mu = 1.0 / mu; // invert mu

        Vector3D wo = (-r.d).normalized();
        double cosTheta = dot(n, wo);

        // total internal reflection
        double radicand = 1.0 - mu * mu * (1.0 - cosTheta * cosTheta);

        if (radicand < 0.0) {
            Vector3D wr = Utils::computeReflectionDirection(r.d, n);

            Ray reflectionRay(its.itsPoint, wr, r.depth + 1);
            reflectionRay.minT = 1e-3;

            return computeColor(reflectionRay, objList, lsList);
        }

        // compute the transmission direction
        Vector3D wt = -mu * wo + n * (mu * cosTheta - std::sqrt(radicand));
        wt = wt.normalized();

        Ray refracRay(its.itsPoint, wt, r.depth + 1);
        refracRay.minT = 1e-3;

        return computeColor(refracRay, objList, lsList);
    }

    // defines the rest of elements to compute the outgoing light
    Vector3D ambientConstant(0.1, 0.1, 0.1);
    Vector3D diffuseCoefficient = objMaterial.getDiffuseReflectance();

    Vector3D ambientLight = ambientConstant * diffuseCoefficient;

    // (3) hasDiffuseOrGlossy() case
    if (objMaterial.hasDiffuseOrGlossy()) {
        for (size_t i = 0; i < lsList.size(); i++) {

            Vector3D lightPosition = lsList[i]->sampleLightPosition();

            Vector3D wi = lightPosition - x;
            double distLight = wi.length();
            Vector3D normWi = wi.normalized();

            double dotProduct = std::max(0.0, dot(n, normWi));

            if (dotProduct <= 0.0)
                continue;

            Ray s(x, normWi);
            s.minT = 0.0001;
            s.maxT = distLight - s.minT;

            double visibilityFactor = 1.0;

            if (Utils::hasIntersection(s, objList)) visibilityFactor = 0.0;

            Vector3D fs = objMaterial.getReflectance(n, wo, normWi);

            Vector3D lightIntensity = lsList[i]->getIntensity();

            outgoingLight += fs * dotProduct * lightIntensity * visibilityFactor;
        }
    }
    return outgoingLight + ambientLight;
}