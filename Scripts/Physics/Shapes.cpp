#include "Shapes.h"

#include <algorithm>
#include <functional>

#include <glm/gtx/norm.hpp>

#include "Utils\DynamicDispatching.h"

#include "Utils/SimpleMath.h"

namespace physics
{
	Line::Line(const glm::vec2& origin, const glm::vec2& direction)
	{
		glm::vec2 dir = { direction.y, -direction.x };
		
		C = -glm::dot(origin, dir);
		A = dir.x;
		B = dir.y;
	}

	std::optional<glm::vec2> Line::IntersectWith(const Line& other) const
	{
		if (glm::dot(glm::vec2(A, B), glm::vec2(other.B, -other.A)) == 0)
		{
			return std::nullopt;
		}

		if (A != 0.0f)
		{ 
			float k = other.A / A;
			float c = other.C - k * C;
			k = other.B - k * B;

			float y = -c / k;
			float x = (-C - B * y) / A;
			return { {x, y} };
		}
		
		float k = A / other.A;
		float c = C - k * other.C;
		k = B - k * other.B;

		float y = -c / k;
		float x = (-other.C - other.B * y) / other.A;
		return { {x, y} };
	}

	std::optional<glm::vec2> Ray::IntersectWith(const Line& line) const
	{
		Line tLine(origin, direction);
		auto intersect = line.IntersectWith(tLine);

		if (!intersect)
		{
			return std::nullopt;
		}

		glm::vec2 dir = *intersect - origin;
		if (glm::dot(dir, direction) > 0)
		{
			return intersect;
		}

		return std::nullopt;
	}

	std::optional<glm::vec2> Ray::IntersectWith(const Ray& ray) const
	{
		Line line(ray.origin, ray.direction);
		auto intersect = IntersectWith(line);

		if (!intersect)
		{
			return std::nullopt;
		}

		glm::vec2 dir = *intersect - ray.origin;
		if (glm::dot(dir, direction) > 0)
		{
			return intersect;
		}

		return std::nullopt;
	}

	std::optional<glm::vec2> Interval::IntersectWith(const Line& line) const
	{
		auto tLine = Line(start, end - start);
		auto intersect = tLine.IntersectWith(line);

		if (!intersect)
		{
			return std::nullopt;
		}

		glm::vec2 dir1 = *intersect - start;
		glm::vec2 dir2 = end - start;

		float d = glm::dot(dir1, dir2);

		if (0 < d && d < glm::length2(dir2))
		{
			return intersect;
		}
		return std::nullopt;
	}

	std::optional<glm::vec2> Interval::IntersectWith(const Ray& shape) const
	{
		auto rayLine = Line(shape.origin, shape.direction);
		auto collision = IntersectWith(rayLine);

		if (!collision)
		{
			return std::nullopt;
		}

		glm::vec2 dir3 = *collision - shape.origin;
		float d = glm::dot(dir3, shape.direction);

		if (0 < d)
		{
			return collision;
		}
		return std::nullopt;
	}
	
	std::optional<glm::vec2> Interval::IntersectWith(const Interval& shape) const
	{
		auto line = Line(shape.start, shape.end - shape.start);
		auto collision = IntersectWith(line);

		if (!collision)
		{
			return std::nullopt;
		}
		
		glm::vec2 dir3 = *collision - shape.start;
		glm::vec2 dir4 = shape.end - shape.start;

		float d = glm::dot(dir3, dir4);

		if (0 < d && d < glm::length2(dir4))
		{
			return collision;
		}
		return std::nullopt;
	}

	Square::Square(const glm::vec2& min, const glm::vec2& max) : min(min), max(max)
	{}

	glm::vec2 Square::Center() const
	{
		return (min + max) / 2.0f;
	}

	float Square::Size() const
	{
		return glm::length(max - min) / 2;
	}

	bool Square::IntersectWith(const Point& point) const
	{
		return point.x <= max.x && point.y <= max.y && point.x >= min.x && point.y >= min.y;
	}

	bool Square::IntersectWith(const Ray& ray) const
	{
		glm::vec2 corners[4] = { min, {max.x, min.y}, max, {min.x, max.y} };

		for (int i = 0; i < 4; ++i)
		{
			auto interval = Interval(corners[i], corners[(i + 1) % 4]);
			if (interval.IntersectWith(ray))
			{
				return true;
			}
		}

		return false;
	}

	Circle::Circle(glm::vec2 origin, float radius) : origin(origin), radius(radius)
	{}

	glm::vec2 Circle::Center() const
	{
		return origin;
	}

	float Circle::Size() const
	{
		return radius;
	}

	bool Circle::IntersectWith(const Point& point) const
	{
		return glm::length2(point - origin) <= std::powf(radius, 2.0f);
	}

	bool Circle::IntersectWith(const Ray& shape) const
	{
		auto line = Line(shape.origin, shape.direction);

		float len = (line.A * origin.x + line.B * origin.y + line.C);

		if (radius >= len)
		{
			glm::vec2 point = { origin.x - len * line.A, origin.y - len * line.B };
			if (len != radius)
			{
				float len2 = std::sqrt(radius * radius - len * len);
				glm::vec2 point1 = point + len2 * glm::vec2(line.B, -line.A);
				glm::vec2 point2 = point + len2 * glm::vec2(-line.B, line.A);

				if (glm::dot(point1 - shape.origin, shape.direction) > 0)
					return true;
				if (glm::dot(point2 - shape.origin, shape.direction) > 0)
					return true;
			}
			else
			{
				if (glm::dot(point - shape.origin, shape.direction) > 0)
					return true;
			}
		}
		return false;
	}


	glm::vec2 Circle::GetFarthestPoint(const glm::vec2& dir) const
	{
		return origin + glm::normalize(dir) * radius;
	}

	Polygon::Polygon(const std::vector<glm::vec2>& vert): vertices(vert)
	{
	}

	glm::vec2 Polygon::Center() const
	{
		glm::vec2 result = {0.0f, 0.0f};
		for (auto& vert : vertices)
		{
			result += vert;
		}
		result /= vertices.size();
		return result;
	}

	float Polygon::Size() const
	{
		return 0;
	}

	bool Polygon::IntersectWith(const Point& shape) const
	{
		Ray ray(shape, { 1.0f, 0.0f });
		int count = 0;
		for (int i = 0; i < vertices.size(); ++i)
		{
			Interval interval(vertices[i], vertices[(i + 1) % vertices.size()]);
			if (interval.IntersectWith(ray))
			{
				++count;
			}
		}
		return count == 1;
	}

	bool Polygon::IntersectWith(const Ray& ray) const
	{
		for (int i = 0; i < vertices.size(); ++i)
		{
			Interval interval(vertices[i], vertices[(i + 1) % vertices.size()]);
			if (interval.IntersectWith(ray))
			{
				return true;
			}
		}
		return false;
	}

	std::optional<Collision> IsCollision(const Square& A, const Square& B)
	{
		glm::vec2 dir = B.Center() - A.Center();

		float halfAx = (A.max.x - A.min.x) / 2;
		float halfBx = (B.max.x - B.min.x) / 2;

		float overlapX = halfAx + halfBx - std::abs(dir.x);

		if (overlapX >= 0)
		{
			float halfAy = (A.max.y - A.min.y) / 2;
			float halfBy = (B.max.y - B.min.y) / 2;

			float overlapY = halfAy + halfBy - std::abs(dir.y);

			if (overlapY >= 0)
			{
				if (overlapX >= overlapY)
				{
					glm::vec2 normal = glm::vec2{ 0.0f, 1.0f } *sgn(dir.y);

					float contactX = (std::max(A.min.x, B.min.x) + std::min(A.max.x, B.max.x)) / 2;

					Collision ToA = { {contactX, A.Center().y + halfAy * sgn(dir.y)}, normal, overlapY };
					return { ToA };
				}
				else
				{
					glm::vec2 normal = glm::vec2{ 1.0f, 0.0f } *sgn(dir.x);

					float contactY = (std::max(A.min.y, B.min.y) + std::min(A.max.y, B.max.y)) / 2;

					Collision ToA = { {contactY, A.Center().x + halfAx * sgn(dir.x)}, normal, overlapX };
					return { ToA };
				}

			}
		}
		return std::nullopt;
	}

	std::optional<Collision> IsCollision(const Circle& A, const Circle& B)
	{
		glm::vec2 dir = B.origin - A.origin;
		float dist = glm::length2(dir);

		if (dist > std::powf(A.radius + B.radius, 2.0f))
		{
			return std::nullopt;
		}

		if (dist != 0)
		{
			dir = glm::normalize(dir);
			dist = std::sqrtf(dist);
			float penetration = std::abs(A.radius - (dist - B.radius));
			Collision ToA = { A.origin + dir * A.radius, dir, penetration };
			return { ToA };
		}
		else
		{
			Collision ToA = { A.origin, {0.0f, 1.0f}, A.radius };
			return { ToA };
		}
	}

	std::optional<Collision> IsCollision(const Square& A, const Circle& B)
	{
		glm::vec2 closest = { std::clamp(B.origin.x, A.min.x, A.max.x),  std::clamp(B.origin.y, A.min.y, A.max.y) };
		
		glm::vec2 normal = B.origin - closest;
		float penetration = B.radius - glm::length(normal);

		if (penetration < 0.0f)
		{
			return std::nullopt;
		}

		normal = glm::normalize(normal);

		Collision ToA = { closest, normal, penetration };

		return { ToA };
	}

	std::optional<Collision> IsCollision(const Circle& A, const Square& B)
	{
		auto res = IsCollision(B, A);
		if (res)
		{
			return { {res->pos, -res->normal, res->penetration} };
		}
		return res;
	}

	int Polygon::GetFarthestPointIndex(const glm::vec2& dir) const
	{
		float maxProj = -FLT_MAX;
		int idx = -1;
		for (int i = 0; i < vertices.size(); ++i)
		{
			float projection = glm::dot(vertices[i], dir);
			if (projection > maxProj)
			{
				idx = i;
				maxProj = projection;
			}
		}

		return idx;
	};

	GJKPoint GetSupport(const Polygon& polyA, const Polygon& polyB, const glm::vec2& direction)
	{
		int pA = polyA.GetFarthestPointIndex(direction);
		int pB = polyB.GetFarthestPointIndex(-direction);
		return { polyA.vertices[pA] - polyB.vertices[pB], pA, pB };
	};

	GJKSimplex& GJKSimplex::operator=(const std::initializer_list<GJKPoint>& list)
	{
		size = 0;
		for (auto& point : list)
		{
			points[size] = point;
			++size;
		}
				
		return *this;
	}

	void GJKSimplex::Push(const GJKPoint& point)
	{
		points = { point, points[0], points[1] };
		size = std::min(size + 1, StackSize);
	}

	GJKPoint& GJKSimplex::operator[](int i)
	{
		return points[i];
	}

	GJKPoint GJKSimplex::operator[](int i) const
	{
		return physics::GJKSimplex::points[i];
	}

	bool ProcessLine(GJKSimplex& simplex, glm::vec2& direction)
	{
		const glm::vec2 ab = simplex[1].pos - simplex[0].pos;
		const glm::vec2 ao = -simplex[0].pos;

		if (glm::dot(ab, ao) > 0) 
		{
			direction = orthogonalize(ab, ao);
		}
		else 
		{
			simplex = { simplex[0] };
			direction = ao;
		}

		return false;
	}

	bool ProcessTriangle(GJKSimplex& simplex, glm::vec2& direction)
	{
		glm::vec2 ab = simplex[1].pos - simplex[0].pos;
		glm::vec2 ac = simplex[2].pos - simplex[0].pos;
		glm::vec2 ao = -simplex[0].pos;

		if (glm::dot(-orthogonalize(ac, ab), ao) > 0)
		{
			simplex = { simplex[2], simplex[0] };
			return ProcessLine(simplex, direction);
		}
		else if (glm::dot(-orthogonalize(ab, ac), ao) > 0)
		{
			simplex = { simplex[1], simplex[0] };
			return ProcessLine(simplex, direction);
		}

		return true;
	}

	bool NextSimplex(GJKSimplex& simplex, glm::vec2& direction)
	{
		switch (simplex.size)
		{
			case 2: return ProcessLine(simplex, direction);
			case 3: return ProcessTriangle(simplex, direction);
		}

		return false;
	}

	std::optional<GJKSimplex> GJK(const Polygon& A, const Polygon& B)
	{
		GJKSimplex simplex;

		GJKPoint support = GetSupport(A, B, glm::vec2(1.0f, 0.0f));
		simplex.Push(support);
		glm::vec2 direction = -support.pos;

		while (true) {
			support = GetSupport(A, B, direction);

			if (glm::dot(support.pos, direction) <= 0) 
			{
				return std::nullopt;
			}

			simplex.Push(support);

			if (NextSimplex(simplex, direction))
			{
				return {simplex};
			}
		}

		return std::nullopt;
	}

	int CorrectMod(int a, int N) 
	{
		int res = a % N;
		if (res < 0)
		{
			res = N - res;
		}
		return res;
	}

	glm::vec2 GetCollisionPoint(const Polygon& A, const Polygon& B, const GJKPoint& p1, const GJKPoint& p2)
	{
		glm::vec2 result{0.0f, 0.0f};
		if (p1.pA == p2.pA)
		{
			Interval intervalA1{ A.vertices[CorrectMod(p1.pA - 1, A.vertices.size())], A.vertices[p1.pA] };
			Interval intervalA2{ A.vertices[CorrectMod(p1.pA + 1, A.vertices.size())], A.vertices[p1.pA] };
			Interval intervalB{ B.vertices[p1.pB], B.vertices[p2.pB] };

			result += intervalA1.IntersectWith(intervalB).value();
			result += intervalA2.IntersectWith(intervalB).value();
		}
		else if (p1.pB == p2.pB)
		{
			Interval intervalB1{ B.vertices[CorrectMod(p1.pB - 1, B.vertices.size())], B.vertices[p1.pB] };
			Interval intervalB2{ B.vertices[CorrectMod(p1.pB + 1, B.vertices.size())], B.vertices[p1.pB] };
			Interval intervalA{ A.vertices[p1.pA], A.vertices[p2.pA] };

			result += intervalB1.IntersectWith(intervalA).value();
			result += intervalB2.IntersectWith(intervalA).value();
		}
		else
		{
			Interval intervalA{ A.vertices[p1.pA], A.vertices[p2.pA] };
			Interval intervalB{ B.vertices[p1.pB], B.vertices[p2.pB] };

			int p1A = std::max(p1.pA, p2.pA);
			int p2A = std::min(p1.pA, p2.pA);

			int p1B = std::max(p1.pB, p2.pB);
			int p2B = std::min(p1.pB, p2.pB);

			Interval intervalBL{ B.vertices[p1B], B.vertices[CorrectMod(p1B + 1, B.vertices.size())] };
			Interval intervalBR{ B.vertices[p2B], B.vertices[CorrectMod(p2B - 1, B.vertices.size())] };

			result += intervalA.IntersectWith(intervalBL).value_or(glm::vec2{ 0.0f, 0.0f });
			result += intervalA.IntersectWith(intervalBR).value_or(glm::vec2{ 0.0f, 0.0f });

			Interval intervalAL{ A.vertices[p1A], A.vertices[CorrectMod(p1A + 1, A.vertices.size())] };
			Interval intervalAR{ A.vertices[p2A], A.vertices[CorrectMod(p2A - 1, A.vertices.size())] };

			result += intervalB.IntersectWith(intervalAL).value_or(glm::vec2{ 0.0f, 0.0f });
			result += intervalB.IntersectWith(intervalAR).value_or(glm::vec2{ 0.0f, 0.0f });
		}
		return result / 2.0f;
	}

	std::optional<Collision> EPA(const GJKSimplex& simplex, const Polygon& A, const Polygon& B) {
		int minIndex = 0;
		float minDistance = FLT_MAX;
		glm::vec2 minNormal;

		std::vector<GJKPoint> poly;
		for (int i = 0; i < simplex.size; ++i)
		{
			poly.push_back(simplex[i]);
		}

		while (minDistance == FLT_MAX) {
			for (int i = 0; i < poly.size(); i++)
			{
				int j = (i + 1) % poly.size();

				glm::vec2 dir = poly[j].pos - poly[i].pos;

				glm::vec2 normal = glm::normalize(normalExternalCCV(dir));
				float distance = glm::dot(normal, poly[i].pos);

				normal *= sgn(distance); // becouse point is not ccv
				distance *= sgn(distance);

				if (distance < minDistance)
				{
					minDistance = distance;
					minNormal = normal;
					minIndex = i;
				}
			}

			GJKPoint support = GetSupport(A, B, minNormal);
			float sDistance = glm::dot(minNormal, support.pos);

			if (std::abs(sDistance - minDistance) > 0.001f)  // change cmp by index
			{
				minDistance = FLT_MAX;
				poly.insert(std::next(poly.begin(),  minIndex + 1), support);
			}
		}

		return { {GetCollisionPoint(A, B, poly[minIndex], poly[(minIndex + 1) % poly.size()]), minNormal, minDistance } };
	}

	std::optional<Collision> IsCollision(const Polygon& A, const Polygon& B)
	{
		auto simplex = GJK(A, B);
		if (!simplex)
			return std::nullopt;

		return EPA(*simplex, A, B);
	}

	std::pair<std::optional<glm::vec2>, std::optional<glm::vec2>> Intersect(const Interval& interval, const Circle& circle)
	{
		glm::vec2 dirToPoint = circle.Center() - interval.start;

		float len = glm::length(interval.end - interval.start);
		glm::vec2 lineDir = (interval.end - interval.start) / len;

		float proj = glm::dot(lineDir, dirToPoint);
		glm::vec2 projPoint = interval.start + proj * lineDir;

		float h2 = glm::length2(dirToPoint) - proj * proj;

		float r2 = std::powf(circle.radius, 2.0f);
		if (h2 >= r2)
		{
			return { std::nullopt, std::nullopt };
		}

		float d = std::sqrt(r2 - h2);

		glm::vec2 p1 = lineDir * d + projPoint;
		glm::vec2 p2 = -lineDir * d + projPoint;

		float d1 = glm::dot(p1 - interval.start, lineDir);
		float d2 = glm::dot(p2 - interval.start, lineDir);

		if (0 < d1 && d1 < len && 0 < d2 && d2 < len)
		{
			return { p1, p2 };
		}
		else if (0 < d1 && d1 < len)
		{
			return { p1, std::nullopt };
		}
		else if (0 < d2 && d2 < len)
		{
			return { p2, std::nullopt };
		}

		return { std::nullopt, std::nullopt };
	}

	std::optional<Collision> IsCollision(const Polygon& A, const Circle& B)
	{
		float minDistance = -FLT_MAX;
		glm::vec2 minNormal;
		std::vector<glm::vec2> intersects; //fix

		for (int i = 0; i < A.vertices.size(); ++i)
		{
			int j = (i+1) % A.vertices.size();

			glm::vec2 normal = normalExternalCCV(A.vertices[j] - A.vertices[i]);

			glm::vec2 support = B.GetFarthestPoint(-normal);

			float dist = glm::dot(normal, support - A.vertices[i]);

			if (dist > minDistance)
			{
				minDistance = dist;
				minNormal = normal;
			}

			auto intersect = Intersect({ A.vertices[j], A.vertices[i] }, B);
			if (intersect.first)
			{
				intersects.push_back(*intersect.first);
				if (intersect.second)
				{
					intersects.push_back(*intersect.second);
				}
			}

			if (intersects.size() == 2)
			{
				break;
			}
		}

		if (intersects.size() != 2)
		{
			return std::nullopt;
		}

		return { { (intersects[0] + intersects[1]) / 2.0f, minNormal, minDistance} };
	}


	std::optional<Collision> Shape::GetCollision(const Shape& shape) const
	{
		std::function<std::optional<Collision>(const Square&, const Square&)> func1 =
			static_cast<std::optional<Collision>(*)(const Square&, const Square&)>(IsCollision);
		std::function<std::optional<Collision>(const Square&, const Circle&)> func2 =
			static_cast<std::optional<Collision>(*)(const Square&, const Circle&)>(IsCollision);
		std::function<std::optional<Collision>(const Circle&, const Circle&)> func3 =
			static_cast<std::optional<Collision>(*)(const Circle&, const Circle&)>(IsCollision);
		std::function<std::optional<Collision>(const Circle&, const Square&)> func4 =
			static_cast<std::optional<Collision>(*)(const Circle&, const Square&)>(IsCollision);

		auto result = DynamicDispatch(*this, shape, func1, func2, func3, func4);

		return result;
	}

	std::unique_ptr<Shape> ApplyTransformToShape(const Shape& shape, const glm::vec2& position, const glm::vec2& scale)
	{
		auto* circle = dynamic_cast<const Circle*>(&shape);
		auto* square = dynamic_cast<const Square*>(&shape);
		auto* polygon = dynamic_cast<const Polygon*>(&shape);
		std::unique_ptr<Shape> result;
		if (circle)
		{
			result = std::make_unique<Circle>(circle->Center() + position, circle->Size() * scale.x);
		}
		else if (square)
		{
			glm::vec2 diagMin = (square->min - square->Center()) * scale;
			glm::vec2 diagMax = (square->max - square->Center()) * scale;
			result = std::make_unique<Square>(square->Center() + diagMin + position, square->Center() + diagMax + position);
		}
		else if (polygon)
		{
			std::vector<glm::vec2> vertices;
			glm::vec2 center = polygon->Center();
			for (const auto& l : polygon->vertices)
			{
				vertices.push_back(center + (l - center) *scale + position);
			}
			result = std::make_unique<Polygon>(vertices);
		}
		return result;
	}
}