#include "RigidBody.h"

#include "factory.hpp"

using std::string;
using namespace Eigen;

string RigidBody::getDocs(){
    return string(
"System representing a rigid body in 3D\n\n"
);
}

RigidBody::RigidBody(void) :
    force(pysim::vector::Zero(3)),
    moment(pysim::vector::Zero(3)),
    mass(1.0),
    inertia(pysim::matrix::Zero(3, 3)),
    cog(pysim::vector::Zero(3)),
    position(pysim::vector::Zero(3)), d_pos(pysim::vector::Zero(3)),
    velocity(pysim::vector::Zero(3)), d_velocity(pysim::vector::Zero(3)),
    attitude(pysim::vector::Zero(3)), d_att(pysim::vector::Zero(3)),
    rotation_vel(pysim::vector::Zero(3)), d_rotation_vel(pysim::vector::Zero(3))

{
    INPUT(force, "Force applied to the body, in body coordinates");
    INPUT(moment, "Moment applied to the body, in body coordinates");
    INPUT(mass, "The total mass of the body");
    //INPUT(inertia, "Inertia of the rigid body");
    INPUT(cog, "Center of gravity of the body");

    STATE(position, d_pos, "Position of the body in global coordinates");
    STATE(velocity, d_velocity, "Velocity of the body in local coordinates");
    STATE(attitude, d_att, "Attitude of the body");
    STATE(rotation_vel, d_rotation_vel, "Rotational velocity of the body");

    //OUTPUT(acceleration, "Acceleration of the body [m/s^2]");
}

//First calculation in a simulation, only done once
void RigidBody::preSim() {
};

void RigidBody::doStep(double time) {
    using namespace Eigen;
    RowVector3d euler = d_rotation_vel.head<3>().cross(cog.head<3>());
    RowVector3d coriolis = rotation_vel.head<3>().cross(velocity.head<3>());
    RowVector3d centrifugal = rotation_vel.head<3>().cross(rotation_vel.head<3>().cross(cog.head<3>()));
    RowVector3d acc = force / mass;
    d_velocity = acc;// -euler - coriolis - centrifugal;
    Matrix3d m;
    m = AngleAxisd(attitude(2), Vector3d::UnitZ()),    //Yaw
        AngleAxisd(attitude(1), Vector3d::UnitY()),    //Pitch
        AngleAxisd(attitude(0), Vector3d::UnitX());    //Roll
    d_pos = m * velocity.transpose();
}

REGISTER_SYSTEM(RigidBody);

