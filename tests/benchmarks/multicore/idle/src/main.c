/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/watchdog.h>
#include <zephyr/pm/pm.h>
#include <stdbool.h>

LOG_MODULE_REGISTER(idle);

#define WDT_MAX_WINDOW  2100U
#define WDT_OPT            0U
#define WDT_FEED_TRIES 5
#define WDG_FEED_INTERVAL 100U

static int wdt_channel_id;
static int err;

void hw_wdt_disable(void)
{
	const struct device *const wdt = DEVICE_DT_GET(DT_ALIAS(watchdog0));
	if (!device_is_ready(wdt)) {
		return;
	}

	wdt_disable(wdt);
}

void hw_wdt_restart(void)
{
	const struct device *const wdt = DEVICE_DT_GET(DT_ALIAS(watchdog0));
	if (!device_is_ready(wdt)) {
		return;
	}

	struct wdt_timeout_cfg wdt_config = {
		/* Reset SoC when watchdog timer expires. */
		.flags = WDT_FLAG_RESET_SOC,

		/* Expire watchdog after max window */
		.window.min = 0,
		.window.max = WDT_MAX_WINDOW,
	};

	wdt_channel_id = wdt_install_timeout(wdt, &wdt_config);
	err = wdt_setup(wdt, WDT_OPT);
	if (err) {
		return;
	}
}

int main(void)
{
	const struct device *const wdt = DEVICE_DT_GET(DT_ALIAS(watchdog0));

	if (!device_is_ready(wdt)) {
		return 0;
	}

	struct wdt_timeout_cfg wdt_config = {
		/* Reset SoC when watchdog timer expires. */
		.flags = WDT_FLAG_RESET_SOC,

		/* Expire watchdog after max window */
		.window.min = 0,
		.window.max = WDT_MAX_WINDOW,
	};

	wdt_channel_id = wdt_install_timeout(wdt, &wdt_config);
	err = wdt_setup(wdt, WDT_OPT);

	static struct pm_notifier wdt_pm_notifier = {
		.state_entry = (void (*)(enum pm_state))hw_wdt_disable,
		.state_exit  = (void (*)(enum pm_state))hw_wdt_restart,
	};
	pm_notifier_register(&wdt_pm_notifier);

	for (int i = 0; i < WDT_FEED_TRIES; ++i) {
		wdt_feed(wdt, wdt_channel_id);
		k_sleep(K_MSEC(WDG_FEED_INTERVAL));
	}

#if defined(CONFIG_SOC_NRF54H20_CPUAPP)
	//wdt_disable(wdt);
#endif

	while (1) {
		// wdt_disable(wdt);
		// k_msleep(WDT_MAX_WINDOW - 100);
		// wdt_install_timeout(wdt, &wdt_config);
		// wdt_setup(wdt, WDT_OPT);
		// wdt_feed(wdt, wdt_channel_id);
		// wdt_disable(wdt);

		// wdt_install_timeout(wdt, &wdt_config);
		// wdt_setup(wdt, WDT_OPT);
		// wdt_feed(wdt, wdt_channel_id);
		// wdt_disable(wdt);

		// wdt_install_timeout(wdt, &wdt_config);
		// wdt_setup(wdt, WDT_OPT);
		// wdt_feed(wdt, wdt_channel_id);
		// wdt_disable(wdt);

		// wdt_install_timeout(wdt, &wdt_config);
		// wdt_setup(wdt, WDT_OPT);
		// wdt_feed(wdt, wdt_channel_id);
		// wdt_disable(wdt);

		// wdt_install_timeout(wdt, &wdt_config);
		// wdt_setup(wdt, WDT_OPT);
		// wdt_feed(wdt, wdt_channel_id);
		// wdt_disable(wdt);

		// wdt_install_timeout(wdt, &wdt_config);
		// wdt_setup(wdt, WDT_OPT);
		// wdt_feed(wdt, wdt_channel_id);

		k_msleep(WDT_MAX_WINDOW - 100);


	}
	return 0;
}
