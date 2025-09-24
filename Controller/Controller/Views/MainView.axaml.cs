using System;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using Avalonia.Controls;
using Controller.ViewModels;
using Newtonsoft.Json;

namespace Controller.Views;

public partial class MainView : UserControl
{
    TankViewModel _tankViewModel = new TankViewModel();
    public MainView()
    {
        InitializeComponent();
        _tankViewModel.LeftTrack = 1;
        _tankViewModel.CanMonoBehaviour = true;
        PointerPressed +=(s,e)=> SendDataToEsp1(
            $"CSharpST{{{_tankViewModel.LeftTrack.ToString("0.00")},{_tankViewModel.RightTrack.ToString("0.00")},{_tankViewModel.TurretH.ToString("0.00")},{_tankViewModel.TurretV.ToString("0.00")},{_tankViewModel.FirePower.ToString("0.00")}}}CSharpED");
    }

    static async Task SendDataToEsp1(string message)
    {
        // ESP-01S在AP模式下的固定IP和端口
        string espIp = "192.168.5.1";
        int espPort = 8080;

        TcpClient client = null;
        NetworkStream stream = null;

        try
        {
            // 1. 创建TcpClient并连接到ESP-01S的TCP服务器
            client = new TcpClient();
            await client.ConnectAsync(espIp, espPort); // 使用异步连接
            Console.WriteLine("成功连接到ESP-01S！");

            // 2. 获取网络流用于发送和接收数据
            stream = client.GetStream();

            // 3. 将消息转换为字节数组
            byte[] dataToSend = Encoding.UTF8.GetBytes(message);

            // 4. 发送数据
            await stream.WriteAsync(dataToSend, 0, dataToSend.Length);
            Console.WriteLine($"已发送数据：{message}");

            // --- 可选：接收ESP-01S的回复（如果需要）---
            // byte[] receiveBuffer = new byte[1024];
            // int bytesRead = await stream.ReadAsync(receiveBuffer, 0, receiveBuffer.Length);
            // string response = Encoding.UTF8.GetString(receiveBuffer, 0, bytesRead);
            // Console.WriteLine($"收到回复：{response}");

        }
        catch (Exception ex)
        {
            Console.WriteLine($"发生错误: {ex.Message}");
        }
        finally
        {
            // 5. 关闭连接
            stream?.Close();
            client?.Close();
            Console.WriteLine("连接已关闭。");
        }
    }
}