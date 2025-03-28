/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 No0ne (https://github.com/No0ne)
 *           (c) 2024 Bernd Strobel
 *           (c) 2024 pdaxrom (https://github.com/pdaxrom)
 * Modified by Chandler Klüser for MSX Goa'uld Friend, 2024
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#include "usbin.h"

typedef struct {
  const hid_report_item_t *x;
  const hid_report_item_t *y;
  const hid_report_item_t *z;
  const hid_report_item_t *lb;
  const hid_report_item_t *mb;
  const hid_report_item_t *rb;
  const hid_report_item_t *bw;
  const hid_report_item_t *fw;
} ms_items_t;

struct {
  u8 report_count;
  hid_report_info_t report_info[MAX_REPORT];
} hid_info[CFG_TUH_HID];

ms_items_t ms_items;

struct {
  u8 dev_addr;
  u8 instance;
} keyboards[8];

extern uint8_t SHIFT_UP;
extern void printChar(uint8_t character, uint8_t col, uint8_t row );
extern void clearScreen();
extern void printHome();

bool hid_parse_find_bit_item_by_page(hid_report_info_t* report_info_arr, u8 type, u16 page, u8 bit, const hid_report_item_t **item) {
  for(u8 i = 0; i < report_info_arr->num_items; i++) {
	if(report_info_arr->item[i].item_type == type && report_info_arr->item[i].attributes.usage.page == page) {
	  if(item) {
		if(i+bit < report_info_arr->num_items && report_info_arr->item[i+bit].item_type == type && report_info_arr->item[i+bit].attributes.usage.page == page) {
		  *item = &report_info_arr->item[i + bit];
		} else {
		  return false;
		}
	  }
	  return true;
	}
  }
  return false;
}

bool hid_parse_find_item_by_usage(hid_report_info_t* report_info_arr, u8 type, u16 usage, const hid_report_item_t **item) {
  for(u8 i = 0; i < report_info_arr->num_items; i++) {
	if(report_info_arr->item[i].item_type == type && report_info_arr->item[i].attributes.usage.usage == usage) {
	  if(item) {
		*item = &report_info_arr->item[i];
	  }
	  return true;
	}
  }
  return false;
}

bool hid_parse_get_item_value(const hid_report_item_t *item, const u8 *report, u8 len, s32 *value) {
  (void)len;
  if(item == NULL || report == NULL) return false;
  u8 boffs = item->bit_offset & 0x07;
  u8 pos = 8 - boffs;
  u8 offs  = item->bit_offset >> 3;
  u32 mask = ~(0xffffffff << item->bit_size);
  s32 val = report[offs++] >> boffs;
  while(item->bit_size > pos) {
	val |= (report[offs++] << pos);
	pos += 8;
  }
  val &= mask;
  if(item->attributes.logical.min < 0) {
	if(val & (1 << (item->bit_size - 1))) {
	  val |= (0xffffffff << item->bit_size);
	}
  }
  *value = val;
  return true;
}

s32 to_signed_value(const hid_report_item_t *item, const u8 *report, u16 len) {
  s32 value = 0;
  if(hid_parse_get_item_value(item, report, len, &value)) {
	s32 midval = ((item->attributes.logical.max - item->attributes.logical.min) >> 1) + 1;
	value -= midval;
	value <<= (16 - item->bit_size);
  }
  if(value > 32767) value = 32767;
  if(value < -32767) value = -32767;
  return value;
}

s8 to_signed_value8(const hid_report_item_t *item, const u8 *report, u16 len) {
  s32 value = 0;
  if(hid_parse_get_item_value(item, report, len, &value)) {
	value = (value > 127) ? 127 : (value < -127) ? -127 : value;
  }
  return value;
}

bool to_bit_value(const hid_report_item_t *item, const u8 *report, u16 len) {
  s32 value = 0;
  hid_parse_get_item_value(item, report, len, &value);
  return value ? true : false;
}

u8 hid_parse_report_descriptor(hid_report_info_t* report_info_arr, u8 arr_count, u8 const* desc_report, u16 desc_len) {
  union TU_ATTR_PACKED {
	u8 byte;
	struct TU_ATTR_PACKED {
	  u8 size: 2;
	  u8 type: 2;
	  u8 tag: 4;
	};
  } header;

  tu_memclr(report_info_arr, arr_count * sizeof(tuh_hid_report_info_t));

  u8 report_num = 0;
  hid_report_info_t* info = report_info_arr;

  u16 ri_global_usage_page = 0;
  s32 ri_global_logical_min = 0;
  s32 ri_global_logical_max = 0;
  s32 ri_global_physical_min = 0;
  s32 ri_global_physical_max = 0;
  u8 ri_report_count = 0;
  u8 ri_report_size = 0;
  u8 ri_report_usage_count = 0;

  u8 ri_collection_depth = 0;

  while(desc_len && report_num < arr_count) {
	header.byte = *desc_report++;
	desc_len--;

	u8 const tag  = header.tag;
	u8 const type = header.type;
	u8 const size = header.size;

	u32 data;
	u32 sdata;
	switch(size) {
	  case 1: data = desc_report[0]; sdata = ((data & 0x80) ? 0xffffff00 : 0 ) | data; break;
	  case 2: data = (desc_report[1] << 8) | desc_report[0]; sdata = ((data & 0x8000) ? 0xffff0000 : 0 ) | data;  break;
	  case 3: data = (desc_report[3] << 24) | (desc_report[2] << 16) | (desc_report[1] << 8) | desc_report[0]; sdata = data; break;
	  default: data = 0; sdata = 0;
	}

	switch(type) {
	  case RI_TYPE_MAIN:
		switch(tag) {
		  case RI_MAIN_INPUT:
		  case RI_MAIN_OUTPUT:
		  case RI_MAIN_FEATURE:
			u16 offset = (info->num_items == 0) ? 0 : (info->item[info->num_items - 1].bit_offset + info->item[info->num_items - 1].bit_size);
			for(u8 i = 0; i < ri_report_count; i++) {
			  if(info->num_items + i < MAX_REPORT_ITEMS) {
				info->item[info->num_items + i].bit_offset = offset;
				info->item[info->num_items + i].bit_size = ri_report_size;
				info->item[info->num_items + i].bit_count = ri_report_count;
				info->item[info->num_items + i].item_type = tag;
				info->item[info->num_items + i].attributes.logical.min = ri_global_logical_min;
				info->item[info->num_items + i].attributes.logical.max = ri_global_logical_max;
				info->item[info->num_items + i].attributes.physical.min = ri_global_physical_min;
				info->item[info->num_items + i].attributes.physical.max = ri_global_physical_max;
				info->item[info->num_items + i].attributes.usage.page = ri_global_usage_page;
				if(ri_report_usage_count != ri_report_count && ri_report_usage_count > 0) {
				  if(i >= ri_report_usage_count) {
					info->item[info->num_items + i].attributes.usage = info->item[info->num_items + i - 1].attributes.usage;
				  }
				}
			  }
			  offset += ri_report_size;
			}
			info->num_items += ri_report_count;
			ri_report_usage_count = 0;
		  break;

		  case RI_MAIN_COLLECTION:
			ri_collection_depth++;
		  break;

		  case RI_MAIN_COLLECTION_END:
			ri_collection_depth--;
			if(ri_collection_depth == 0) {
			  info++;
			  report_num++;
			}
		  break;
		}
	  break;

	  case RI_TYPE_GLOBAL:
		switch(tag) {
		  case RI_GLOBAL_USAGE_PAGE:
			if(ri_collection_depth == 0) {
			  info->usage_page = data;
			}
			ri_global_usage_page = data;
		  break;

		  case RI_GLOBAL_LOGICAL_MIN:
			ri_global_logical_min = sdata;
		  break;
		  case RI_GLOBAL_LOGICAL_MAX:
			ri_global_logical_max = sdata;
		  break;
		  case RI_GLOBAL_PHYSICAL_MIN:
			ri_global_physical_min = sdata;
		  break;
		  case RI_GLOBAL_PHYSICAL_MAX:
			ri_global_physical_max = sdata;
		  break;
		  case RI_GLOBAL_REPORT_ID:
			info->report_id = data;
		  break;
		  case RI_GLOBAL_REPORT_SIZE:
			ri_report_size = data;
		  break;
		  case RI_GLOBAL_REPORT_COUNT:
			ri_report_count = data;
		  break;
		}
	  break;

	  case RI_TYPE_LOCAL:
		if(tag == RI_LOCAL_USAGE) {
		  if(ri_collection_depth == 0) {
			info->usage = data;
		  } else {
			if(ri_report_usage_count < MAX_REPORT_ITEMS) {
			  info->item[info->num_items + ri_report_usage_count].attributes.usage.usage = data;
			  ri_report_usage_count++;
			}
		  }
		}
	  break;
	}

	desc_report += size;
	desc_len -= size;
  }

  return report_num;
}

u8 hid_parse_keyboard_modifiers(hid_report_info_t* report_info_arr, const u8 *report, u8 len) {
	u8 modifiers = 0;
	u8 bit = 0;
	for(u8 i = 0; i < report_info_arr->num_items; i++) {
		if(report_info_arr->item[i].item_type == RI_MAIN_INPUT &&
		report_info_arr->item[i].attributes.usage.page == HID_USAGE_PAGE_KEYBOARD &&
		report_info_arr->item[i].bit_size == 1 &&
		report_info_arr->item[i].bit_count == 8) {
			modifiers |= to_bit_value(&report_info_arr->item[i], report, len) << bit;
			bit++;
			if(bit == 8) break;
		}
	}
	return modifiers;
}

bool hid_parse_keyboard_is_nkro(hid_report_info_t* report_info_arr) {
	for(u8 i = 0; i < report_info_arr->num_items; i++) {
		if(report_info_arr->item[i].item_type == RI_MAIN_INPUT &&
		report_info_arr->item[i].attributes.usage.page == HID_USAGE_PAGE_KEYBOARD &&
		report_info_arr->item[i].bit_size == 1 &&
		report_info_arr->item[i].bit_count >= 32) {
		return true;
		}
	}
	return false;
}

void kb_report_receive(uint8_t modifiers, uint8_t const* report, u16 len) {
	// Report format for HID keyboard:
    // [0] Modifier keys (bitfield)
    // [1] Reserved
    // [2-7] Key codes (max 6 keys pressed simultaneously)
	if(modifiers != kb_modifiers) {
		// modifiers have changed
		uint8_t rbits = modifiers;
		uint8_t pbits = kb_modifiers;

		for(uint8_t j = 0; j < 8; j++) {
			rbits = rbits >> 1;
			pbits = pbits >> 1;
		}
		kb_modifiers = modifiers;
	}

	// go over activated non-modifier keys in prev_rpt and
	// check if they are still in the current report
	for(uint8_t i = 0; i < len; i++) {
		if(kb_keys[i]) {
			bool brk = true;
			for(uint8_t j = 0; j < len; j++) {
				if(kb_keys[i] == report[j]) {
					brk = false;
					break;
				}
			}
		}
	}

	// go over activated non-modifier keys in report and check if they were
	// already in prev_rpt.
	for(uint8_t i = 0; i < len; i++) {
		if(report[i]) {
			bool make = true;
			for(uint8_t j = 0; j < len; j++) {
				if(report[i] == kb_keys[j]) {
					make = false;
					break;
				}
			}
			// send make if key was in the current report the first time
			if(make) {
				uint8_t keycode = report[i];
				     if (keycode == 0x45) putchar(0x00); // F12 button - OSD
				else if (keycode == 0x44) putchar(0x04); // F11 button - SCANLINES
				else if (keycode == 0x43) printHome();   // F10 button - print HOME
				else if (keycode == 0x42) clearScreen(); // F9  button - clear OSD
				else if (keycode < 128) {
                    // bool shift = (modifiers & 0x22) != 0; // Left or Right Shift
    			    // ((modifiers & 0x22) != 0) ? putchar(0x05) : putchar(0x01); // Left or Right Shift
                    // if (shift) putchar(0x05);
    				// sleep_ms(1);
                    putchar(0x01);
					sleep_ms(5);
                    uint8_t byte_to_send = keycode_to_goauld[keycode];
					putchar(byte_to_send);
					sleep_ms(40);
					putchar(0x01);
					sleep_ms(5);
					putchar((uint8_t)143);
				}
			}
		}
	}

	memset(kb_keys, 0, sizeof(kb_keys));
	memcpy(kb_keys, report, len);
}

void tuh_kb_set_leds(uint8_t leds) {
	for(uint8_t i = 0; i < 8; i++) {
		if(keyboards[i].dev_addr != 0) {
		kb_leds = leds;
		tuh_hid_set_report(keyboards[i].dev_addr, keyboards[i].instance, 0, HID_REPORT_TYPE_OUTPUT, &kb_leds, sizeof(kb_leds));
		}
	}
}

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, u16 desc_len) {
	// This happens if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE.
	// Consider increasing #define CFG_TUH_ENUMERATION_BUFSIZE 256 in tusb_config.h
	if(desc_report == NULL && desc_len == 0) {
		// printf("WARNING: HID(%d,%d) skipped!\n", dev_addr, instance);
		return;
	}

	hid_interface_protocol_enum_t hid_if_proto = tuh_hid_interface_protocol(dev_addr, instance);
	u16 vid, pid;
	tuh_vid_pid_get(dev_addr, &vid, &pid);

	char* hidprotostr = "none";
	if(hid_if_proto == HID_ITF_PROTOCOL_KEYBOARD) hidprotostr = "keyboard";
	if(hid_if_proto == HID_ITF_PROTOCOL_MOUSE) hidprotostr = "mouse";
	hid_info[instance].report_count = hid_parse_report_descriptor(hid_info[instance].report_info, MAX_REPORT, desc_report, desc_len);
	if(!tuh_hid_receive_report(dev_addr, instance)) {
	} else {
		if(hid_if_proto == HID_ITF_PROTOCOL_KEYBOARD) {
			for(uint8_t i = 0; i < 8; i++) {
				if(keyboards[i].dev_addr == 0 && keyboards[i].instance == 0) {
					keyboards[i].dev_addr = dev_addr;
					keyboards[i].instance = instance;
					break;
				}
			}
		}
		board_led_write(1);
	}
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
	board_led_write(0);
	for(uint8_t i = 0; i < 8; i++) {
		if(keyboards[i].dev_addr == dev_addr && keyboards[i].instance == instance) {
			keyboards[i].dev_addr = 0;
			keyboards[i].instance = 0;
			break;
		}
	}
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, u16 len) {
	hid_report_info_t* rpt_info_arr = hid_info[instance].report_info;
	hid_report_info_t* rpt_info = NULL;

	if(hid_info[instance].report_count == 1 && rpt_info_arr[0].report_id == 0) {
		rpt_info = &rpt_info_arr[0];
	} else {
		uint8_t const rpt_id = report[0];
		for(uint8_t i = 0; i < hid_info[instance].report_count; i++) {
		if(rpt_id == rpt_info_arr[i].report_id) {
			rpt_info = &rpt_info_arr[i];
			break;
		}
		}
		report++;
		len--;
	}

  	if(!rpt_info) return;

	if(tuh_hid_get_protocol(dev_addr, instance) == HID_PROTOCOL_BOOT) {
	  u8 modifiers = report[0];
	  report++; report++;
	  kb_report_receive(modifiers, report, 6);
	} else if(rpt_info->usage_page == HID_USAGE_PAGE_DESKTOP && rpt_info->usage == HID_USAGE_DESKTOP_KEYBOARD) {
	  u8 modifiers = hid_parse_keyboard_modifiers(rpt_info, report, len);
	  if(hid_parse_keyboard_is_nkro(rpt_info)) {
		u8 current_key = 0;
		u8 newreport[sizeof(kb_keys)] = {0};
		u8 newindex = 0;
		for(u8 i = 1; i < len && i < 16; i++) {
		  for(u8 j = 0; j < 8; j++) {
			if(report[i] >> j & 1 && newindex < sizeof(kb_keys)) {
			  newreport[newindex] = current_key;
			  newindex++;
			}
			current_key++;
		  }
		}
		kb_report_receive(modifiers, newreport, sizeof(kb_keys));
	  } else if(len == 7) {
		report++;
		kb_report_receive(modifiers, report, 6);

	  } else if(len == 8 || len == 9) {
		report++; report++;
		kb_report_receive(modifiers, report, 6);
	  }
	}
  tuh_hid_receive_report(dev_addr, instance);
}
