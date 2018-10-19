/*
 * test_bufer.cxx
 *
 *  Created on: 19 paź 2018
 *      Author: krystian
 */






#include <gtest/gtest.h>
#include <np_bridge/np_bridge.h>
#include <vector>

using namespace np_bridge;
TEST(numpy_tools, CBuffer_allocation) {
  NumpyArrayCBuffer2D buffer(2, 3);
  buffer.at(0, 0) = 0;
  buffer.at(0, 1) = 11;
  buffer.at(1, 0) = 10;
  buffer.at(1, 2) = 14;
  buffer.at(1, 2) = 14;
  ASSERT_EQ(buffer.at_c(1, 2), 14);
  ASSERT_EQ(buffer.at_c(1, 0), 10);
  ASSERT_EQ(buffer.at_c(0, 1), 11);
  ASSERT_EQ(buffer.at_c(0, 0), 0);
}

TEST(numpy_tools, CBuffer_allocation_fortran_order) {
  NumpyArrayCBuffer2D buffer(2, 3, true);
  buffer.at(0, 0) = 0;
  buffer.at(0, 1) = 11;
  buffer.at(1, 0) = 10;
  buffer.at(1, 2) = 14;
  buffer.at(1, 2) = 14;
  ASSERT_EQ(buffer.at_c(1, 2), 14);
  ASSERT_EQ(buffer.at_c(1, 0), 10);
  ASSERT_EQ(buffer.at_c(0, 1), 11);
  ASSERT_EQ(buffer.at_c(0, 0), 0);
}

TEST(numpy_tools, CBuffer_view_access_allocated_buffer) {
  double vec_buffer[4]{1.0, 2.0, 3., 4.};
  NumpyArrayCBufferView2D buf_view((char *)vec_buffer, 2, 2);
  double actual = buf_view.at_c(0, 1);
  ASSERT_EQ(actual, 3.0);
  double actual2 = buf_view.at_c(1, 0);
  ASSERT_EQ(actual2, 2.0);
  NumpyArrayCBufferView2D buf_view_fortran((char *)vec_buffer, 2, 2, false);
  double actual3 = buf_view_fortran.at_c(0, 1);
  ASSERT_EQ(actual3, 2.0);
  double actual4 = buf_view_fortran.at_c(1, 0);
  ASSERT_EQ(actual4, 3.0);
}
