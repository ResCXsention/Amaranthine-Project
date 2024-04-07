#include "mesh.hxx"

#ifndef NDEBUG
#include <cassert>
#include <vector>
#include <numbers>
#include <matrix.hxx>
#include <polar.hxx>
#include <util.hxx>
#endif

#include <glbinding/gl/gl.h>

namespace res
{
	
	Mesh::Mesh(
			const std::vector<float> vertices,
			const std::vector<float> normals,
			const std::vector<unsigned int> indices
			)
		:vertices{vertices}, normals{normals}, indices{indices}
	{
		gl::glGenVertexArrays(1, &vao);	
		gl::glGenBuffers(1, &vbo);
		gl::glGenBuffers(1, &ebo);
		gl::glGenBuffers(1, &nbo);

		gl::glBindVertexArray(vao);

		gl::glBindBuffer(gl::GL_ARRAY_BUFFER, vbo);
		gl::glBufferData(gl::GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), gl::GL_STATIC_DRAW);
		gl::glEnableVertexAttribArray(0);
		gl::glVertexAttribPointer(0, 3, gl::GL_FLOAT, false, 0, reinterpret_cast<void*>(0));

		gl::glBindBuffer(gl::GL_ARRAY_BUFFER, nbo);
		gl::glBufferData(gl::GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), gl::GL_STATIC_DRAW);
		gl::glEnableVertexAttribArray(1);
		gl::glVertexAttribPointer(1, 3, gl::GL_FLOAT, false, 0, reinterpret_cast<void*>(0));

		gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, ebo);
		gl::glBufferData(gl::GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), gl::GL_STATIC_DRAW);

		gl::glBindBuffer(gl::GL_ARRAY_BUFFER, 0);
		gl::glBindVertexArray(0);
		gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	unsigned int Mesh::get_vao() const
	{
		return vao;
	}

#if !defined(NDEBUG) && defined(LIB_MIDNIGHT_MATRIX) && defined(LIB_MIDNIGHT_POLAR) && defined(LIB_MIDNIGHT_UTIL)
	Mesh spherical_mesh(
		const float radius,
		const unsigned int longitudes,
		const unsigned int latitudes
		)
	{
		assert(radius > 0);
		assert(longitudes > 0);
		assert(latitudes > 0);

		const float lon_step{(2 * std::numbers::pi_v<float>) / longitudes};
		const float half_lon_step{lon_step / 2};
		const float lat_step{std::numbers::pi_v<float> / (latitudes + 1)};
		const float depression_to_point_up{-std::numbers::pi_v<float> / 2};

		std::vector<float> points;
		points.reserve((longitudes * latitudes + 2) * 3);
		// top point
		points.insert(points.begin(), {0, radius, 0});
		// formation points
		for (int lat{0}; lat < latitudes; ++lat) {
			const float current_lat{lat_step * (lat + 1)};
			for (int lon{0}; lon < longitudes; ++lon) {
				const midnight::Polar t{radius, lon * lon_step, depression_to_point_up + current_lat};
				midnight::Vector3 c{midnight::cartesian3(t)};
				if (std::fabs(c.entry(0, 0)) < 0.00001F) {
					c.entry(0, 0) = 0.0F;
				}
				if (std::fabs(c.entry(1, 0)) < 0.00001F) {
					c.entry(1, 0) = 0.0F;
				}
				if (std::fabs(c.entry(2, 0)) < 0.00001F) {
					c.entry(2, 0) = 0.0F;
				}
				points.push_back(c.entry(0, 0));
				points.push_back(c.entry(1, 0));
				points.push_back(c.entry(2, 0));
			}
		}
		// bottom point
		points.insert(points.end(), {0, -radius, 0});

		std::vector<unsigned int> indices;
		indices.reserve(longitudes * latitudes * 6);
		// connect top to first latitude
		for (int lon{0}; lon < longitudes; ++lon) {
			indices.push_back(0);
			indices.push_back(1 + lon);
			indices.push_back(1 + (lon != longitudes - 1 ? lon + 1 : 0));
		}
		// connect latitudes to each other
		if (latitudes > 1) {
			for (int lat{0}; lat < latitudes - 1; ++lat) {
				const unsigned int lat_offset{lat * longitudes};
				const unsigned int next_lat_offset{(1 + lat) * longitudes};
				for (int lon{0}; lon < longitudes; ++lon) {
					const int lon_wrap{lon != longitudes - 1 ? lon + 1 : 0};
					// first triangle
					indices.push_back(1 + lat_offset + lon);
					indices.push_back(1 + next_lat_offset + lon);
					indices.push_back(1 + next_lat_offset + lon_wrap);
					// second triangle to complete the rectangle
					indices.push_back(1 + lat_offset + lon);
					indices.push_back(1 + next_lat_offset + lon_wrap);
					indices.push_back(1 + lat_offset + lon_wrap);
				}
			}
		}
		// connect bottom to last latitude
		// the bottom point is defined as longitudes * latitudes + 1
		for (int lon{0}; lon < longitudes; ++lon) {
			indices.push_back(1 + (latitudes - 1) * longitudes + lon);
			indices.push_back(1 + longitudes * latitudes);
			indices.push_back(1 + (latitudes - 1) * longitudes + (lon != longitudes - 1 ? lon + 1 : 0));
		}
		
		std::vector<float> normals;
		normals.reserve((longitudes * latitudes + 2) * 3);
		std::vector<std::vector<unsigned int>> point_triangles(longitudes * latitudes + 2);
		point_triangles.front().reserve(longitudes * 3);
		point_triangles.back().reserve(longitudes * 3);
		for (auto t{point_triangles.begin() + 1}; t != point_triangles.end() - 1; ++t) {
			t->reserve(15);
		}
		// index triangles
		for (auto t{indices.begin()}; t != indices.end(); t += 3) {
			const unsigned int a{*t}, b{*(t + 1)}, c{*(t + 2)};
			point_triangles.at(a).push_back(a);
			point_triangles.at(a).push_back(b);
			point_triangles.at(a).push_back(c);
			point_triangles.at(b).push_back(a);
			point_triangles.at(b).push_back(b);
			point_triangles.at(b).push_back(c);
			point_triangles.at(c).push_back(a);
			point_triangles.at(c).push_back(b);
			point_triangles.at(c).push_back(c);
		}
		// compute normals
		for (int p{0}; p < longitudes * latitudes + 2; ++p) {
			midnight::Vector3 n;
			for (auto t{point_triangles.at(p).begin()}; t != point_triangles.at(p).end(); t += 3) {
				const unsigned int a{*t}, b{*(t + 1)}, c{*(t + 2)};
				const midnight::Vector3 v1{points.at(a * 3), points.at(a * 3 + 1), points.at(a * 3 + 2)};
				const midnight::Vector3 v2{points.at(b * 3), points.at(b * 3 + 1), points.at(b * 3 + 2)};
				const midnight::Vector3 v3{points.at(c * 3), points.at(c * 3 + 1), points.at(c * 3 + 2)};
				n += midnight::cross(v1 - v3, v2 - v3);
			}
			n = midnight::normalise(n);
			normals.push_back(n.entry(0, 0));
			normals.push_back(n.entry(1, 0));
			normals.push_back(n.entry(2, 0));
		}

		return Mesh(points, normals, indices);
	}
#endif
}
