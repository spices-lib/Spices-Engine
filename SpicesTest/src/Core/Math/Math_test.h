/**
* @file Math_test.h.
* @brief The Math_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Math/Math.h>
#include "Instrumentor.h"

#include <glm/gtx/quaternion.hpp>

namespace SpicesTest {

	/**
	* @brief Testing UInt2.
	*/
	TEST(Math_test, UInt2) {

		SPICESTEST_PROFILE_FUNCTION();

		Spices::UInt2 u0(1, 2);
		Spices::UInt2 u1(1, 2);
		Spices::UInt2 u2(1, 3);

		EXPECT_EQ(u0 == u1, true);
		EXPECT_EQ(u0 == u2, false);

		std::unordered_map<Spices::UInt2, bool> maps;

		maps[u0] = true;
		EXPECT_EQ(maps.size(), 1);

		maps[u1] = true;
		EXPECT_EQ(maps.size(), 1);

		maps[u2] = true;
		EXPECT_EQ(maps.size(), 2);
	}

	/**
	* @brief Testing String2.
	*/
	TEST(Math_test, String2) {

		SPICESTEST_PROFILE_FUNCTION();

		Spices::String2 u0("1", "2");
		Spices::String2 u1("1", "2");
		Spices::String2 u2("1", "3");

		EXPECT_EQ(u0 == u1, true);
		EXPECT_EQ(u0 == u2, false);

		std::unordered_map<Spices::String2, bool> maps;

		maps[u0] = true;
		EXPECT_EQ(maps.size(), 1);

		maps[u1] = true;
		EXPECT_EQ(maps.size(), 1);

		maps[u2] = true;
		EXPECT_EQ(maps.size(), 2);
	}

	/**
	* @brief Testing DecomposeTransform.
	*/
	TEST(Math_test, DecomposeTransform) {

		SPICESTEST_PROFILE_FUNCTION();

		{
			glm::mat4 matrix = {
				-1, 0,  0, 0,
				 0, 1,  0, 0,
				 0, 0, -1, 0,
				 0, 0,  0, 1
			};
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;

			Spices::DecomposeTransform(matrix, position, rotation, scale);

			rotation = glm::vec3(glm::degrees(rotation.x), glm::degrees(rotation.y), glm::degrees(rotation.z));
		}

		{
			glm::mat4 matrix = {
				1, 0, 0, 0, 
				0, 1, 0, 0, 
				0, 0, 1, 0, 
				0, 0, 0, 1
			};
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;

			Spices::DecomposeTransform(matrix, position, rotation, scale);

			rotation = glm::vec3(glm::degrees(rotation.x), glm::degrees(rotation.y), glm::degrees(rotation.z));
		}

		{
			glm::quat rotationQuat = glm::quat(0.5, -0.5, -0.5, -0.5);
			glm::mat4 matrix = glm::toMat4(rotationQuat);

			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;

			Spices::DecomposeTransform(matrix, position, rotation, scale);

			rotation = glm::vec3(glm::degrees(rotation.x), glm::degrees(rotation.y), glm::degrees(rotation.z));
		}

		{
			glm::quat rotationQuat({ -0.7898206, 0.613338, 0, 0 });
			glm::mat4 matrix = glm::toMat4(rotationQuat);

			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;

			Spices::DecomposeTransform(matrix, position, rotation, scale);

			rotation = glm::vec3(glm::degrees(rotation.x), glm::degrees(rotation.y), glm::degrees(rotation.z));
		}
	}
}
