#include "cylinder.h"

namespace RayTracing
{

    void RayTracing::Cylinder::load(const YAML::Node& node)
    {
        Node::load(node);
        m_radius = node["cylinder"]["r"].as<double>();
        m_height = node["cylinder"]["h"].as<double>();
    }

    std::vector<Point> RayTracing::Cylinder::rayIntersetion(const Ray & ray) const
    {
        std::vector<Point> intersections;
        Ray tfRay = ray.transform(inverse(m_world));

        double a = tfRay.direction().x * tfRay.direction().x + tfRay.direction().y * tfRay.direction().y;
        double b = 2.0 * (tfRay.direction().x * tfRay.start().x + tfRay.direction().y * tfRay.start().y);
        double c = tfRay.start().x * tfRay.start().x + tfRay.start().y * tfRay.start().y - m_radius * m_radius;
        double d = b * b - 4 * a * c;

        if (d >= 0)
        {
            double t1 = (-b + sqrt(d)) / (2.0 * a);
            double t2 = (-b - sqrt(d)) / (2.0 * a);
            if (t1 > 0)
            {
                Point p = tfRay.eval(t1);
                if (p.z >= -m_height / 2 && p.z <= m_height / 2)
                    intersections.push_back(p.transform(m_world, 1.0));
            }
            if (t2 > 0)
            {
                Point p = tfRay.eval(t2);
                if (p.z >= -m_height / 2 && p.z <= m_height / 2)
                    intersections.push_back(p.transform(m_world, 1.0));
            }
        }

        Plane p1({ 0.0, 0.0, m_height / 2 }, { 0.0, 0.0, 1.0 });
        Plane p2({ 0.0, 0.0, -m_height / 2 }, { 0.0, 0.0, -1.0 });
        std::vector<Point> tmp = p1.rayIntersetion(tfRay);
        if (tmp.size() > 0 && tmp[0].x * tmp[0].x + tmp[0].y * tmp[0].y < m_radius * m_radius)
            intersections.push_back(tmp[0].transform(m_world, 1.0));
        tmp = p2.rayIntersetion(tfRay);
        if (tmp.size() > 0 && tmp[0].x * tmp[0].x + tmp[0].y * tmp[0].y < m_radius * m_radius)
            intersections.push_back(tmp[0].transform(m_world, 1.0));

        return intersections;
    }

    Point RayTracing::Cylinder::normal(const Point & point) const
    {
        Point norm;
        Point p = point.transform(inverse(m_world), 1.0);
        if (p.x * p.x + p.y * p.y < m_radius * m_radius - 1e-5)
            if (p.z > 0)
                norm = { 0.0, 0.0, 1.0 };
            else
                norm = { 0.0, 0.0, -1.0 };
        else
            norm = { p.x, p.y, 0.0 };
        norm = norm.transform(bq::transposed(bq::inverse(m_world)), 0.0);
        return norm;
    }

}
