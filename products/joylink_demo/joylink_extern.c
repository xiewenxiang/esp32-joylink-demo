/* --------------------------------------------------
 * @brief: 
 *
 * @version: 1.0
 *
 * @date: 10/08/2015 09:28:27 AM
 *
 * --------------------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "esp_err.h"
#include "esp_wifi.h"
#include "tcpip_adapter.h"

#include "nvs.h"
#include "nvs_flash.h"

#include "bt.h"
#include "bta_api.h"

#include "esp_bt_device.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"

#include "cJSON.h"

#include "esp_joylink.h"

typedef struct _attr_manage_{
    uint32_t power;
} esp_device_t;

esp_device_t g_esp_dev, *g_dev = &g_esp_dev;


/**
 * brief:
 * Get dev snap shot.
 *
 * @Param:out_snap
 *
 * @Returns: snap shot len.
 */
int
joylink_dev_get_snap_shot(char *out_snap, int32_t out_max)
{
    if(NULL == out_snap || out_max < 0){
        return 0;
    }
    /**
     *FIXME:must to do
     */
    int len = 0;
    len = snprintf(out_snap,out_max, "{\"code\":0, \"streams\":[{\"current_value\":%d,\"stream_id\":\"power\"}]}", g_dev->power);

    return len;
}

/**
 * brief:
 * Get dev snap shot.
 *
 * @Param:out_snap
 *
 * @Returns: snap shot len.
 */
int
joylink_dev_get_json_snap_shot(char *out_snap, int32_t out_max, int code, char *feedid)
{
    /**
     *FIXME:must to do
     */
    sprintf(out_snap, "{\"code\":%d, \"feedid\":\"%s\"}", code, feedid);

    return strlen(out_snap);
}

/**
 * brief:
 * json ctrl.
 *
 * @Param:json_cmd
 *
 * @Returns: 
 *  E_RET_OK
 *  E_RET_ERROR
 */
E_JLRetCode_t 
joylink_dev_lan_json_ctrl(const char *json_cmd)
{
    /**
     *FIXME:must to do
     */
    printf("json ctrl:%s", json_cmd);
    int ret = E_RET_ERROR;
    if(json_cmd == NULL){
        printf("--->:ERROR: json_cmd is NULL\n");
        return ret;
    }

    cJSON * pJson = cJSON_Parse(json_cmd);

    if(NULL == pJson){
        printf("--->:cJSON_Parse is error:%s\n", json_cmd);
      return ret;
    }

    cJSON * pSub = cJSON_GetObjectItem(pJson, "cmd");
    if(NULL != pSub){
        printf("--->:cmd is %d\n",pSub->valueint);
    }

    pSub = cJSON_GetObjectItem(pJson, "data");
    if(NULL != pSub){
        cJSON * psubjson = cJSON_GetObjectItem(pSub, "streams");
        if(NULL != psubjson){
            pSub = psubjson;
            int size = cJSON_GetArraySize(pSub);
            for (int loop = 0; loop < size;loop++) {
                psubjson = cJSON_GetArrayItem(pSub,loop);

                if (NULL != psubjson) {
                    pSub = psubjson;
                    psubjson = cJSON_GetObjectItem(pSub, "stream_id");

                    if (NULL != psubjson) {
                        printf("para:%s\n",psubjson->valuestring);

                        if (strcmp("power",psubjson->valuestring) == 0) {
                            psubjson = cJSON_GetObjectItem(pSub, "current_value");
                            if (NULL != psubjson) {
                                printf("value:%s\n",psubjson->valuestring);
                                g_dev->power = atoi(psubjson->valuestring);
                            }
                        }
                    }
                }
            }
        }
    }
    
    
    cJSON_Delete(pJson);

    ret = E_RET_OK;

    return ret;
}

/**
 * brief:
 * script control.
 * @Param: 
 *
 * @Returns: 
 *  E_RET_OK
 *  E_RET_ERROR
 */
E_JLRetCode_t 
joylink_dev_script_ctrl(const char *cmd, JLContrl_t *ctr, int from_server)
{
    if(NULL == cmd|| NULL == ctr){
        return -1;
    }
    /**
     *FIXME:must to do
     */
    int ret = E_RET_ERROR;
    int offset = 0;
    int time_tmp;
    memcpy(&time_tmp, cmd + offset, 4);
    offset +=4;
    memcpy(&ctr->biz_code, cmd + offset, 4);
    offset +=4;
    memcpy(&ctr->serial, cmd + offset, 4);
    offset +=4;

    if(ctr->biz_code == JL_BZCODE_GET_SNAPSHOT){
        /*
         *Nothing to do!
         */
        ret = 0;
    }else if(ctr->biz_code == JL_BZCODE_CTRL){
        /**
         *Must to do!
         */
        printf("script ctrl:%s", cmd+offset);
        const char* json_cmd = cmd+offset;
        cJSON * pJson = cJSON_Parse(json_cmd);

        if(NULL == pJson){
            printf("--->:cJSON_Parse is error:%s\n", json_cmd);
          return ret;
        }

        cJSON * psubjson = cJSON_GetObjectItem(pJson, "streams");
        if(NULL != psubjson){
            cJSON * parrayjson = psubjson;
            int size = cJSON_GetArraySize(parrayjson);
            printf("array size:%d\n",size);
            for (int loop = 0; loop < size;loop++) {
                psubjson = cJSON_GetArrayItem(parrayjson,loop);

                if (NULL != psubjson) {
                    cJSON *pSub = psubjson;
                    psubjson = cJSON_GetObjectItem(pSub, "stream_id");

                    if (NULL != psubjson) {
                        printf("para:%s\n",psubjson->valuestring);

                        if (strcmp("power",psubjson->valuestring) == 0) {
                            psubjson = cJSON_GetObjectItem(pSub, "current_value");
                            if (NULL != psubjson) {
                                printf("value:%s\n",psubjson->valuestring);
                                g_dev->power = atoi(psubjson->valuestring);
                            }
                        }
                    }
                }
            }
        }


        cJSON_Delete(pJson);

        ret = E_RET_OK;

    }
    
    return ret;
}

