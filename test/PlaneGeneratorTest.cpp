#include <gtest/gtest.h>
#include <iostream>

#include <GLRF/PlaneGenerator.hpp>

using namespace GLRF;

TEST (PlaneGeneration, SimplestParameters) {
    PlaneGenerator gen = PlaneGenerator();
    glm::vec3 c(0);
    glm::vec3 n(0, 1, 0);
    glm::vec3 d_1(1, 0, 0);
    glm::vec3 d_2 = glm::cross(n, d_1);
    float side_length = 1;
    float uv_scaling = 1;
    unsigned int tesselation = 0;
    auto data = gen.create(c, n, d_1, side_length, tesselation, uv_scaling);

    ASSERT_TRUE(data->vertices.size() == 4);
    ASSERT_TRUE(data->indices.has_value());
    ASSERT_TRUE(data->indices.value().size() == 6);

    for (VertexFormat vertex : data->vertices) {
        ASSERT_TRUE(vertex.normal == n);
    }

    VertexFormat vertex = data->vertices.at(0);
    ASSERT_TRUE(vertex.position == c - (d_1 + d_2) / 2.f);
    ASSERT_TRUE(vertex.uv == glm::vec2(0));

    glm::vec3 sum_vec = glm::vec3(0.f);
    for (VertexFormat vertex : data->vertices) {
        sum_vec += vertex.position - c;
    }
    ASSERT_TRUE(glm::length(sum_vec) < FLT_EPSILON);

    vertex = data->vertices.at(3);
    ASSERT_TRUE(vertex.position == c + (d_1 + d_2) / 2.f);
    ASSERT_TRUE(vertex.uv == glm::vec2(1));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}