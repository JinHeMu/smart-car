#include "buzzer.h"
#include "button.h"

#define KEY_1 C4  // ���������ϰ�����Ӧ����
#define KEY_2 C26 // ���������ϰ�����Ӧ����
#define KEY_3 C27 // ���������ϰ�����Ӧ����
#define KEY_4 C31 // ���������ϰ�����Ӧ����

// ����״̬����
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;

// ��һ�ο���״̬����
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;

// �����ź���
rt_sem_t key1_sem;
rt_sem_t key2_sem;
rt_sem_t key3_sem;
rt_sem_t key4_sem;

void button_entry(void *parameter)
{

    // ���水��״̬
    key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;
    key4_last_status = key4_status;

    // ��ȡ��ǰ����״̬
    key1_status = gpio_get(KEY_1);
    key2_status = gpio_get(KEY_2);
    key3_status = gpio_get(KEY_3);
    key4_status = gpio_get(KEY_4);

    // ��⵽��������֮�󲢷ſ� �ͷ�һ���ź���
    if (key1_status && !key1_last_status)
    {
        rt_sem_release(key1_sem);
        rt_mb_send(buzzer_mailbox, 100); // ��buzzer_mailbox����100
        rt_mb_send(display_mailbox, 1);  // ��display_mailbox����1
    }
    if (key2_status && !key2_last_status)
    {
        rt_sem_release(key2_sem);
        rt_mb_send(buzzer_mailbox, 100);
        rt_mb_send(display_mailbox, 2);
    }
    if (key3_status && !key3_last_status)
    {
        rt_sem_release(key3_sem);
        rt_mb_send(buzzer_mailbox, 100);
        rt_mb_send(display_mailbox, 3);
    }
    if (key4_status && !key4_last_status)
    {
        rt_sem_release(key4_sem);
        rt_mb_send(buzzer_mailbox, 100);
        rt_mb_send(display_mailbox, 4);
    }
}

// ������ʼ��
void button_init(void)
{
    rt_timer_t timer1;

    gpio_init(KEY_1, GPI, GPIO_HIGH, GPIO_PIN_CONFIG); // ��ʼ��ΪGPIO�������� Ĭ�������ߵ�ƽ
    gpio_init(KEY_2, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
    gpio_init(KEY_3, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
    gpio_init(KEY_4, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);

    key1_sem = rt_sem_create("key1", 0, RT_IPC_FLAG_FIFO); // �����������ź��������������¾��ͷ��ź���������Ҫʹ�ð����ĵط���ȡ�ź�������
    key2_sem = rt_sem_create("key2", 0, RT_IPC_FLAG_FIFO);
    key3_sem = rt_sem_create("key3", 0, RT_IPC_FLAG_FIFO);
    key4_sem = rt_sem_create("key4", 0, RT_IPC_FLAG_FIFO);

    timer1 = rt_timer_create("button", button_entry, RT_NULL, 20, RT_TIMER_FLAG_PERIODIC);

    if (RT_NULL != timer1)
    {
        rt_timer_start(timer1);
    }
}