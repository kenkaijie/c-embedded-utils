#pragma once

/**
 *  @file   Tests the static pool behaviour against it's function documentation. Note that since it also implements the object pool interface,
 *          we can reuse most of this testing for any other implementations of the object pool.
 * 
 *  @todo   Split out the object pool interfacing behavioural tests from the static pool specific unit tests.
 */

int test_static_pool_run_tests(void);