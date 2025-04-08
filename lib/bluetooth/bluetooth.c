/*
 * Copyright (c) 2025 NTNU EiT - TTK4852
 * SPDX-License-Identifier: Apache-2.0
 */

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

static callbacks_t spinsat_callbacks;

static const struct bt_le_adv_param *adv_param =
    BT_LE_ADV_PARAM((BT_LE_ADV_OPT_CONNECTABLE |
                     BT_LE_ADV_OPT_USE_IDENTITY), /* Connectable advertising and
                                                     use identity address */
                    800, /* Min Advertising Interval 500ms (800*0.625ms) */
                    801, /* Max Advertising Interval 500.625ms (801*0.625ms) */
                    NULL); /* Set to NULL for undirected advertising */

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static unsigned char url_data[] = {0x17, '/', '/', 'g', 'i', 't', 'h', 'u',
                                   'b',  '.', 'c', 'o', 'm', '/', 's', 'a',
                                   'n',  'd', 'v', 'i', 'k', 'l', 'e', 'e',
                                   '/',  's', 'p', 'i', 'n', 's', 'a', 't'};

static const struct bt_data sd[] = {
    BT_DATA(BT_DATA_URI, url_data, sizeof(url_data)),
};

static ssize_t write_state(struct bt_conn *conn,
                           const struct bt_gatt_attr *attr, const void *buf,
                           uint16_t len, uint16_t offset, uint8_t flags) {
  LOG_INF("Attribute write, handle: %u, conn: %p", attr->handle, (void *)conn);

  if (!spinsat_callbacks.state_machine_write) {
    LOG_ERR("No callback registered for state machine write");
    return -ENOTSUP;
  }

  uint8_t val = *((uint8_t *)buf);

  spinsat_callbacks.state_machine_write(val, (uint8_t *)buf + 1, len - 1);
}

static void bluetooth_connected(struct bt_conn *conn, uint8_t err) {
  if (err) {
    LOG_ERR("Connection failed (err %d)\n", err);
    return;
  }
  LOG_INF("Connected\n");
}

static void bluetooth_disconnected(struct bt_conn *conn, uint8_t reason) {
  LOG_INF("Disconnected (reason %d)\n", reason);
}

static struct bt_conn_cb conn_callbacks = {
    .connected = bluetooth_connected,
    .disconnected = bluetooth_disconnected,
};


BT_GATT_SERVICE_DEFINE(spinsat_svc, BT_GATT_PRIMARY_SERVICE(BT_UUID_SPINSAT),
                       BT_GATT_CHARACTERISTIC(BT_UUID_SPINSAT_STATE,
                                              BT_GATT_CHRC_WRITE,
                                              BT_GATT_PERM_WRITE, NULL,
                                              write_state, NULL));

callbacks_t *callback_init(void *state_machine_write) {
  callbacks_t *callbacks = (callbacks_t *)k_malloc(sizeof(callbacks_t));
  if (!callbacks) {
    LOG_ERR("Failed to allocate memory for callbacks\n");
    return NULL;
  }
  callbacks->state_machine_write = state_machine_write;

  return callbacks;
}

int16_t bluetooth_init(callbacks_t *callbacks) {
  int err;
  err = bt_enable(NULL);
  if (err) {
    LOG_ERR("Bluetooth init failed (err %d)\n", err);
    return -1;
  }
  bt_conn_cb_register(&conn_callbacks);

  if (!callbacks) {
    LOG_ERR("No callbacks provided\n");
    return -1;
  }

  spinsat_callbacks.state_machine_write = callbacks->state_machine_write;

  err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
  if (err) {
    LOG_ERR("Advertising failed to start (err %d)\n", err);
    return err;
  }

  LOG_INF("Bluetooth initialized\n");

  return 0;
}
