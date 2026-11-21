#include <math.h>
#include "unity.h"
#include "app_light_logic.h"

#define TEST_MAX_LUX 500.0f

void setUp(void) {}
void tearDown(void) {}

void test_zero_channel_returns_zero_lux(void)
{
    TEST_ASSERT_FLOAT_WITHIN(
        0.001f, 0.0f,
        app_light_calculate_lux(0, 100)
    );
}

void test_lux_ratio_below_050(void)
{
    TEST_ASSERT_FLOAT_WITHIN(
        0.05f, 30.4f,
        app_light_calculate_lux(1000, 0)
    );
}

void test_lux_ratio_between_050_and_061(void)
{
    TEST_ASSERT_FLOAT_WITHIN(
        0.01f, 5.35f,
        app_light_calculate_lux(1000, 550)
    );
}

void test_lux_ratio_between_061_and_080(void)
{
    TEST_ASSERT_FLOAT_WITHIN(
        0.01f, 2.09f,
        app_light_calculate_lux(1000, 700)
    );
}

void test_lux_ratio_between_080_and_130(void)
{
    TEST_ASSERT_FLOAT_WITHIN(
        0.01f, 0.34f,
        app_light_calculate_lux(1000, 1000)
    );
}

void test_lux_ratio_above_130_returns_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(
        0.001f, 0.0f,
        app_light_calculate_lux(1000, 1400)
    );
}

void test_relative_index_zero(void)
{
    TEST_ASSERT_EQUAL_UINT8(
        0, app_light_calculate_relative_index(0.0f, TEST_MAX_LUX)
    );
}

void test_relative_index_conversion(void)
{
    TEST_ASSERT_EQUAL_UINT8(
        25, app_light_calculate_relative_index(125.0f, TEST_MAX_LUX)
    );
}

void test_relative_index_rounding(void)
{
    TEST_ASSERT_EQUAL_UINT8(
        1, app_light_calculate_relative_index(2.5f, TEST_MAX_LUX)
    );
}

void test_relative_index_clamps_at_100(void)
{
    TEST_ASSERT_EQUAL_UINT8(
        100, app_light_calculate_relative_index(750.0f, TEST_MAX_LUX)
    );
}

void test_relative_index_clamps_negative_values(void)
{
    TEST_ASSERT_EQUAL_UINT8(
        0, app_light_calculate_relative_index(-10.0f, TEST_MAX_LUX)
    );
}

void test_relative_index_rejects_invalid_max_lux(void)
{
    TEST_ASSERT_EQUAL_UINT8(
        0, app_light_calculate_relative_index(100.0f, 0.0f)
    );
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_zero_channel_returns_zero_lux);
    RUN_TEST(test_lux_ratio_below_050);
    RUN_TEST(test_lux_ratio_between_050_and_061);
    RUN_TEST(test_lux_ratio_between_061_and_080);
    RUN_TEST(test_lux_ratio_between_080_and_130);
    RUN_TEST(test_lux_ratio_above_130_returns_zero);
    RUN_TEST(test_relative_index_zero);
    RUN_TEST(test_relative_index_conversion);
    RUN_TEST(test_relative_index_rounding);
    RUN_TEST(test_relative_index_clamps_at_100);
    RUN_TEST(test_relative_index_clamps_negative_values);
    RUN_TEST(test_relative_index_rejects_invalid_max_lux);

    return UNITY_END();
}
