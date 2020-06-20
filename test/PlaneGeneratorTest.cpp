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
    ASSERT_TRUE(data->indices.value().size() == 6);

    for (VertexFormat vertex : data->vertices) {
        ASSERT_TRUE(vertex.normal == n);
    }

    VertexFormat vertex = data->vertices.at(0);
    ASSERT_TRUE(vertex.position == c - (d_1 + d_2) / 2.f);
    ASSERT_TRUE(vertex.uv == glm::vec2(0));

    ASSERT_EQ(glm::length(data->vertices.at(data->indices.value().at(1)).position
        - data->vertices.at(data->indices.value().at(0)).position), 1);
    ASSERT_EQ(glm::length(data->vertices.at(data->indices.value().at(2)).position
        - data->vertices.at(data->indices.value().at(0)).position), 1);

    ASSERT_EQ(glm::length(data->vertices.at(data->indices.value().at(1)).position
        - data->vertices.at(data->indices.value().at(4)).position), 1);
    ASSERT_EQ(glm::length(data->vertices.at(data->indices.value().at(2)).position
        - data->vertices.at(data->indices.value().at(4)).position), 1);

    vertex = data->vertices.at(3);
    ASSERT_TRUE(vertex.position == c + (d_1 + d_2) / 2.f);
    ASSERT_TRUE(vertex.uv == glm::vec2(1));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}