#define CATCH_CONFIG_MAIN

#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream>
#include <list>
#include <limits>

// #include "../include/vibes.h" // might need to change those includes

#include <catch2/catch.hpp>

#define protected public

//TEST_CASE("Cell manipulation")
//{
//    Cell *c11 = new Cell(1,1);
//    Cell *c12 = new Cell(1,2);
//
//    SECTION("Test 1: Initialize Cells")
//    {
//        REQUIRE (c11->m_x == 1);
//        REQUIRE (c11->m_y == 1);
//
//        REQUIRE (c12->m_x == 1);
//        REQUIRE (c12->m_y == 2);
//    }
//
//    SECTION("Test 2: Connect Cells")
//    {
//        REQUIRE (c11->m_nb_neighb == 0);
//        Cell::add_neighb(c11,c12);
//        REQUIRE (c11->m_nb_neighb == 1);
//    }
//
//    SECTION("Test 3: Connect Cells")
//    {
//        Cell *c11 = new Cell(1,1);
//        Cell *c12 = new Cell(1,2);
//
//        REQUIRE (c11->m_nb_neighb == 0);
//        c11->add_neighb(c12);
//        REQUIRE (c11->m_nb_neighb == 1);
//
//    }
//}
//
//TEST_CASE("Cell display")
//{
//    Cell *c11 = new Cell(1,1);
//
//    SECTION("Test 1: toString")
    {
        std::string expected = "(1,1)";

        REQUIRE(expected.compare(c11->toString()));
    }
}

//TEST_CASE("Path")
//{
//
//}
//
//TEST_CASE("Maze")
//{
//
//}
