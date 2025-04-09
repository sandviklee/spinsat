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

/**
 * @brief Bluetooth LE advertising parameters.
 *
 * This structure defines the advertising parameters for the Bluetooth LE
 * connection.
 */
static const struct bt_le_adv_param *adv_param =
    BT_LE_ADV_PARAM((BT_LE_ADV_OPT_CONNECTABLE |
                     BT_LE_ADV_OPT_USE_IDENTITY), /* Connectable advertising and
                                                     use identity address */
                    800, /* Min Advertising Interval 500ms (800*0.625ms) */
                    801, /* Max Advertising Interval 500.625ms (801*0.625ms) */
                    NULL); /* Set to NULL for undirected advertising */

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

// static unsigned char url_data[] = {0x17, '/', '/', 'g', 'i', 't', 'h', 'u',
//                                    'b',  '.', 'c', 'o', 'm', '/', 's', 'a',
//                                    'n',  'd', 'v', 'i', 'k', 'l', 'e', 'e',
//                                    '/',  's', 'p', 'i', 'n', 's', 'a', 't'};

static const struct bt_data sd[] = {
    // BT_DATA_BYTES(BT_DATA_URI, url_data, sizeof(url_data)),
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_SPINSAT_VAL)};

/**
 * @brief Write callback function for the state machine characteristic.
 *
 * @param conn Pointer to the connection object.
 * @param attr Pointer to the attribute object.
 * @param buf Pointer to the buffer containing the data to write.
 * @param len Length of the data to write.
 * @param offset Offset in the attribute value where the write should start.
 * @param flags Flags for the write operation.
 *
 * @return 0 on success, negative error code on failure.
 */
static ssize_t write_state(struct bt_conn *conn,
                           const struct bt_gatt_attr *attr, const void *buf,
                           uint16_t len, uint16_t offset, uint8_t flags) {
  LOG_INF("Attribute write, handle: %u, conn: %p", attr->handle, (void *)conn);

  if (!spinsat_callbacks.state_machine_write) {
    LOG_ERR("No callback registered for state machine write");
    return -ENOTSUP;
  }

  // NOTE: NRF52 uses little endian architecture...
  // This means that the first byte is the least significant byte
  uint16_t *val = ((uint16_t *)buf);

  if (len < 2 || len > 2) {
    LOG_ERR("Invalid length: %d, max 2 bytes", len);
    return -EINVAL;
  }

  uint8_t state = (uint8_t)(*val & 0xFF);
  uint8_t data = (uint8_t)((*val >> 8) & 0xFF);

  spinsat_callbacks.state_machine_write(state, data);

  return 0;
}

/**
 * @brief Callback function for when a connection is established.
 *
 * @param conn Pointer to the connection object.
 * @param err Error code (0 if successful).
 */
static void bluetooth_connected(struct bt_conn *conn, uint8_t err) {
  if (err) {
    LOG_ERR("Connection failed (err %d)\n", err);
    return;
  }

  if (!spinsat_callbacks.state_machine_write) {
    LOG_ERR("No callback registered for state machine write");
  } else {
    spinsat_callbacks.state_machine_write(IDLE, NO_DATA);
  }

  LOG_INF("CONNECTED: %p\n", (void *)conn);
}

/**
 * @brief Callback function for when a connection is disconnected.
 *
 * @param conn Pointer to the connection object.
 * @param reason Reason for disconnection.
 */
static void bluetooth_disconnected(struct bt_conn *conn, uint8_t reason) {
  LOG_INF("DISCONNECTED (reason %d)\n", reason);
}

/**
 * @brief Connection callbacks structure.
 *
 * This structure contains the callback functions for connection events.
 */
static struct bt_conn_cb conn_callbacks = {
    .connected = bluetooth_connected,
    .disconnected = bluetooth_disconnected,
};

/**
 * @brief GATT service definition for the SPINSAT service.
 *
 * This service contains a characteristic for the state machine.
 */
BT_GATT_SERVICE_DEFINE(spinsat_svc, BT_GATT_PRIMARY_SERVICE(BT_UUID_SPINSAT),
                       BT_GATT_CHARACTERISTIC(BT_UUID_SPINSAT_STATE,
                                              BT_GATT_CHRC_WRITE,
                                              BT_GATT_PERM_WRITE, NULL,
                                              write_state, NULL));

/**
 * @brief Initialize the callbacks structure.
 *
 * @param state_machine_write Pointer to the state machine write function.
 * @return Pointer to the initialized callbacks structure, or NULL on failure.
 */
callbacks_t *callback_init(void *state_machine_write) {
  callbacks_t *callbacks = (callbacks_t *)k_malloc(sizeof(callbacks_t));
  if (!callbacks) {
    LOG_ERR("Failed to allocate memory for callbacks\n");
    return NULL;
  }
  callbacks->state_machine_write = state_machine_write;

  return callbacks;
}

/**
 * @brief Initialize Bluetooth and start advertising.
 *
 * @param callbacks Pointer to the callbacks structure.
 * @return 0 on success, negative error code on failure.
 */
int8_t bluetooth_init(callbacks_t *callbacks) {
  int8_t err;
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

  LOG_INF("BLE & ADV STARTED\n");

  return 0;
}

/**
 * @brief Deinitialize Bluetooth and stop advertising.
 *
 * @return 0 on success, negative error code on failure.
 */
int8_t bluetooth_deinit(void) {
  int8_t err;
  err = bt_disable();
  if (err) {
    LOG_ERR("Bluetooth deinit failed (err %d)\n", err);
    return err;
  }

  err = bt_le_adv_stop();
  if (err) {
    LOG_ERR("Advertising stop failed (err %d)\n", err);
    return err;
  }

  LOG_INF("Bluetooth LE & Advertising has stopped \n");

  return 0;
}
