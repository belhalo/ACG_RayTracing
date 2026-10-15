#ifndef TRANSMISSIVEMATERIAL
#define TRANSMISSIVEMATERIAL

#include "material.h"

class Transmissive : public Material
{
public:
    explicit Transmissive(double mu_t);

    Vector3D getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const;

    bool hasSpecular() const { return true; }
    bool hasTransmission() const { return true; }
    bool hasDiffuseOrGlossy() const { return false; }
    bool isEmissive() const { return false; }

    double getIndexOfRefraction() const;
    Vector3D getEmissiveRadiance() const;
    Vector3D getDiffuseReflectance() const;

private:
    double mu_t;
};

#endif