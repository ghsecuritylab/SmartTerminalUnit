/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      config
  * @brief:     
  * @version:   V1.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-07-21
  * @update:    
  */
#ifndef __DISTRIBUTION_CONFIG_H__
#define __DISTRIBUTION_CONFIG_H__

  //���濪��
#define THREAD_SIMSWITCH_NAME                                              "SIMSW"
#define THREAD_SIMSWITCH_PRIORITY                                          (3)
#define THREAD_SIMSWITCH_STACK_SIZE                                        (1024)
#define THREAD_SIMSWITCH_TIMESLICE                                         (20)

//�ֲ�ʽ�����߼�
#define THREAD_DISTRIBUTION_NAME                                              "distpro"
#define THREAD_DISTRIBUTION_PRIORITY                                          (4)
#define THREAD_DISTRIBUTION_STACK_SIZE                                        (1024)
#define THREAD_DISTRIBUTION_TIMESLICE                                         (20)

//UDPserver
#define THREAD_UDPSERVER_NAME                                              "udpser"
#define THREAD_UDPSERVER_PRIORITY                                          (5)
#define THREAD_UDPSERVER_STACK_SIZE                                        (1024)
#define THREAD_UDPSERVER_TIMESLICE                                         (20)

//·��ͨ��
#define THREAD_MUTAL_NAME                                              "mutal"
#define THREAD_MUTAL_PRIORITY                                          (13)
#define THREAD_MUTAL_STACK_SIZE                                        (2048)
#define THREAD_MUTAL_TIMESLICE                                         (20)


//��������߼�
#define THREAD_CONNECT_NAME                                              "connect"
#define THREAD_CONNECT_PRIORITY                                          (15)
#define THREAD_CONNECT_STACK_SIZE                                        (1024)
#define THREAD_CONNECT_TIMESLICE                                         (20)

#ifdef	__cplusplus
}
#endif

#endif
