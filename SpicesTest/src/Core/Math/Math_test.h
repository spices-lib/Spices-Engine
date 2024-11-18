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
	* @brief Testing Math::DecomposeTransform.
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

			std::cout << "H" << std::endl;
		}

		{
			glm::quat rotationQuat({ -0.7898206, 0.613338, 0, 0 });
			glm::mat4 matrix = glm::toMat4(rotationQuat);

			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;

			Spices::DecomposeTransform(matrix, position, rotation, scale);

			rotation = glm::vec3(glm::degrees(rotation.x), glm::degrees(rotation.y), glm::degrees(rotation.z));

			std::cout << "H" << std::endl;
		}
	}
}
