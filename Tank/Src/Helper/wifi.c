#include "wifi.h"

// 协议标记定义
static const char PROTOCOL_START[] = "CSharpST{";
static const char PROTOCOL_END[] = "}CSharpED";
static const uint8_t START_LEN = 9; // strlen("CSharpST{")
static const uint8_t END_LEN = 8;   // strlen("}CSharpED")

// 解析状态机
typedef enum
{
    STATE_WAIT_START,
    STATE_IN_HEADER,
    STATE_IN_DATA,
    STATE_IN_END,
    STATE_COMPLETE
} ParseState;

typedef struct
{
    ParseState state;
    uint8_t match_pos;
    uint8_t field_index;
    char num_buf[32];
    uint8_t num_pos;
} ParserContext;

static ParserContext parser_ctx = {
    .state = STATE_WAIT_START,
    .match_pos = 0,
    .field_index = 0,
    .num_buf = {0},
    .num_pos = 0
};
static TankData_t TankData = {0};

// 重置解析器
static void ResetParser(void)
{
    memset((void *)&parser_ctx, 0, sizeof(ParserContext));
    parser_ctx.state = STATE_WAIT_START;
}

// 高精度字符串转double（优化版）
static double StringToDouble(const char *str)
{
    double integer = 0.0, fraction = 0.0;
    int sign = 1, decimal_places = 0;
    const char *p = str;

    if (*p == '-')
    {
        sign = -1;
        p++;
    }

    while (*p >= '0' && *p <= '9')
    {
        integer = integer * 10.0 + (*p - '0');
        p++;
    }

    if (*p == '.')
    {
        p++;
        while (*p >= '0' && *p <= '9')
        {
            fraction = fraction * 10.0 + (*p - '0');
            decimal_places++;
            p++;
        }
    }

    // 使用查表法优化pow(10,n)
    static const double pow10[] = {1e0, 1e1, 1e2, 1e3, 1e4, 1e5};
    double divisor = (decimal_places < 6) ? pow10[decimal_places] : pow(10.0, decimal_places);

    return sign * (integer + fraction / divisor);
}

void ParseTankProtocolByte(uint16_t data)
{
    uint8_t byte = (uint8_t)(data & 0xFF);

    switch (parser_ctx.state)
    {
    case STATE_WAIT_START:
        if (byte == PROTOCOL_START[parser_ctx.match_pos])
        {
            if (++parser_ctx.match_pos >= START_LEN)
            {
                parser_ctx.state = STATE_IN_DATA;
                parser_ctx.match_pos = 0;
            }
        }
        else
        {
            parser_ctx.match_pos = 0;
        }
        break;

    case STATE_IN_DATA:
        if (byte == '}')
        {
            parser_ctx.num_buf[parser_ctx.num_pos] = '\0';
            double val = StringToDouble(parser_ctx.num_buf);

            if (parser_ctx.field_index == 4)
            {
                TankData.FirePower = val;
                TankData.parsed_flags = 0x1F;
            }

            parser_ctx.state = STATE_IN_END;
            parser_ctx.match_pos = 1;
        }
        else if (byte == ',')
        {
            parser_ctx.num_buf[parser_ctx.num_pos] = '\0';
            double val = StringToDouble(parser_ctx.num_buf);

            switch (parser_ctx.field_index++)
            {
            case 0:
                TankData.LeftTrack = val;
                break;
            case 1:
                TankData.RightTrack = val;
                break;
            case 2:
                TankData.TurretH = val;
                break;
            case 3:
                TankData.TurretV = val;
                break;
            }

            parser_ctx.num_pos = 0;
        }
        else if ((byte >= '0' && byte <= '9') || byte == '.' || byte == '-')
        {
            if (parser_ctx.num_pos < sizeof(parser_ctx.num_buf) - 1)
            {
                parser_ctx.num_buf[parser_ctx.num_pos++] = byte;
            }
            else
            {
                parser_ctx.num_pos = 0;
                parser_ctx.field_index++;
            }
        }
        break;

    case STATE_IN_END:
        if (byte == PROTOCOL_END[parser_ctx.match_pos])
        {
            if (++parser_ctx.match_pos >= END_LEN)
            {
                parser_ctx.state = STATE_COMPLETE;
            }
        }
        else
        {
            ResetParser();
        }
        break;

    case STATE_COMPLETE:
        if (byte == PROTOCOL_START[0])
        {
            ResetParser();
            parser_ctx.match_pos = 1;
        }
        break;
    }
}

void Wifi_Send(const char *str)
{
    const char *p = str;
    while (p && *p != '\0')
    {
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
            ;
        USART_SendData(USART3, *p);
        p++;
    }
}

void Wifi_Init(uint32_t baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART3, ENABLE);
}

void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
    {
        uint16_t tmp = USART_ReceiveData(USART3);
        ParseTankProtocolByte(tmp);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
            ;
#if WIFI_DEBUG
        USART_SendData(USART1, tmp);
#endif
    }
}

void Wifi_Init_AP(uint32_t baud,
                  const char *wifiName, char *wifiPwd,
                  const char *ip, const char *port, const char *mask)
{
    Wifi_Init(baud);
    char buffer[128] = {'\0'};
    Wifi_Send("AT+RST\r\n");
    delay_ms(1000);
    Wifi_Send("AT+CWMODE=2\r\n");
    delay_ms(1000);
    sprintf("AT+CWSAP=\"%s\",\"%s\",1,3\r\n", wifiName, wifiPwd);
    Wifi_Send(buffer);
    memset(buffer, sizeof(buffer), 0);
    delay_ms(1000);
    Wifi_Send("AT+CIPMUX=1\r\n");
    delay_ms(1000);
    sprintf(buffer, "AT+CIPSERVER=1,%s\r\n", port);
    Wifi_Send(buffer);
    memset(buffer, sizeof(buffer), 0);
    delay_ms(1000);
    sprintf(buffer, "AT+CIPAP=\"%s\",\"%s\",\"%s\"\r\n", ip, ip, mask);
    Wifi_Send(buffer);
    delay_ms(1000);
}

void Wifi_FrameUpdate(MotorContext *motor, GunContext *gun)
{
    if (!TankData.parsed_flags)
        return;
    TankData.parsed_flags = 0;
    if (motor)
    {
        motor->vl = TankData.LeftTrack;
        motor->vr = TankData.RightTrack;
        Motor_Update(motor);
    }
    if (gun)
    {
        gun->ah = TankData.TurretH;
        gun->av = TankData.TurretV;
        gun->fire = TankData.FirePower;
        Gun_Update(gun);
    }
}
