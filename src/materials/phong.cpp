#include "phong.h"
#include "material.h"
#include "core\vector3d.h"
#include "cmath"

#include <iostream>

Phong::Phong()
{
}

Phong::Phong(Vector3D Kd_, Vector3D Ks_, float alpha_) :
    rho_d(Kd_), Ks(Ks_), alpha(alpha_) {
}

Vector3D Phong::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const {
    //we got n, wo, wi
    double pi = 2 * asin(1.0); //PI is also in "corecrt.h" search it next day!

    Vector3D wr = Vector3D(2 * (n * wi) * n - wi.normalized()).normalized();
    double maxdotProduct = std::max(0.0, dot(wo.normalized(), wr));

    //rho_d, Ks, alpha inherited from material
    double dotProduct = dot(wo, wr);

    //ρd / π + ks(ωo · ωr)^α
    Vector3D vectorReflectance = Vector3D((rho_d / pi) + Ks * pow((float)maxdotProduct, (float)alpha));

    return vectorReflectance;
};

double Phong::getIndexOfRefraction() const
{
    std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
        << std::endl;

    return -1;
}


Vector3D Phong::getEmissiveRadiance() const
{
    return Vector3D(0.0);
}


Vector3D Phong::getDiffuseReflectance() const
{
    return rho_d;
}

