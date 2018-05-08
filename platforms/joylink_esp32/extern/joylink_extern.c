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

#include "nvs.h"
#include "nvs_flash.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_partition.h"

#include "tcpip_adapter.h"

#include "cJSON.h"

#include "joylink.h"
#include "joylink_packets.h"
#include "joylink_json.h"
#include "joylink_extern_json.h"

#include "esp_joylink.h"

#define JOYLINK_NVS_NAMESPACE       "joylink"
#define JOYLINK_NVS_CONFIG_NETWORK  "config_network"

typedef struct _attr_manage_{
    uint32_t power;
} esp_device_t;

esp_device_t g_esp_dev, *g_dev = &g_esp_dev;

JLDevice_t  esp_joylink_dev;

extern int joylink_parse_jlp(JLPInfo_t *jlp, char * pMsg);

int 
joylink_util_cut_ip_port(const char *ipport, char *out_ip, int *out_port);

void suffix_object(cJSON *prev,cJSON *item) {prev->next=item;item->prev=prev;}
/**
 * brief: 
 *
 * @Param:
 *
 * @Returns: 
 */
int
joylink_dev_get_random()
{
    /**
     *FIXME:must to do
     */
    int i;
    srand(time(NULL));
    i = rand();
    return i;
}

/**
 * brief:
 * Check dev net is ok. 
 * @Param: st
 *
 * @Returns: 
 *  E_RET_TRUE
 *  E_RET_FAIL
 */
E_JLRetCode_t
joylink_dev_is_net_ok()
{
    /**
     *FIXME:must to do
     */
    int ret = E_RET_TRUE;
	tcpip_adapter_ip_info_t ip_config;

	if (tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA,&ip_config) != ESP_OK) {
        return E_RET_FAIL;
    }
	
	if(ip_config.ip.addr == 0){
		ret = E_RET_FAIL;
	} else {
		ret = E_RET_TRUE;
	}
	
    return ret;
}


/**
 * brief:
 * When connecting server st changed,
 * this fun will be called.
 *
 * @Param: st
 * JL_SERVER_ST_INIT      (0)
 * JL_SERVER_ST_AUTH      (1)
 * JL_SERVER_ST_WORK      (2)
 *
 * @Returns: 
 *  E_RET_OK
 *  E_RET_ERROR
 */
E_JLRetCode_t
joylink_dev_set_connect_st(int st)
{
    /**
     *FIXME:must to do
     */
    int ret = E_RET_OK;
    return ret;
}

/**
 * brief:
 * Save joylink protocol info in flash.
 *
 * @Param:jlp 
 *
 * @Returns: 
 *  E_RET_OK
 *  E_RET_ERROR
 */
E_JLRetCode_t
joylink_dev_set_attr_jlp(JLPInfo_t *jlp)
{
    nvs_handle out_handle;
    if(NULL == jlp){
        return E_RET_ERROR;
    }
    /**
     *FIXME:must to do
     */
    log_debug("--joylink_dev_set_attr_jlp");
    if (nvs_open(JOYLINK_NVS_NAMESPACE, NVS_READWRITE, &out_handle) != ESP_OK) {
        return E_RET_ERROR;
    }
    if(strlen(jlp->feedid)){
        if (nvs_set_str(out_handle,"feedid",jlp->feedid) != ESP_OK) {
            log_debug("--set feedid fail");
            return E_RET_ERROR;
        }
    }

    if(strlen(jlp->accesskey)){
        if (nvs_set_str(out_handle,"accesskey",jlp->accesskey) != ESP_OK) {
            log_debug("--set accesskey fail");
            return E_RET_ERROR;
        }
    }

    if(strlen(jlp->localkey)){
        if (nvs_set_str(out_handle,"localkey",jlp->localkey) != ESP_OK) {
            log_debug("--set localkey fail");
            return E_RET_ERROR;
        }
    }

    if(strlen(jlp->joylink_server)){
        if (nvs_set_str(out_handle,"joylink_server",jlp->joylink_server) != ESP_OK) {
            log_debug("--set joylink_server fail");
            return E_RET_ERROR;
        }

        if (nvs_set_u16(out_handle,"server_port",jlp->server_port) != ESP_OK) {
            log_debug("--set server_port fail");
            return E_RET_ERROR;
        }
    }

    nvs_close(out_handle);
    return E_RET_OK;
}

/**
 * brief: 
 *
 * @Param: jlp
 *
 * @Returns: 
 */
E_JLRetCode_t
joylink_dev_get_idt(jl2_d_idt_t *pidt)
{
    if(NULL == pidt){
        return E_RET_ERROR; 
    }
#if 0
    strcpy(pidt->pub_key, g_esp_dev->idt.pub_key);
    strcpy(pidt->sig, g_esp_dev->idt.sig);
    strcpy(pidt->rand, g_esp_dev->idt.rand);
    strcpy(pidt->f_sig, g_esp_dev->idt.f_sig);
    strcpy(pidt->f_pub_key, g_esp_dev->idt.f_pub_key);
    strcpy(pidt->cloud_pub_key, g_esp_dev->idt.cloud_pub_key);
#endif
    return E_RET_OK;
}

/**
 * brief: 
 *
 * @Param: jlp
 *
 * @Returns: 
 */
E_JLRetCode_t
joylink_dev_get_jlp_info(JLPInfo_t *jlp)
{
    nvs_handle out_handle;
    uint8_t mac[6];
    if(NULL == jlp){
        return E_RET_ERROR;
    }
    /**
     *FIXME:must to do
     */
    int ret = E_RET_ERROR;
    size_t size = 0;
    log_debug("--joylink_dev_get_jlp_info");

    if (esp_wifi_get_mac(ESP_IF_WIFI_STA,mac) == ESP_OK) {
        sprintf(jlp->mac,MACSTR,MAC2STR(mac));
    }

    if (nvs_open(JOYLINK_NVS_NAMESPACE, NVS_READONLY, &out_handle) != ESP_OK) {
        return E_RET_ERROR;
    }

    size = sizeof(jlp->feedid);
    if (nvs_get_str(out_handle,"feedid",jlp->feedid,&size) != ESP_OK) {
        log_debug("--get feedid fail");
        return E_RET_ERROR;
    }

    size = sizeof(jlp->accesskey);
    if (nvs_get_str(out_handle,"accesskey",jlp->accesskey,&size) != ESP_OK) {
        log_debug("--get accesskey fail");
        return E_RET_ERROR;
    }

    size = sizeof(jlp->localkey);
    if (nvs_get_str(out_handle,"localkey",jlp->localkey,&size) != ESP_OK) {
        log_debug("--get localkey fail");
        return E_RET_ERROR;
    }

    size = sizeof(jlp->joylink_server);
    if (nvs_get_str(out_handle,"joylink_server",jlp->joylink_server,&size) != ESP_OK) {
        log_debug("--get joylink_server fail");
        return E_RET_ERROR;
    }

    if (nvs_get_u16(out_handle,"server_port",&jlp->server_port) != ESP_OK) {
        log_debug("--get server_port fail");
        return E_RET_ERROR;
    }

    nvs_close(out_handle);
    
    return ret;
}

/**
 * brief: 
 *
 * @Param: out_modelcode
 * @Param: out_max
 *
 * @Returns: 
 */
int
joylink_dev_get_modelcode(char *out_modelcode, int32_t out_max)
{
    if(NULL == out_modelcode || out_max < 0){
        return 0;
    }
    /**
     *FIXME:must to do
     */

    int len = 0;
#if 0
	LightCtrl_t *pCtrl = &(_g_pLightMgr->lightCtrl);
	
    char *packet_data =  joylink_dev_modelcode_info(0, pCtrl);
    if(NULL !=  packet_data){
        len = strlen(packet_data);
        printf("------>%s:len:%d\n", packet_data, len);
        if(len < out_max){
            memcpy(out_modelcode, packet_data, len); 
        }else{
            len = 0;
        }
    }

    if(NULL !=  packet_data){
        free(packet_data);
    }
#endif
    return len;
}

/**
 * brief: 
 *
 * @Param: out_snap
 * @Param: out_max
 *
 * @Returns: 
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
 *
 * @Param: out_snap
 * @Param: out_max
 * @Param: code
 * @Param: feedid
 *
 * @Returns: 
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
 *
 * @Param: json_cmd
 *
 * @Returns: 
 */
E_JLRetCode_t 
joylink_dev_lan_json_ctrl(const char *json_cmd)
{
    /**
     *FIXME:must to do
     */
    log_debug("json ctrl:%s", json_cmd);
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
    return E_RET_OK;
}

/**
 * brief: 
 *
 * @Param: src
 * @Param: src_len
 * @Param: ctr
 * @Param: from_server
 *
 * @Returns: 
 */
E_JLRetCode_t 
joylink_dev_script_ctrl(const char *src, int src_len, JLContrl_t *ctr, int from_server)
{
    if(NULL == src || NULL == ctr){
        return -1;
    }
    /**
     *FIXME:must to do
     */
    int ret = -1;
    ctr->biz_code = (int)(*((int *)(src + 4)));
    ctr->serial = (int)(*((int *)(src +8)));
	
#if 0
	LightCtrl_t *pCtrl = &(_g_pLightMgr->lightCtrl);
	
    time_t tt = time(NULL);
    log_info("bcode:%d:server:%d:time:%ld", ctr->biz_code, from_server,(long)tt);
#endif

    if(ctr->biz_code == JL_BZCODE_GET_SNAPSHOT){
        /*
         *Nothing to do!
         */
        ret = 0;
    }else if(ctr->biz_code == JL_BZCODE_CTRL){
#if 0
        joylink_dev_parse_ctrl(pCtrl, src + 12);
		
#ifdef __MTK_7687__
		gpio_ligt_ctrl(pCtrl->para_value);
#endif
		if(pCtrl->cmd == LIGHT_CMD_POWER){
			if(pCtrl->cmd == LIGHT_CTRL_ON){ //Open light.
				log_info("LIGHT_CTRL--[%d]--\n", pCtrl->para_value);
			}else{							 //Close light.
				log_info("LIGHT_CTRL--[%d]--\n", pCtrl->para_value);
			}
		}
#endif
        ret = 0;
    }else{
        log_error("unKown biz_code:%d", ctr->biz_code);
    }

    return ret;
}

/**
 * brief: 
 *
 * @Param: otaOrder
 *
 * @Returns: 
 */
E_JLRetCode_t
joylink_dev_ota(JLOtaOrder_t *otaOrder)
{
    if(NULL == otaOrder){
        return -1;
    }
    int ret = E_RET_OK;

    log_debug("serial:%d | feedid:%s | productuuid:%s | version:%d | versionname:%s | crc32:%d | url:%s\n",
     otaOrder->serial, otaOrder->feedid, otaOrder->productuuid, otaOrder->version, 
     otaOrder->versionname, otaOrder->crc32, otaOrder->url);

#ifdef __MTK_7687__
    strcpy(_g_fota_ctx.download_url, (const char*)otaOrder->url);
	//strcpy(_g_fota_ctx.download_url, (const char*)"http://192.168.1.96/mt7687_iot_sdk.bin");
    _g_fota_ctx.crc32 = otaOrder->crc32;
	joylink_fota_download_package();
#elif defined(PLATFORM_ESP32)
	JLOtaUpload_t otaUpload;
    strcpy(otaUpload.feedid, _g_pdev->jlp.feedid);
    strcpy(otaUpload.productuuid, _g_pdev->jlp.uuid);

    /**
     *FIXME:must to do
     *status,status_desc, progress
     */
    joylink_server_ota_status_upload_req(&otaUpload);
#else
    char cmd_download[1024] = {0};
    char *folder = "/home/yn/workspace/";
    sprintf(cmd_download, "wget -b %s -P %s", otaOrder->url, folder);
    system(cmd_download);
#endif

    return ret;
}

/**
 * brief: 
 */
void
joylink_dev_ota_status_upload()
{
    JLOtaUpload_t otaUpload;    
    strcpy(otaUpload.feedid, _g_pdev->jlp.feedid);
    strcpy(otaUpload.productuuid, _g_pdev->jlp.uuid);	

#ifdef __MTK_7687__
    otaUpload.status = _g_fota_ctx.upgrade_status;
    otaUpload.progress = _g_fota_ctx.progress;
    strcpy(otaUpload.status_desc, joylink_fota_get_status_desc(FOTA_ERROR_CODE_NONE));
#endif
    joylink_server_ota_status_upload_req(&otaUpload);   
}

bool esp_joylink_register_device_info(JLDevice_t* dev)
{
    if (!dev) {
        return false;
    }
    // printf("    joylink version: %s(%s)\n",JOYLINK_VERSION,JOYLINK_COMMIT_ID);
    memcpy(&esp_joylink_dev,dev,sizeof(JLDevice_t));
    
    return true;
}

bool esp_joylink_get_config_network(void)
{
	nvs_handle out_handle;
	uint8_t config_network;
	if (nvs_open(JOYLINK_NVS_CONFIG_NETWORK, NVS_READONLY, &out_handle) != ESP_OK) {
		return false;
	}

	if (nvs_get_u8(out_handle,"config_network",&config_network) != ESP_OK) {
		log_debug("--get config_network fail");
		return false;
	}

	nvs_close(out_handle);

	if (config_network == 1) {
		return true;
	}
	return false;;
}
void esp_joylink_set_config_network(bool config_network)
{
	nvs_handle out_handle;
	if (nvs_open(JOYLINK_NVS_CONFIG_NETWORK, NVS_READWRITE, &out_handle) != ESP_OK) {
		return;
	}

	if (nvs_set_u8(out_handle,"config_network",config_network?1:0) != ESP_OK) {
		log_debug("--set config_network fail");
		return;
	}
	nvs_close(out_handle);

	if (config_network) {
		esp_restart();
		for(;;){

		}
	}
}

void esp_restore_factory_setting(void)
{
	const esp_partition_t *p = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS, NULL);
	if (p == NULL) {
		return;
	}

	esp_partition_erase_range(p, 0, p->size);
	esp_restart();
	for(;;){

	}
}
