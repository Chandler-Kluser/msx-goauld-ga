#ifndef USBIN_H
#define USBIN_H

#include "bsp/board_api.h"
#include <stdint.h>
#include "keymaps.h"

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define MAX_REPORT 4
#define MAX_REPORT_ITEMS 32

typedef struct {
  u16 page;
  u16 usage;
} hid_usage_t;

typedef struct {
  u32 type;
  u8 exponent;
} hid_unit_t;

typedef struct {
  s32 min;
  s32 max;
} hid_minmax_t;

typedef struct {
  hid_usage_t usage;
  hid_unit_t unit;
  hid_minmax_t logical;
  hid_minmax_t physical;
} hid_report_item_attributes_t;

typedef struct {
  u16 bit_offset;
  u8 bit_size;
  u8 bit_count;
  u8 item_type;
  u16 item_flags;
  hid_report_item_attributes_t attributes;
} hid_report_item_t;

typedef struct {
  u8 report_id;
  u8 usage;
  u16 usage_page;
  u8 num_items;
  hid_report_item_t	item[MAX_REPORT_ITEMS];
} hid_report_info_t;

extern uint8_t kb_leds;
extern uint8_t kb_modifiers;
extern uint8_t kb_keys[120];
extern uint8_t isMounted;
extern tusb_desc_device_t desc_device;

bool hid_parse_find_bit_item_by_page(hid_report_info_t* report_info_arr, u8 type, u16 page, u8 bit, const hid_report_item_t **item);
bool hid_parse_find_item_by_usage(hid_report_info_t* report_info_arr, u8 type, u16 usage, const hid_report_item_t **item);
bool hid_parse_get_item_value(const hid_report_item_t *item, const u8 *report, u8 len, s32 *value);
s32 to_signed_value(const hid_report_item_t *item, const u8 *report, u16 len);
s8 to_signed_value8(const hid_report_item_t *item, const u8 *report, u16 len);
bool to_bit_value(const hid_report_item_t *item, const u8 *report, u16 len);
u8 hid_parse_report_descriptor(hid_report_info_t* report_info_arr, u8 arr_count, u8 const* desc_report, u16 desc_len);
u8 hid_parse_keyboard_modifiers(hid_report_info_t* report_info_arr, const u8 *report, u8 len);
bool hid_parse_keyboard_is_nkro(hid_report_info_t* report_info_arr);
void kb_report_receive(u8 modifiers, u8 const* report, u16 len);
void tuh_kb_set_leds(u8 leds);
void tuh_hid_mount_cb(u8 dev_addr, u8 instance, u8 const* desc_report, u16 desc_len);
void tuh_hid_umount_cb(u8 dev_addr, u8 instance);
void tuh_hid_report_received_cb(u8 dev_addr, u8 instance, u8 const* report, u16 len);

#endif