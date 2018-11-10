/**
  ******************************************************************************
  * @file media.h 
  * @author leon.xie
  * @version v1.0.0
  * @date 2018-10-20 16:42:37
  * @brief This file contains all the functions prototypes for the BOARD 
  *  media 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __MEDIA_H
#define __MEDIA_H

/* Includes ------------------------------------------------------------------*/

#include "configs.h" 
#include "type.h" 

#ifdef  __cplusplus
extern "C" {
#endif

/* Exported typedef ----------------------------------------------------------*/
typedef enum{
	MEDIA_TYPE_VIDEO,	//视频
	MEDIA_TYPE_AUDIO,	//音频
	MEDIA_TYPE_BUTT
}media_type_e;


//音频数据.
typedef struct media_audio_info_s {
	int res;
}media_audio_info_t;

//视频数据.
typedef struct media_video_info_s {
	int res;
}media_video_info_t;

//音视频媒体数据.
typedef struct media_info_s {
	union {
		media_audio_info_t audio;
		media_video_info_t video;
	};
}media_info_t;
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/***************************************************
 * Function : media_update
 * Author : leon.xie
 * Creat Date : 2018/11/10  21:1:34
 * Description : 音视频数据更新
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int media_update(media_type_e type,media_info_t *media);

/***************************************************
 * Function : media_read
 * Author : leon.xie
 * Creat Date : 2018/11/10  20:23:55
 * Description : 音视频读取
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int media_read(media_type_e type,media_info_t *mediaOutput,L64 time);

#ifdef  __cplusplus
}
#endif

#endif  /* __MEDIA_H */


