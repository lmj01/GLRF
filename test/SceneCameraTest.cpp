#include <gtest/gtest.h>
#include <iostream>

#include <SceneCamera.hpp>

using namespace GLRF;

TEST (SceneCameraGemeraton, Generation) {
    glm::vec3 position(0);
    glm::vec3 upVector(0, 1, 0);
    glm::vec3 target(0, 0, 2);
    SceneCamera cam(position, upVector, target);
    glm::vec3 w = cam.getW();
    ASSERT_TRUE(w == glm::vec3(0, 0, -1));
    ASSERT_TRUE(cam.getUpVector() == upVector);
    ASSERT_TRUE(cam.getPosition() == position);
    glm::vec3 u = cam.getU();
    ASSERT_TRUE(glm::dot(u, w) == 0);
    glm::vec3 v = cam.getV();
    ASSERT_TRUE(glm::dot(v, w) == 0);
    ASSERT_TRUE(glm::dot(v, u) == 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}