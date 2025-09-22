using System;
using System.Text;
using System.Threading.Tasks;
using MQTTnet;
using MQTTnet.Protocol;

namespace Controller.Services.CommAdapter
{
    /// <summary>
    /// 基于MQTT协议的通信服务实现，专为物联网设备控制设计（如坦克控制系统）
    /// 功能：支持命令下发、状态上报、双向通信
    /// </summary>
    public class Mqtt : ICommService, IDisposable
    {
        private IMqttClient _mqttClient = null!;
        private bool _isConnected = false;
        private readonly string _clientIdPrefix = "TankCtrl_";

        /// <summary>
        /// 尝试异步连接到MQTT代理服务器
        /// </summary>
        /// <param name="context">连接参数数组：
        /// context[0] - 服务器地址 (string)
        /// context[1] - 服务器端口 (int)
        /// context[2] - 用户名 (string)
        /// context[3] - 密码 (string)
        /// context[4] - 自动重连间隔 (int, 可选, 毫秒)
        /// </param>
        /// <returns>连接结果对象</returns>
        public async Task<object> TryConnectAsync(params object[] context)
        {
            if (context == null || context.Length < 4)
                throw new ArgumentException("需要提供host, port, username和password");

            var host = (string)context[0];
            var port = (int)context[1];
            var username = (string)context[2];
            var password = (string)context[3];
            var reconnectDelay = context.Length > 4 ? (int)context[4] : 0;

            var factory = new MqttClientFactory();
            _mqttClient = factory.CreateMqttClient();

            // 配置连接选项
            var options = new MqttClientOptionsBuilder()
                .WithTcpServer(host, port)
                .WithCredentials(username, password)
                .WithClientId(string.Concat(_clientIdPrefix, Guid.NewGuid().ToString().AsSpan(0, 8)))
                .WithCleanSession()
                .WithKeepAlivePeriod(TimeSpan.FromSeconds(30))
                .Build();

            // 配置自动重连（如果指定）
            if (reconnectDelay > 0)
            {
                _mqttClient.DisconnectedAsync += async _ =>
                {
                    await Task.Delay(reconnectDelay);
                    await TryConnectAsync(context);
                };
            }

            // 建立连接
            var result = await _mqttClient.ConnectAsync(options);
            _isConnected = result.ResultCode == MqttClientConnectResultCode.Success;

            return new
            {
                IsSuccess = _isConnected,
                ResultCode = result.ResultCode.ToString()
            };
        }

        /// <summary>
        /// 异步发送控制指令或数据
        /// </summary>
        /// <param name="context">发送参数数组：
        /// context[0] - 主题 (string)
        /// context[1] - 消息内容 (string/byte[])
        /// context[2] - QoS等级 (可选, 默认AtLeastOnce)
        /// context[3] - 保留标志 (可选, 默认false)
        /// context[4] - 响应主题 (可选, MQTT 5.0)
        /// context[5] - 关联数据 (可选, MQTT 5.0)
        /// </param>
        /// <returns>发布结果对象</returns>
        public async Task<object> SendAsync(params object[] context)
        {
            if (!_isConnected || _mqttClient == null)
                throw new InvalidOperationException("MQTT客户端未连接");

            if (context == null || context.Length < 2)
                throw new ArgumentException("需要提供topic和payload");

            var topic = (string)context[0];
            var payload = context[1] is string ? Encoding.UTF8.GetBytes((string)context[1]) : (byte[])context[1];

            var qos = context.Length > 2
                ? (MqttQualityOfServiceLevel)context[2]
                : MqttQualityOfServiceLevel.AtLeastOnce;
            var retain = context.Length > 3 && (bool)context[3];

            var messageBuilder = new MqttApplicationMessageBuilder()
                .WithTopic(topic)
                .WithPayload(payload)
                .WithQualityOfServiceLevel(qos)
                .WithRetainFlag(retain);

            // MQTT 5.0特性支持
            if (context.Length > 4 && context[4] is string responseTopic)
            {
                messageBuilder.WithResponseTopic(responseTopic);
                if (context.Length > 5)
                {
                    messageBuilder.WithCorrelationData(Encoding.UTF8.GetBytes(context[5].ToString() ?? string.Empty));
                }
            }

            var message = messageBuilder.Build();
            var result = await _mqttClient.PublishAsync(message);

            return new
            {
                result.PacketIdentifier,
                ReasonCode = result.ReasonCode.ToString(),
                IsSuccess = result.ReasonCode == MqttClientPublishReasonCode.Success
            };
        }

        /// <summary>
        /// 异步订阅主题以接收数据或响应
        /// </summary>
        /// <param name="context">订阅参数数组：
        /// context[0] - 主题过滤器 (string)
        /// context[1] - QoS等级 (可选, 默认AtLeastOnce)
        /// context[2] - 消息处理回调
        /// </param>
        /// <returns>订阅结果对象</returns>
        public async Task<object> ReceiveAsync(params object[] context)
        {
            if (!_isConnected || _mqttClient == null)
                throw new InvalidOperationException("MQTT客户端未连接");

            if (context == null || context.Length < 1)
                throw new ArgumentException("需要提供topicFilter");

            var topicFilter = (string)context[0];
            var qos = context.Length > 1
                ? (MqttQualityOfServiceLevel)context[1]
                : MqttQualityOfServiceLevel.AtLeastOnce;

            // 注册消息处理器（如果提供）
            if (context.Length > 2 && context[2] is Action<MqttApplicationMessageReceivedEventArgs> handler)
            {
                _mqttClient.ApplicationMessageReceivedAsync += e =>
                {
                    handler(e);
                    return Task.CompletedTask;
                };
            }

            var result = await _mqttClient.SubscribeAsync(topicFilter, qos);

            return new
            {
                Topics = result.Items,
                ReasonCodes = string.Join(",", result.ReasonString)
            };
        }

        /// <summary>
        /// 断开连接并释放资源
        /// </summary>
        public async void Dispose()
        {
            try
            {
                if (_isConnected)
                {
                    await _mqttClient.DisconnectAsync();
                }

                _mqttClient.Dispose();
            }
            catch
            {
                // ignored
            }
        }
    }
}