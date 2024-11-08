#pragma once

#include "Point.hpp"
#include "fundamentals/Vector2.hpp"

struct Spring {
    double  springConst;
    double  dampFact;
    double  naturalLength;
    PointId p1;
    PointId p2;

    void springHandler(Point& point1, Point& point2) const {
        Vec2 force = forceCalc(point1, point2);
        point1.f += force; // equal and opposite reaction
        point2.f -= force;
    }

    Vec2 forceCalc(const Point& point1, const Point& point2) const {
        Vec2 diff = point1.pos - point2.pos; // broken out alot "yes this is faster! really like 3x"
        double diffMag = diff.mag();
        if (diffMag < 1E-30) return {}; // prevent 0 length spring exploding sim
        Vec2   unitDiff = diff / diffMag;
        double ext      = diffMag - naturalLength;
        double springf  = -springConst * ext;                               // f = -ke hookes law
        double dampf    = unitDiff.dot(point2.vel - point1.vel) * dampFact; // damping force
        return (springf + dampf) * unitDiff;
    }

    // represent spring as a string and push through stream
    friend std::ostream& operator<<(std::ostream& os, const Spring& s) {
        return os << s.springConst << ' ' << s.naturalLength << ' ' << s.dampFact << ' ' << s.p1
                  << ' ' << s.p2;
    }

    // create spring from string stream
    friend std::istream& operator>>(std::istream& is, Spring& s) {
        safeStreamRead(is, s.springConst);
        safeStreamRead(is, s.naturalLength);
        safeStreamRead(is, s.dampFact);
        safeStreamRead(is, s.p1);
        safeStreamRead(is, s.p2);
        if (is.good()) {
            throw std::runtime_error("To many columns for a spring - file invalid");
        }
        return is;
    }
};

using SpringId = Ref<Spring>;