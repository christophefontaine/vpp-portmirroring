/*
 * Copyright (c) 2015 Cisco and/or its affiliates.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if DPDK==1

#include <vnet/plugin/plugin.h>
#include <vnet/api_errno.h>
#include <portmirroring/port_mirroring.h>

int pm_conf(u8 dst_interface, u8 is_del)
{
  pm_main_t *pm = &pm_main;

  if(is_del == 0) {
      vec_add1 (pm->sw_if_index, dst_interface);
  } else {
      int i;
      for (i = 0; i < vec_len (pm->sw_if_index); i++)
      {
        if (pm->sw_if_index[i] == dst_interface) {
            vec_delete(pm->sw_if_index, dst_interface, i);
        }
      }
  }

  return 0;  
}

static clib_error_t *
set_pm_command_fn (vlib_main_t * vm,
        unformat_input_t * input, vlib_cli_command_t * cmd)
{
    pm_main_t *pm = &pm_main;
    int enable_disable = 1;
    int sw_if_index = ~0;

    while (unformat_check_input (input) != UNFORMAT_END_OF_INPUT)
    {
        if (unformat (input, "to %U", unformat_vnet_sw_interface,
                    pm->vnet_main, &sw_if_index));
        else if (unformat (input, "del"))
            enable_disable = 0;
        else if (unformat (input, "disable"))
            enable_disable = 0;
        else
            break;
    }

    if (sw_if_index == ~0)
        return clib_error_return (0, "interface required");


    if (enable_disable)
    {
        vec_add1 (pm->sw_if_index, sw_if_index);
        return 0;
    }
    else
    {
        int i;
        for (i = 0; i < vec_len (pm->sw_if_index); i++)
        {
            if (pm->sw_if_index[i] == sw_if_index) {
                vec_delete(pm->sw_if_index, sw_if_index, i);
                return 0;
            }
        }
        return clib_error_return (0, "interface %d not configured", sw_if_index);
    }


    return 0;
}

VLIB_CLI_COMMAND (set_pm_command, static) =
{
    .path = "set pm",
    .short_help = "set pm to <intfc> [del]",
    .function = set_pm_command_fn,
};


static clib_error_t *
pm_init (vlib_main_t * vm)
{
    pm_main_t *pm = &pm_main;

    pm->pm_in_hit_node_index = pm_in_hit_node.index;
    pm->pm_out_hit_node_index = pm_out_hit_node.index;

    uword l2in_classify_idx =  vlib_get_node_by_name(vm, (u8*) "l2-input-classify")->index;
    uword l2out_classify_idx =  vlib_get_node_by_name(vm, (u8*) "l2-output-classify")->index;

    vlib_node_add_next(vm, l2in_classify_idx, pm->pm_in_hit_node_index);
    vlib_node_add_next(vm, l2out_classify_idx, pm->pm_out_hit_node_index);

    pm->interface_output_node_index = vlib_get_node_by_name(vm, (u8*) "interface-output")->index;

    return 0;
}

VLIB_INIT_FUNCTION (pm_init);

clib_error_t *
vlib_plugin_register (vlib_main_t * vm,
                      vnet_plugin_handoff_t * h,
                      int from_early_init)
{
  clib_error_t *error = 0;
  pm_main_t *pm = &pm_main;
  pm->vnet_main = vnet_get_main();
  pm->vlib_main = vm;
  return error;
}

#else
#error "port mirroring requires DPDK"
#endif
