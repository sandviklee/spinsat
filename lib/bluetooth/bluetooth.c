/*
 * Copyright (c) 2025 NTNU EiT - TTK4852
 * SPDX-License-Identifier: Apache-2.0
 */

#include "zephyr/sys/util_macro.h"
#include <app/lib/bluetooth.h>
#include <stdint.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/mgmt/mcumgr/transport/smp_bt.h>
#include <zephyr/settings/settings.h>

LOG_MODULE_REGISTER(bluetooth, CONFIG_APP_LOG_LEVEL);

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

static const struct bt_data ad[] = {
    // Advertiement flags
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    // Advertisement packet data
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static unsigned char url_data[] = {0x17, '/', '/', 'g', 'i', 't', 'h', 'u',
                                   'b',  '.', 'c', 'o', 'm', '/', 's', 'a',
                                   'n',  'd', 'v', 'i', 'k', 'l', 'e', 'e',
                                   '/',  's', 'p', 'i', 'n', 's', 'a', 't'};

static const struct bt_data sd[] = {
    BT_DATA(BT_DATA_URI, url_data, sizeof(url_data)),
};

BT_GATT_SERVICE_DEFINE(spinsat_svc, BT_GATT_PRIMARY_SERVICE(BT_UUID_SPINSAT),
                       BT_GATT_CHARACTERISTIC(BT_UUID_SPINSAT_BUTTON, BT_GATT_CHRC_READ, BT_GATT_PERM_READ, read_button, _write, _user_data)

);

int32_t bluetooth_init(void) {
  int err = 0;
  err = bt_enable(NULL);
  if (err) {
    LOG_ERR("Bluetooth init failed (err %d)\n", err);
    return -1;
  }

  smp_bt_register();

  err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), ad, ARRAY_SIZE(ad));
  if (err) {
    LOG_ERR("Advertising failed to start (err %d)\n", err);
    return err;
  }

  LOG_INF("Bluetooth initialized\n");
  return 0;
}

int32_t bluetooth_publish(bluetooth_payload *payload) {
  struct bt_gatt_notify_params params = {0};
  const struct bt_gatt_attr *attr = NULL;
}
