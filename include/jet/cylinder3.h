// Copyright (c) 2016 Doyub Kim

#ifndef INCLUDE_JET_CYLINDER3_H_
#define INCLUDE_JET_CYLINDER3_H_

#include <jet/surface3.h>

namespace jet {

//!
//! \brief 3-D cylinder geometry.
//!
//! This class represents 3-D cylinder geometry which extends Surface3 by
//! overriding surface-related queries. The cylinder is aligned with the y-axis.
//!
class Cylinder3 final : public Surface3 {
 public:
    class Builder;

    //! Center of the cylinder.
    Vector3D center;

    //! Radius of the cylinder.
    double radius = 1.0;

    //! Height of the cylinder.
    double height = 1.0;

    //! Constructs a cylinder with
    explicit Cylinder3(bool isNormalFlipped = false);

    //! Constructs a cylinder with \p center, \p radius, and \p height.
    Cylinder3(
        const Vector3D& center,
        double radius,
        double height,
        bool isNormalFlipped = false);

    //! Copy constructor.
    Cylinder3(const Cylinder3& other);

    //! Returns the closest point from the given point \p otherPoint to the
    //! surface.
    Vector3D closestPoint(const Vector3D& otherPoint) const override;

    //! Returns the closest distance from the given point \p otherPoint to the
    //! point on the surface.
    double closestDistance(const Vector3D& otherPoint) const override;

    //! Returns true if the given \p ray intersects with this cylinder object.
    bool intersects(const Ray3D& ray) const override;

    //! Returns the bounding box of this cylinder object.
    BoundingBox3D boundingBox() const override;

    //! Returns builder fox Cylinder3.
    static Builder builder();

 protected:
    Vector3D actualClosestNormal(const Vector3D& otherPoint) const override;

    SurfaceRayIntersection3 actualClosestIntersection(
        const Ray3D& ray) const override;
};

//! Shared pointer type for the Cylinder3.
typedef std::shared_ptr<Cylinder3> Cylinder3Ptr;


//!
//! \brief Front-end to create Cylinder3 objects step by step.
//!
class Cylinder3::Builder final
    : public SurfaceBuilderBase3<Cylinder3::Builder> {
 public:
    //! Returns builder with center.
    Builder& withCenter(const Vector3D& center);

    //! Returns builder with radius.
    Builder& withRadius(double radius);

    //! Returns builder with height.
    Builder& withHeight(double height);

    //! Builds Cylinder3.
    Cylinder3 build() const;

    //! Builds shared pointer of Cylinder3 instance.
    Cylinder3Ptr makeShared() const;

 private:
    Vector3D _center{0, 0, 0};
    double _radius = 1.0;
    double _height = 1.0;
};

}  // namespace jet


#endif  // INCLUDE_JET_CYLINDER3_H_
