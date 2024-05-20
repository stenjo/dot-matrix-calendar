#ifdef TEST

#include "unity.h"

#include "driver.h"
#include "Mockspi_master.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_driver_WhenInit_ThenSetDefaults(void)
{
    max7219_t dev;
    uint32_t clock = 1000000;
    spi_host_device_t spi = 2;
    gpio_num_t cs = 10;

    spi_bus_add_device_ExpectAndReturn(spi, &(dev.spi_cfg), &(dev.spi_dev), 0);

    init_descriptor(&dev, spi, clock, cs);

    TEST_ASSERT_EQUAL(clock, dev.spi_cfg.clock_speed_hz);
}

#endif // TEST
